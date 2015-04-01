#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
#extension GL_OES_texture_3D : enable
#endif

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler3D tex0;
uniform vec2 resolution;
uniform vec3 eye;
uniform vec3 lookat;
uniform vec3 up;
uniform vec3 eyedir;
uniform vec3 volumescale;
uniform vec3 volumedim;
uniform vec3 offset;

#define EPS        0.05
#define GAMMA      1.5
#define RAY_MULT   0.45
#define SAMPLES    500.0
#define num_steps  SAMPLES
#define T_MARGIN   15.0

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


vec2 rot(vec2 p, float a) {
	return vec2(
			p.x * cos(a) - p.y * sin(a),
			p.x * sin(a) + p.y * cos(a));
}

float r2d(float a) {
	return (a * 3.141592653) / 180.0;
}


float raytrace_cyl(vec3 org, vec3 dir, vec3 point, float radius) {
	vec3 temp = org - point;
	float a = 0.0;
	float b = 0.0;
	float c = 0.0;
	float d = 0.0;
	float t0 = -10000.0;
	float t1 = -10000.0;
	a = dot(dir.xy, dir.xy);
	b = 2.0 * dot(dir.xy, temp.xy);
	c = dot(temp.xy, temp.xy) - (radius * radius);
	d = b * b - 4.0 * a * c;
	if(d > 0.0) {
		t0 = (-b + d) / (2.0 * a);
		t1 = (-b - d) / (2.0 * a);
		if(t0 < t1) return t0;
		if(t0 > t1) return t1;
	}
	return -10000.0;
}

void main(void)
{
	vec3 p;
	vec3 n;
	vec3 dir;
	isectinfo(p, n, dir);

	//Create RAY
	vec3 rayorg = eye;
	vec3 raydir = p - eye;
	raydir = normalize(raydir);

	//Calc Bounding Box
	vec4 sum   = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 sum_k = vec4(0.0, 0.0, 0.0, 0.0);
	//vec3 pmin = vec3(-0.5, -0.5, -0.5) * volumescale + offset;
	//vec3 pmax = vec3( 0.5,  0.5,  0.5) * volumescale + offset;

	vec3 pmin = vec3(-0.5, -0.5, -0.5) * volumescale + offset;
	vec3 pmax = vec3( 0.5,  0.5,  0.5) * volumescale + offset;
	
#if _DEBUG_
	//gl_FragColor = vec4(offset.x, offset.y, offset.z, 1.0);
	float R = 0.0;
	float G = 0.0;
	float B = 0.0;
	if(volumescale.x >= 64.0)  R = 0.5;
	if(volumescale.x >= 128.0) R = 1.0;
	if(volumescale.y >= 64.0)  B = 0.5;
	if(volumescale.y >= 128.0) B = 1.0;

	gl_FragColor = vec4(R, G, B, 1.0);
	return ;
#endif //_DEBUG_

	//Create Sampling info
	float tmin, tmax;
	IntersectP(rayorg, raydir, pmin, pmax, tmin, tmax);
	tmin = max(0.0, tmin);
	tmax = max(0.0, tmax);
	float count = 0.0;
	float tstep = (tmax - tmin) / num_steps;

	//COLOR
	vec4 col = vec4(0.0);

	//culling for cylinder
	vec3   cylpos = vec3(0.0, 0.0, 0.0);
	float  cylrad = 40.0;

	float t = raytrace_cyl(rayorg, raydir, cylpos, cylrad);

	if(t > 0.0) {
#if _DEBUG_
		col.y = 1.0;
		col.w = 1.0;
		//gl_FragColor = vec4(offset.x, offset.y, offset.z, 1.0);
		//gl_FragColor = vec4(col);
		//return ;
#endif //_DEBUG_
	}

	//HIT(innner)
	if(t < 0.0)
	{
		float pu  = 0.0;
		float pp  = 0.0;
		float phi = 0.0;
		float psi = 0.0;
		t = tmin;
		t += 25.0 + sin(abs(raydir.x)) * 30.0;
		while(t < tmax) {
			vec3 ipos  = rayorg + raydir * t;
			vec3 texpos = (ipos - offset) / volumescale + 0.5;
			vec4 temp  = samplingVolume(texpos);
			pu  = temp.x;
			pp  = temp.y;
			phi = temp.z;
			psi = temp.w;
			//-----------------------------------------------------------
			//INFO   : 8 4 512 512 1024
			//MIN    : 0.000000 -5715.101563 -459.964142 -5.000000
			//MAX    : 5.246408 5533.366699 436.549988 5.000000
			//-----------------------------------------------------------
			if(psi < EPS && phi >= EPS) {
				if(pp > 10.0 && pp < 300.0) {
					if(pp > 10.0    && pp < 100.0) {
						col.z += pp * 0.2;
					} else if(pp > 100.0 && pp < 200.0) {
						col.y += pp * 0.3;
					} else {
						col.x += pp;
					}
				}
			}
			count = count + 1.0;
			t += tstep;

			if( length(raydir * t) > 185.0) break;
		}
		col.xyz   /= count;
		col.xyz = sqrt(col.xyz);
		col.xyz   /= GAMMA;
	}
	col.w = 1.0;
	gl_FragColor = vec4(col);
}

