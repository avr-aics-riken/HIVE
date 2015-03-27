#ifdef LSGL_ES
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
const float num_steps = 525.0;
//const float num_steps = 200.0;
uniform vec3 volumescale;
uniform vec3 volumedim;
const float volumedim_cust = 90.0;
//const float volumedim_cust = 8.0;
uniform vec3 offset;

// Simple Isosurf
uniform float isoval;


#define SASAKI_EPS

vec3 getN(vec3 p)
{
#ifdef SASAKI_EPS
	vec3 eps = vec3(0.5)/volumedim_cust;
#else 
	vec3 eps = vec3(0.5)/volumedim;
#endif
	return -normalize(vec3(
		texture3D(tex0,p+vec3(eps.x,0,0)).x-texture3D(tex0,p-vec3(eps.x,0,0)).x,
		texture3D(tex0,p+vec3(0,eps.y,0)).x-texture3D(tex0,p-vec3(0,eps.y,0)).x,
		texture3D(tex0,p+vec3(0,0,eps.z)).x-texture3D(tex0,p-vec3(0,0,eps.z)).x
	));
}

vec3 lap(vec3 p)
{
#ifdef SASAKI_EPS
	vec3 eps = vec3(0.5)/volumedim_cust;
#else 
	vec3 eps = vec3(0.5)/volumedim;
#endif
	return (vec3(
		texture3D(tex0,p+vec3(eps.x,0,0)).x-texture3D(tex0,p-vec3(eps.x,0,0)).x,
		texture3D(tex0,p+vec3(0,eps.y,0)).x-texture3D(tex0,p-vec3(0,eps.y,0)).x,
		texture3D(tex0,p+vec3(0,0,eps.z)).x-texture3D(tex0,p-vec3(0,0,eps.z)).x
	));
}

vec4 samplingVolume(vec3 texpos)
{
	vec4 col = texture3D(tex0, texpos);
	float p  = texture3D(tex0, texpos).x;
	float l = length(col.xyz);
	vec4 sum = vec4(0.0);
	vec3 vn = lap(texpos);//*0.5+0.5;
	//if (vn.y > 0.05 && vn.x > 0.05 && abs(vn.z) > 0.1)
	//if (vn.y > 0.5 && vn.x > 0.5 && abs(vn.z) > 0.1)
	//if (vn.y > 0.5 && vn.x > 0.5 && abs(vn.z) > 0.5)
	//if (vn.y > 0.15 && vn.x > 0.15 && abs(vn.z) > 0.1)
	//if (vn.y > 0.1 && vn.x > 0.1 && abs(vn.z) > 0.1)
	if (vn.y > 0.05 && vn.x > 0.05 && abs(vn.z) > 0.05)
	//if (vn.y > 0.01 && vn.x > 0.01 && abs(vn.z) > 0.01)
	//if (vn.y > 0.8 && vn.x > 0.8 && abs(vn.z) > 0.8)
    {
		float rr = l * 0.5;
		//if(!(col.w >= 5.0))
		if(p > 2.0) {
			vec4 cx = mix( mix(vec4(1,0,0,1), vec4(0,1,0,1), min(1.0,rr/0.6)), vec4(0,0,1,1), min(1.0,max(0.0,rr-0.6)/0.6));
			sum = cx;
			//return sum;

			//shading
			float dt1 = max(0.0, dot(vn, normalize(vec3(0.3,0.5,1.0))));
			float dt2 = max(0.0, dot(vn, normalize(vec3(1.0,0.3,-0.5))));
			float dt3 = max(0.0, dot(vn, normalize(vec3(0.1,1.0,0.1))));
			sum = vec4(vec3(0.8,0.8,0.8)*dt1*dt1
				+ vec3(0.5,0.5,0.5)*dt2*dt2
				+ vec3(0.4,0.4,0.4)*dt3*dt3, 1.0);
		}
	}
	return sum;
}

