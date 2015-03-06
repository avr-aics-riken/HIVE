#extension GL_LSGL_trace : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2      resolution;
//uniform vec4      vval;
//varying vec2 texcoord;
varying float matID;
varying vec3 mnormal;

float frac(float x) { return mod(x,1.0); }
float noise(vec2 x, float t) { return frac(sin(dot(x,vec2(1.38984*sin(t),1.13233*cos(t))))*653758.5453); }

#define PI 3.1415926


void orthoBasis(out vec3 basis0,out vec3 basis1,out vec3 basis2, vec3 n)
{
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

vec3 degamma(vec3 col)
{
	return vec3(pow(col.r,2.2),pow(col.g,2.2),pow(col.b,2.2));
}
vec3 gamma(vec3 col)
{
	return vec3(pow(col.r,1.0/2.2),pow(col.g,1.0/2.2),pow(col.b,1.0/2.2));
}

vec3 randRay(vec3 nn, vec2 p, float r)
{
	float theta = sqrt(noise(p.xy,r*2.0*PI));
	float phi   = 2.0 * PI * noise(p.xy+vec2(0.3,0.2),r*2.0*PI+23.5);
	vec3 ref;
	
	ref.x = cos(phi) * theta;
	ref.y = sin(phi) * theta;
	ref.z = sqrt(1.0 - theta * theta);
	vec3 basis0 = vec3(1,0,0);
	vec3 basis1 = vec3(0,1,0);
	vec3 basis2 = vec3(0,0,1);
	orthoBasis(basis0,basis1,basis2, nn);
	vec3 rray;
	rray.x = ref.x * basis0.x + ref.y * basis1.x + ref.z * basis2.x;
	rray.y = ref.x * basis0.y + ref.y * basis1.y + ref.z * basis2.y;
	rray.z = ref.x * basis0.z + ref.y * basis1.z + ref.z * basis2.z;
	return rray;
}

void main(void) {
			
	int depth;
	raydepth(depth);
	if (depth > 3){
		gl_FragColor = vec4(0,0,0,0);
		return;
	}
	
	vec3 p;
    vec3 n;
    vec3 dir;
    isectinfo(p, n, dir);
	vec3 N = normalize(n);//normalize(mnormal);
	
	/*if (noise(p.xy,float(depth)*0.1) > 0.8){
		gl_FragColor = vec4(0,0,0,0);
		return;
	}*/
	
	vec4 matcol = vec4(0.8,0.8,0.8,1);
	if (matID<=0.5)
		matcol = vec4(0.8,0.8,0.8,1);
	else if (matID <= 1.5)
		matcol = vec4(0.8,0.0,0,1);
	else
		matcol = vec4(0.0,0.8,0.0,1);
		
	vec4 col=vec4(0,0,0,0);
	vec4 rcol;
	int maxsamp = 8;
	for (int i = 0; i < maxsamp; ++i)
	{
		vec3 rdir = randRay(n,p.xy,p.x+float(i)*0.01);
		float hit = trace(p+n*0.001, rdir, rcol, 0.0);
		if (hit < 0.0)
			col += matcol*vec4(1.0,1.0,1.0,1.0);
		else
			col += matcol*rcol;
	}
	col /= float(maxsamp);
	gl_FragColor = vec4(col.rgb,1.0);//vec4(1.0-hit,0,0,1);//col;
	return;
}
