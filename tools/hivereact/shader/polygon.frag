#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
#endif

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D tex0;
uniform vec2  resolution;
varying float matID;
varying vec3  normal;
varying vec3  vnormal;
uniform vec4  color;
varying vec3  eyedir;
uniform mat4  lsgl_WorldInverseTranspose;

void main(void)
{
#ifdef LSGL_ES
    vec3 p,n,dir;
    isectinfo(p, n, dir);
    dir = vec3(0,1,0);
#else
    vec3 dir = vec3(0.0,1.0,0.0);
    vec3 n = vnormal;
#endif
    vec3 N = n;
    if (length(normal) < 0.001) {
        N = normalize(n);
    } else {
        N = normalize(normal);
    }

    //N = (lsgl_WorldInverseTranspose * vec4(N, 0.0)).xyz;
    vec3 light  = normalize(dir);
    float dt = max(0.0, dot(N, light));
    dt = dt * 0.8 + 0.2;
    gl_FragColor = color * vec4(dt,dt,dt,1.0);
    return;
}