//vec4 samplingVolume2(vec3 texpos) {
//	vec4  col = texture3D(tex0, texpos);
//	return vec4(col.x);
//}

vec4 samplingVolume2(vec3 texpos) {
	float u    = texture3D(tex0, texpos).x;
	float p    = texture3D(tex0, texpos).y;
	float phi  = texture3D(tex0, texpos).z;
	float psi  = texture3D(tex0, texpos).w;
	return vec4(u, p, phi, psi);
}

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
	vec3 p;
    vec3 n;
    vec3 dir;
    isectinfo(p, n, dir);

    vec3 rayorg = eye;
	vec3 raydir = p - eye;
    raydir = normalize(raydir);
    vec4 sum   = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 sum_k = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 pmin = vec3(-0.5, -0.5, -0.5) * volumescale + offset;
    vec3 pmax = vec3( 0.5,  0.5,  0.5) * volumescale + offset;
    float tmin, tmax;
    IntersectP(rayorg, raydir, pmin, pmax, tmin, tmax);
	tmin = max(0.0, tmin);
	tmax = max(0.0, tmax);
	
    // raymarch.
    float t = tmin;
    float tstep = (tmax - tmin) / num_steps;
    float cnt = 0.0;
    float subcnt = 0.0;
	float state = 0.0;
	float press  = 0.0;
	float kotai  = 0.0;
	float ekitai = 0.0;
    while (cnt < float(num_steps)) {
		vec3 p = rayorg + t * raydir; // [-0.5*volscale, 0.5*volscale]^3 + offset
		vec3 texpos = (p - offset) / volumescale + 0.5; // [0, 1]^3
		vec4 temp = samplingVolume2(texpos);
		float tu  = temp.x;
		float tp  = temp.y;
		float phi = temp.z;
		float psi = temp.w;
		press += tu;

/*
		//KOTAI
		if(psi >= 0.0 && state > 1.5) {
			sum_k += vec4(1.0);
			cnt_k += 1.0;
		} else {
			//EKITAI
			if(psi < 0.0 && phi >= 0.0) {
				sum += vec4(1.0);
				state = 2.0;
				t += tstep * 0.2;
			}
		}
*/

		//EKITAI
		if(psi < 0.0 && phi >= 0.0) {
			sum += samplingVolume(texpos);
			ekitai += 0.003;
			state = 2.0;
		}

		//KOTAI
		if(psi >= 0.0 && state > 1.5) {
			if(state < 4.5) {
				state += 0.02;
			} else {
				state = 4.0;
			}
			if(state < 5.0) {
				//if(sum.x < 250.0)
				{
					//sum += vec4(1.2);
					sum += samplingVolume(texpos);
					kotai += 0.1;
				}
			}
		}

		//KITAI (tabun nai.)
		if(psi < 0.0 && phi < 0.0) {
			//sum += vec4(7.0);
			state = 6.0;
		}

		/////////////////////////////////////////////////////////////////////////
		//if(sum.a > 1.0) break;

        t += tstep;
		cnt += 1.0;
    }
	sum = sum / cnt;
	//press /= (cnt * 2.3);
	
	sum.a   = min(1.0, sum.a);
	gl_FragColor = vec4(sum.x + kotai, sum.y, sum.z, sum.a);
	//gl_FragColor = vec4(press * kotai, press, press + ekitai, press);
}

#else

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2 resolution;
uniform mat4 view;
varying float matID;
varying vec3 normal;
varying vec3 vnormal;
varying vec3 eyedir;

vec3 degamma(vec3 col)
{
	return pow(col,vec3(2.2,2.2,2.2));
}
vec3 gamma(vec3 col)
{
	return pow(col,vec3(1.0/2.2,1.0/2.2,1.0/2.2));
}

void main(void)
{
	gl_FragColor = vec4(0.8,0.8,1.0, 0.4);
}

#endif
