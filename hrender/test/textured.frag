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

void main(void)
{
	vec3 p,n,dir;
	isectinfo(p, n, dir);

	//gl_FragColor = vec4(texcoord, 1.0, 1.0);
	gl_FragColor = texture2D(mytex0, texcoord);
	return;
}

