#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
#endif

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2  resolution;
varying float matID;
varying vec3  mnormal;
varying vec3  vnormal;
uniform vec4  color;

void main(void)
{
#ifdef LSGL_ES
	vec3 p,n,dir;
	isectinfo(p, n, dir);

#else
	vec3 n = vnormal;
#endif
    vec3 Nn = normalize(n);

	gl_FragColor = vec4(abs(Nn), 1.0);
	return;
}

