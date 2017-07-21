#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2  resolution;
varying vec2  texcoord;

varying float matID;
varying vec3  mnormal;
uniform vec4  color;

void main(void)
{
	gl_FragColor = texture2D(tex0, texcoord);
	return;
}

