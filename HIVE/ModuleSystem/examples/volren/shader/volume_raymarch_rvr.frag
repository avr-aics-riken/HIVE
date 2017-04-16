#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
#extension GL_OES_texture_3D : enable
#endif

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler3D tex0;
//uniform vec2      resolution;
uniform vec3      eye;
//uniform vec3      lookat;
//uniform vec3      up;
//uniform vec3      eyedir;
uniform vec3 volumescale;
uniform vec3 volumedim;
uniform vec3 u_offset;

// Transfer function parameters
uniform sampler2D tf_tex;
uniform float     tf_min;
uniform float     tf_max;

uniform float u_samples;
uniform float u_enableLighting;
uniform vec4 u_light;//(posX, posY, posZ, intensity)
uniform vec3 u_attenuation;//(const, linear, quadratic);
uniform vec3 u_ambient;
uniform vec3 u_specular;

uniform mat4 lsgl_WorldInverse; // inverse of object to world matrix

// gradient map
uniform float u_enableGradientMap;
uniform sampler2D u_gradTex;

vec3 computeGradient(vec3 t){
    vec3 sample1, sample2;
    vec3 ofst = 1.0 / (volumedim) / 2.;
    sample1.x = texture3D(tex0, t-vec3(ofst.x, 0.0, 0.0)).x;
	sample2.x = texture3D(tex0, t+vec3(ofst.x, 0.0, 0.0)).x;
	sample1.y = texture3D(tex0, t-vec3(0.0, ofst.y, 0.0)).x;
	sample2.y = texture3D(tex0, t+vec3(0.0, ofst.y, 0.0)).x;
	sample1.z = texture3D(tex0, t-vec3(0.0, 0.0, ofst.z)).x;
	sample2.z = texture3D(tex0, t+vec3(0.0, 0.0, ofst.z)).x;
	vec3 N	= sample1 - sample2;
    vec4 N4 = lsgl_WorldInverse * vec4(N, 0);
    return N4.xyz;
}

