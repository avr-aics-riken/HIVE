#extension GL_LSGL_trace : enable
#extension GL_OES_texture_3D : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler3D tex0;
uniform vec2 resolution;
varying float matID;
varying vec3 mnormal;
uniform vec4 color;

uniform vec3 volumescale;
uniform vec3 volumedim;
uniform vec3 offset;
uniform vec3 eye;
uniform vec3 lookat;
uniform vec3 up;
uniform float width, height, depth;

void main(void)
{
	vec3 p;
	vec3 n;
	vec3 dir;
	isectinfo(p, n, dir);
	vec3 rayorg = eye;
	vec3 raydir = p - eye;
	vec3 scale = vec3(width, height, depth);
	vec3 texpos = (p - offset) / scale + 0.5; // [0, 1]^3
	vec4 dens = texture3D(tex0, texpos);
	gl_FragColor = vec4(normalize(dens.xyz), 1.0);
	return;
}