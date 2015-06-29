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
	vec3 p;
    vec3 n;
    vec3 dir;
    isectinfo(p, n, dir);
    vec3 N;
    if (length(mnormal) < 0.001) {
    	N = normalize(n);
    } else {
		N = normalize(mnormal);
    }

	float dt = max(0.0, dot(N, normalize(vec3(-0.5,1,1))));
	dt = dt * 0.9 + 0.1;
	gl_FragColor = vec4(dt,dt,dt,1.0);
	return;
}
