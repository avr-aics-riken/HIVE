#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2  resolution;
varying float matID;
varying vec3  mnormal;
uniform vec4  color;
uniform float power;

void main(void)
{
	gl_FragColor = vec4(1.0,1.0,1.0,1.0);
	return;
}