vec4 samplingVolume(vec3 texpos) {
	vec4 col = texture3D(tex0, texpos);
	return col;
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

vec4 densityToColor( float d, vec3 eye, vec3 shadingPos, vec3 texPos ){
    vec4 color = texture2D(tf_tex, vec2(d, 0.5));
    if(u_enableLighting == 0. && u_enableGradientMap == 0.)
        return color;
    float a = color.a;
    vec3 n = computeGradient(texPos);
    if(u_enableGradientMap != 0.){
        float w = length(n) / 1.73205;
        vec4 gCol = texture2D(u_gradTex, vec2(w, 0.5));
        a *= gCol.x;
        color.a = a;
        if(u_enableLighting == 0.)
            return color;
    }

    if(length(n) == 0.0)
        return color;

    n = normalize(n);
    vec3 lightVec = u_light.xyz - shadingPos;
    float dist = length(lightVec);
    lightVec = normalize(lightVec);
    vec3 revLightVec = vec3(-lightVec.x, -lightVec.y, -lightVec.z);
    vec3 viewVec = normalize(-shadingPos);
    vec3 halfVec = normalize(lightVec + viewVec);
    vec3 halfVec2 = normalize(revLightVec + viewVec);

    float specular = pow(max(dot(n, halfVec), 0.), u_light.w);
    float specular2 = pow(max(dot(n, halfVec2), 0.), u_light.w);
    float attenuation = 1.0 / (u_attenuation.x
                               + u_attenuation.y * dist
                               + u_attenuation.z * dist * dist);
    float diffuse = (dot(lightVec, n)) * 0.66;
    float diffuse2 = (dot(revLightVec, n)) * 0.33;

    color.rgb += color.rgb * diffuse
        + color.rgb * diffuse2
        + u_ambient
        + u_specular * specular * attenuation
        + u_specular * specular2 * attenuation;
    color.rgb = min(color.rgb, vec3(1));
    color.a = a;
    return color;
}

void  main(void) {
    int depth;
    raydepth(depth);

    if (depth > 15) {
        gl_FragColor = vec4(0);
        return;
    }

	vec3 p;
    vec3 n;
    vec3 dir;
    isectinfo(p, n, dir);

    vec3 rayorg = eye;
	vec3 raydir = p - eye;
    raydir = normalize(raydir);
    vec3 pmin = vec3(-0.5, -0.5, -0.5) * volumescale + u_offset;
    vec3 pmax = vec3( 0.5,  0.5,  0.5) * volumescale + u_offset;
    float tmin, tmax;
    IntersectP(rayorg, raydir, pmin, pmax, tmin, tmax);
	tmin = max(0.0, tmin);
	tmax = max(0.0, tmax);

    // raymarch.
    float tstep = (tmax - tmin) / u_samples;

	vec4 col  = vec4(0.0, 0.0, 0.0, 0.0);

	float i = 0.0;
    float thickness = max(max(volumescale.x, volumescale.y), volumescale.z) / u_samples;
    float tOffset = thickness / 10.;
    float t = tmin + tOffset;

    vec3 nx, ny, nz;
    orthoBasis(nx, ny, nz, n);

    float dist = abs(dot(nx, (tmax * raydir) - tmin * raydir));
    dist = max(dist, abs(dot(ny, (tmax * raydir) - tmin * raydir)));
    dist = max(dist, abs(dot(nz, (tmax * raydir) - tmin * raydir)));

    float samples = floor(dist / thickness) + 1.;
    tstep = (tmax - tmin - tOffset) / samples;
    //    samples = u_samples;

    float tminOffset = tOffset * 0.1;
    vec4 otherObjCol = vec4(0);
    float otherObjDist = trace(rayorg + raydir * (tmin + tminOffset), raydir, otherObjCol, 0.0);
    if(otherObjDist <= 0.){
        otherObjCol = vec4(0);
        otherObjDist = tmax + 1.;
    }else{
        otherObjDist += tmin + tminOffset;
    }

    bool hitOtherObj = false;

	while((i < samples)
          && (min(min(col.x, col.y), col.z) < 1.0)
          && (col.w < 0.999)
          && t <= tmax) {

		vec3 p = rayorg + t * raydir; // [-0.5*volscale, 0.5*volscale]^3 + u_offset
		vec3 texpos = (p - u_offset) / volumescale + 0.5; // [0, 1]^3
		vec4 temp = samplingVolume(texpos);

        if (abs(temp.x) < 0.00001 &&
            abs(temp.y) < 0.00001 &&
            abs(temp.z) < 0.00001 &&
            abs(temp.w) < 0.00001) {
            // Skip
        } else if (temp.x < tf_min) {
            // Skip
        } else if (temp.x > tf_max) {
            // Skip
        } else {
            float f = clamp(temp.x, tf_min, tf_max);
            float x = (f - tf_min) / (tf_max - tf_min); // normalize
            vec4 tfCol = densityToColor(x, rayorg, p, texpos);
            tfCol.w *= min(min(volumedim.x, volumedim.y), volumedim.z) / u_samples; //u_volumeDensity;

            tfCol.rgb *= tfCol.a;
            col = (1. - col.a) * tfCol + col;
        }

		t += tstep;
		i = i + 1.0;

        if(t > otherObjDist && hitOtherObj == false) {
            otherObjCol.rgb *= otherObjCol.a;
            col = (1. - col.a) * otherObjCol + col;
            hitOtherObj = true;
        }
	}

    if(hitOtherObj == false) {
        otherObjCol.rgb *= otherObjCol.a;
        col = (1. - col.a) * otherObjCol + col;
    }

	col.w = min(1.0, col.w);
    //    col.rgb = pow(col.rgb, vec3(1.0));
	gl_FragColor = col;
}
