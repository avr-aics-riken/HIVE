#extension GL_LSGL_trace : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mytex0;
uniform vec2  resolution;
varying float matID;
varying vec3  mnormal;
varying vec2  texcoord;
uniform vec4  color;


vec2 NormalToUV(vec3 normal)
{
	float PI = 3.1415926535;
	float lat = acos(-normal.y);
    float lon = atan(normal.z, normal.x);
    vec2 uv = vec2((lon / (2.0*PI)) + 0.5, lat / PI);
	return uv;
}

void main(void)
{
	vec3 p,n,dir;
	isectinfo(p, n, dir);

	float PI = 3.1415926535;

	vec3 N = normalize(n);
	vec2 uv = NormalToUV(N);

	vec4 col = texture2D(mytex0, uv);
	col.rgb = clamp(col.rgb, vec3(0.0), vec3(1.0));
	col.a = 1.0;
	gl_FragColor = col;
	return;
}

