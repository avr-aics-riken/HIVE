#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2  resolution;
varying float matID;
varying vec3  mnormal;
uniform vec4  color;
varying vec3 testvec3;

void main(void)
{
	gl_FragColor = vec4(testvec3, 1.0);
	return;
}

