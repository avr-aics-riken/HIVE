#extension GL_LSGL_trace : enable
#extension GL_OES_texture_3D : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler3D tex0;
uniform vec2      resolution;
uniform vec3      eye;
uniform vec3      lookat;
uniform vec3      up;
uniform vec3      eyedir;
const float fov = 45.0;
const float num_steps = 256.0;
uniform vec3 volumescale;
uniform vec3 volumedim;
uniform vec3 offset;

uniform sampler2D trans;


const float kOpacityThreshold = 0.99;
const float kBrightness = 1.0;
const float kDensity = 0.05;

// Simple transfer function
vec4 density_to_color(float dens)
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

vec4 samplingVolume(vec3 texpos, vec4 sum)
{
	vec4 dens = texture3D(tex0, texpos);
	vec4 col = texture2D(trans, vec2(dens.x, 0.5));//density_to_color(0.4 * dens.x);
	
	col.w *= kDensity;
	
	// front-to-back blending.
	col *= (1.0f - sum.a);
	
	return col; // add to sum
}

//-----------------------------------------------------------------------------------------

int inside(vec3 p,vec3 pmin, vec3 pmax) {
    if ((p.x >= pmin.x) && (p.x < pmax.x) &&
        (p.y >= pmin.y) && (p.y < pmax.y) &&
        (p.z >= pmin.z) && (p.z < pmax.z)) {
        return 1;
    }
    return 0;
}

int
IntersectP(vec3 rayorg, vec3 raydir, vec3 pMin, vec3 pMax, out float hit0, out float hit1) {
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

	int dpt;
	raydepth(dpt);
	if (dpt > 15){
		gl_FragColor = vec4(0,0,0,0);
		return;
	}

	vec3 p;
    vec3 n;
    vec3 dir;
    isectinfo(p, n, dir);

    vec3 rayorg = eye;
	vec3 raydir = p - eye;
    raydir = normalize(raydir);
    vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 pmin = vec3(-0.5, -0.5, -0.5)*volumescale + offset;
    vec3 pmax = vec3( 0.5,  0.5,  0.5)*volumescale + offset;
    float tmin, tmax;
    IntersectP(rayorg, raydir, pmin, pmax, tmin, tmax);
	tmin = max(0.0, tmin);
	tmax = max(0.0, tmax);
	
    // raymarch.
    float t = tmin;
    float tstep = (tmax - tmin) / num_steps;
    float cnt = 0.0;
    while (cnt < float(num_steps)) {
		vec4 acol;
		float hit = 0.0;
		int pds;
		rayoption(pds, 0);
		hit = trace(rayorg+t*raydir, tstep*raydir, acol, 0.0);
		if (hit > 0.0 && hit < 1.0) {
			sum += acol;
			break;
		}
		
		vec3 p = rayorg + t * raydir; // [-0.5*volscale, 0.5*volscale]^3 + offset
		vec3 texpos = (p - offset) / volumescale + 0.5; // [0, 1]^3
		sum += samplingVolume(texpos, sum);
		if (sum.a > kOpacityThreshold)
			break;
		
        t += tstep;
        cnt += 1.0;
    }
	
	sum *= kBrightness;
	
	sum.a = min(1.0, sum.a); // alpha clamp
	
	float ntmin = 0.0;
	vec4 ncol = vec4(0,0,0,0);
	// another primitve
	if (sum.a < 1.0)
		ntmin = trace(rayorg + (tmax+0.001)*raydir, raydir, ncol, 0.0);
	if (ntmin < 0.0)
		gl_FragColor = sum;
	else
		gl_FragColor = sum + (1.0-sum.a)*ncol;
}
