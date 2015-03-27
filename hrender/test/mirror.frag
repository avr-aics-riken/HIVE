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
	vec3 N;
    if (length(mnormal) < 0.01) {
    	N = normalize(n);
    } else {
		N = normalize(mnormal);
   	}
	
	vec4 rcol;
	vec3 rdir = reflect(normalize(dir), N);
	float hit = trace(p + N * 0.001, rdir, rcol, 0.0);
	if (hit < 0.0) { // no hit
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		gl_FragColor = rcol;
	}
	return;
}

