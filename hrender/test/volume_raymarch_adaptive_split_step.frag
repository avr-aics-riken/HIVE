#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
#extension GL_OES_texture_3D : enable
#endif

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler3D tex0;
uniform vec2      resolution;
uniform vec3      eye;
uniform vec3      lookat;
uniform vec3      up;
uniform vec3      eyedir;
uniform vec3 volumescale;
uniform vec3 volumedim;
uniform vec3 offset;

// Transfer function parameters
// uniform sampler2D tf_tex;
uniform float     tf_min;
uniform float     tf_max;

uniform float u_numSteps;

vec4 densityToColor(float dens)
{
    if (dens < 0.01) {
        return vec4(0.0, 0.0, 0.0, 0.005);
    } else if (dens < 0.25) {
        float t = 4.0 * dens; // [0, 1]
        return vec4(t, 0.2*(1.0 - t), 0.0, .5);
    } else if (dens < 0.5) {
        float t = 4.0 * (dens - 0.25); // [0, 1]
        return vec4(0.0, t, 1.0-t,0.5);
    } else if (dens < 0.75) {
        float t = 4.0 * (dens - 0.5); // [0, 1]
        return vec4(1.0-t, 0.0, t,0.5);
    } else if (dens < 1.0) {
		float t = 4.0 * (dens - 0.75); // [0, 1]
        return vec4(0.4-0.4*t, 0.4-0.4*t, 0.2*t,0.5);
    } else if (dens < 10.0) {
		float t = 0.1 * (dens - 1.0); // [0, 1]
        return vec4(0.5*t, 0.0, 0.2-0.2*t,0.5);
	} else {
        return vec4(0.8, 0.0, 0.0,0.5);
	}
}

void orthoBasis(out vec3 basis0,out vec3 basis1,out vec3 basis2, vec3 n) {
    basis2 = vec3(n.x, n.y, n.z);
    basis1 = vec3(0.0, 0.0, 0.0);

    if ((n.x < 0.6) && (n.x > -0.6))
        basis1.x = 1.0;
    else if ((n.y < 0.6) && (n.y > -0.6))
        basis1.y = 1.0;
    else if ((n.z < 0.6) && (n.z > -0.6))
        basis1.z = 1.0;
    else
        basis1.x = 1.0;

    basis0 = cross(basis1, basis2);
    basis0 = normalize(basis0);

    basis1 = cross(basis2, basis0);
    basis1 = normalize(basis1);
}

vec4 samplingVolume(vec3 texpos) {
	vec4 col = texture3D(tex0, texpos);
	return col;
}

int IntersectP(vec3 rayorg, vec3 raydir, vec3 pMin, vec3 pMax, out float hit0, out float hit1) {
    float t0 = -10000.0, t1 = 10000.0;
    hit0 = t0;
    hit1 = t1;

    vec3 tNear = (pMin - rayorg) / raydir;
    vec3 tFar  = (pMax - rayorg) / raydir;
    if (tNear.x > tFar.x) {
        float tmp = tNear.x;
        tNear.x = tFar.x;
        tFar.x = tmp;
    }
    t0 = max(tNear.x, t0);
    t1 = min(tFar.x, t1);

    if (tNear.y > tFar.y) {
        float tmp = tNear.y;
        tNear.y = tFar.y;
        tFar.y = tmp;
    }
    t0 = max(tNear.y, t0);
    t1 = min(tFar.y, t1);

    if (tNear.z > tFar.z) {
        float tmp = tNear.z;
        tNear.z = tFar.z;
        tFar.z = tmp;
    }
    t0 = max(tNear.z, t0);
    t1 = min(tFar.z, t1);

    if (t0 <= t1) {
        hit0 = t0;
        hit1 = t1;
        return 1;
    }
    return 0;
}

void  main(void) {
	vec3 p;
    vec3 n;
    vec3 dir;
    isectinfo(p, n, dir);

    vec3 rayorg = eye;
	vec3 raydir = p - eye;
    raydir = normalize(raydir);
    vec3 pmin = vec3(-0.5, -0.5, -0.5) * volumescale + offset;
    vec3 pmax = vec3( 0.5,  0.5,  0.5) * volumescale + offset;
    float tmin, tmax;
    IntersectP(rayorg, raydir, pmin, pmax, tmin, tmax);
	tmin = max(0.0, tmin);
	tmax = max(0.0, tmax);

    // raymarch.
	vec4 col  = vec4(0.0, 0.0, 0.0, 0.0);

    float thickness = max(max(volumescale.x, volumescale.y),
                          volumescale.z) / u_numSteps;
    float tOffset = thickness / 10.;
    float t = tmin + tOffset;
    vec3 nx, ny, nz;
    orthoBasis(nx, ny, nz, n);

    float dist = abs(dot(nx, (tmax * raydir) - tmin * raydir));
    dist = max(dist, abs(dot(ny, (tmax * raydir) - tmin * raydir)));
    dist = max(dist, abs(dot(nz, (tmax * raydir) - tmin * raydir)));

    float maxSamples = floor(dist / thickness) + 1.;
    float tstep = (tmax - tmin - tOffset) / maxSamples;

	float i = 0.0;
	while((i < maxSamples) &&
          (min(min(col.x, col.y), col.z) < 1.0) &&
          (col.w < 0.999)) {
		vec3 p = rayorg + t * raydir; // [-0.5*volscale, 0.5*volscale]^3 + offset
		vec3 texpos = (p - offset) / volumescale + 0.5; // [0, 1]^3
		vec4 temp = samplingVolume(texpos);

        if (abs(temp.x) < 0.00001 && abs(temp.x) < 0.00001 && abs(temp.y) < 0.00001 && abs(temp.z) < 0.00001) {
            // Skip
        } else if (temp.x < tf_min) {
            // Skip
        } else if (temp.x > tf_max) {
            // Skip
        } else {

            float f = clamp(temp.x, tf_min, tf_max);
            float x = (f - tf_min) / (tf_max - tf_min); // normalize

            vec4 tfCol = densityToColor(x);
            tfCol.w *= min(min(volumedim.x, volumedim.y), volumedim.z) / u_numSteps;

            tfCol.rgb *= tfCol.a;
            col = (1. - col.a) * tfCol + col;
        }

		t += tstep;
		i = i + 1.0;
	}

	col.w = 1.0;

	gl_FragColor = col;
}
