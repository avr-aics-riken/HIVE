#extension GL_LSGL_trace : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2  resolution;
varying float matID;
varying vec3  mnormal;
uniform vec4  color;

void main(void)
{
	vec3 p,n,dir;
	isectinfo(p, n, dir);

    vec3 Nn = normalize(n);

	gl_FragColor = vec4(abs(Nn), 1.0);
	return;
}

