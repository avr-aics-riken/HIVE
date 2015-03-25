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

	vec4 rcol;
	vec3 N = normalize(n);
	if (dot(-dir, N) < 0.0) { // Back face
		float hit = trace(p + N * 0.001, dir, rcol, 0.0);
		if (hit < 0.0) { // no hit
			gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		} else {
			gl_FragColor = rcol;
		}
		return;
	}

	vec3 rdir = refract(dir, N, 0.67);
	float hit = trace(p - N * 0.001, rdir, rcol, 0.0);
	if (hit < 0.0) { // no hit
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		gl_FragColor = rcol;
	}
	return;
}

