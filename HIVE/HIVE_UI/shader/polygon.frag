#extension GL_LSGL_trace : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2  resolution;
varying float matID;
varying vec3  mnormal;
uniform vec4  color;
uniform mat4  lsgl_WorldInverseTranspose;

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

    N = (lsgl_WorldInverseTranspose * vec4(N, 0.0)).xyz;
    vec3 light  = normalize(-dir);
    float dt = max(0.0, dot(N, light));
    dt = dt * 0.9 + 0.1;
    gl_FragColor = color * vec4(dt,dt,dt,1.0);
    return;
}
