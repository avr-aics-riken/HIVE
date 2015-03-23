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

const float PIVAL = 3.141592;

void main(void)
{
	vec3 p,n,dir;
	isectinfo(p, n, dir);

    dir = normalize(dir);

    // dir to theta,phi. Y up to Z up
    float theta = acos(dir.y);
    float phi = 0.0;
    if (dir.z == 0.0) {
    } else {
        phi = atan(dir.x, -dir.z);
    }

    // 0.99999 = Prevent texture warp around.
    vec2 coord = vec2(phi / (2.0 * PIVAL), clamp(1.0 - theta / PIVAL, 0.0, 0.99999));

    vec4 c = texture2D(mytex0, coord);
	gl_FragColor = vec4(c.xyz, 1.0);

	return;
}

