#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
#endif

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D colormap;
uniform vec2  resolution;
varying float matID;
varying vec3  mnormal;
varying vec3  vnormal;
uniform vec4  color;

void main(void)
{
	float u = gl_FragCoord.x / resolution.x;
	float v = 0.5 / resolution.y;
	gl_FragColor = texture2D(colormap, vec2(u, v));
	return;
}

