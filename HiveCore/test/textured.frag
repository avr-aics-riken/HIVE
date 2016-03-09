#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
#endif

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D mytex0;
uniform vec2  resolution;
varying float matID;
varying vec3  mnormal;
varying vec3  vnormal;
varying vec2  texcoord;
uniform vec4  color;

void main(void)
{
#ifdef LSGL_ES
	vec3 p,n,dir;
	isectinfo(p, n, dir);

#else
	vec3 n = vnormal;
	if (length(n) < 0.001) {
		n = vec3(1.0,1.0,1.0);
	} 
#endif

	//gl_FragColor = vec4(texcoord, 1.0, 1.0);
	gl_FragColor = texture2D(mytex0, texcoord);
	return;
}

