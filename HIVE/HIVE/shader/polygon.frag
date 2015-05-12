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
	float dt = max(0.0, dot(normalize(mnormal), normalize(vec3(-0.5,1,1))));
	dt = dt * 0.9 + 0.1;
	gl_FragColor = vec4(dt,dt,dt,1.0);
	return;
}
