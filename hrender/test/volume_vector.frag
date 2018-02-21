#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
#extension GL_LSGL_random : enable
#extension GL_OES_texture_3D : enable
#endif

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler3D tex0;
uniform vec2      resolution;
uniform vec3      eye;
//uniform vec3      lookat;
//uniform vec3      up;
//uniform vec3      eyedir;
uniform vec3 volumescale;
uniform vec3 volumedim;
uniform vec3 u_offset;

// Transfer function parameters
uniform sampler2D u_tfTexture;
uniform vec2 u_volumeMinMax; // [min, max]
uniform int u_colorMode;

uniform mat4 lsgl_WorldInverse; // inverse of object to world matrix

void orthoBasis(vec3 n,
                out vec3 basis0,out vec3 basis1,out vec3 basis2) {
  basis2 = vec3(n.x, n.y, n.z);
  basis1 = vec3(0.0, 0.0, 0.0);
  if ((n.x < 0.6) && (n.x > -0.6))
    basis1.x = 1.0;
  else if ((n.y < 0.6) && (n.y > -0.6))
    basis1.y = 1.0;
  else if ((n.z < 0.6) && (n.z > -0.6))
    basis1.z = 1.0;
  else
    basis1.x = 1.0;
  basis0 = cross(basis1, basis2);
  basis0 = normalize(basis0);
  basis1 = cross(basis2, basis0);
  basis1 = normalize(basis1);
}

const vec4 K = vec4(1.0, .666666, .333333, 3.0);
vec3 hsv2rgb(const vec3 c){
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// http://lolengine.net/blog/2013/07/27/rgb-to-hsv-in-glsl
vec3 rgb2hsv(vec3 c){
  vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
  vec4 p = c.g < c.b ? vec4(c.bg, K.wz) : vec4(c.gb, K.xy);
  vec4 q = c.r < p.x ? vec4(p.xyw, c.r) : vec4(c.r, p.yzx);

  float d = q.x - min(q.w, q.y);
  float e = 1.0e-10;
  return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

bool intersectBox(vec3 rayOrg, vec3 rayDir, vec3 boxMin, vec3 boxMax,
                  out float hit0, out float hit1) {
	float t0 = -10000.0, t1 = 10000.0;
    hit0 = t0;
    hit1 = t1;

    vec3 tNear = (boxMin - rayOrg) / rayDir;
    vec3 tFar  = (boxMax - rayOrg) / rayDir;

    if (tNear.x > tFar.x) {
        float tmp = tNear.x;
        tNear.x = tFar.x;
        tFar.x = tmp;
    }
    t0 = max(tNear.x, t0);
    t1 = min(tFar.x, t1);

    if (tNear.y > tFar.y) {
        float tmp = tNear.y;
        tNear.y = tFar.y;
        tFar.y = tmp;
    }
    t0 = max(tNear.y, t0);
    t1 = min(tFar.y, t1);

    if (tNear.z > tFar.z) {
        float tmp = tNear.z;
        tNear.z = tFar.z;
        tFar.z = tmp;
    }
    t0 = max(tNear.z, t0);
    t1 = min(tFar.z, t1);

    if (t0 <= t1) {
        hit0 = t0;
        hit1 = t1;
        return true;
    }
    return false;
}

bool insideBBox(vec3 bboxMin, vec3 bboxMax, vec3 p) {
    return bboxMin.x < p.x && p.x < bboxMax.x &&
        bboxMin.y < p.y && p.y < bboxMax.y &&
        bboxMin.z < p.z && p.z < bboxMax.z;
}

bool intersectPlane(const vec3 normal, const vec3 p,
                    const vec3 rayOrigin, const vec3 rayDir,
                    out vec3 isectPoint, inout float t0) {
    float d = -dot(p, normal);
    float v = dot(normal, rayDir);
    float t = -(dot(normal, rayOrigin) + d) / v;
    isectPoint = rayOrigin + t * rayDir;
    if(0.00001 < t && t0 < t && dot(normal, rayDir) > 0.){
        t0 = t;
        return true;
    }
    return false;
}

// Simple transfer function
vec4 density_to_color(float dens)
{
    if (dens < 0.01) {
        return vec4(0.0, 0.0, 0.0, 0.005);
    } else if (dens < 0.25) {
        float t = 4.0 * dens; // [0, 1]
        return vec4(t, 0.2*(1.0 - t), 0.0, .5);
    } else if (dens < 0.5) {
        float t = 4.0 * (dens - 0.25); // [0, 1]
        return vec4(0.0, t, 1.0-t,0.5);
    } else if (dens < 0.75) {
        float t = 4.0 * (dens - 0.5); // [0, 1]
        return vec4(1.0-t, 0.0, t,0.5);
    } else if (dens < 1.0) {
		float t = 4.0 * (dens - 0.75); // [0, 1]
        return vec4(0.4-0.4*t, 0.4-0.4*t, 0.2*t,0.5);
    } else if (dens < 10.0) {
		float t = 0.1 * (dens - 1.0); // [0, 1]
        return vec4(0.5*t, 0.0, 0.2-0.2*t,0.5);
	} else {
        return vec4(0.8, 0.0, 0.0,0.5);	
	}
}

vec4 sampleVolume(vec3 p) {
    return texture3D(tex0, (p - u_offset) / volumescale + 0.5);
}

float densityToAlpha(float v) {
    return v < 0.1 ? 0.01 : .3;
}

vec4 volumeColor(vec4 v) {
    vec3 nv = abs((v.rgb));
    if (u_colorMode == 0) {
        // vector xyz to rgb
        return vec4((v.rgb), densityToAlpha(length(v.rgb)));
    } else if (u_colorMode == 1) {
        return vec4(clamp(nv.r, 0.0, 1.0), 0, 0, clamp(nv.r, 0.0, 1.0));
    } else if (u_colorMode == 2) {
        return vec4(0, clamp(nv.g, 0.0, 1.0), 0, clamp(nv.g, 0.0, 1.0));
    } else if (u_colorMode == 3) {
        return vec4(0, 0, clamp(nv.b, 0.0, 1.0), clamp(nv.b, 0.0, 1.0));
    } else if (u_colorMode == 4) {
        //return density_to_color(length(v.rgb));
        return texture2D(u_tfTexture, vec2(length(v.rgb), 0.5));
    }
}


void computeBBox(out vec3 bboxMin, out vec3 bboxMax) {
  bboxMin = vec3(-0.5) * volumescale + u_offset;
  bboxMax = vec3(0.5) * volumescale + u_offset;
}

void main(void) {
    int depth;
    raydepth(depth);

    if (depth > 15) {
        gl_FragColor = vec4(0);
        return;
    }

	vec3 isectP, isectN, isectDir;
    isectinfo(isectP, isectN, isectDir);
    vec3 rayOrg = eye;
    vec3 rayDir = isectP - eye;
    rayDir = normalize(rayDir);

    vec3 basisX, basisY, basisZ;
    orthoBasis(isectN, basisX, basisY, basisZ);

    vec3 bboxMin, bboxMax;
    computeBBox(bboxMin, bboxMax);

    float t0, t1;
    bool hit = intersectBox(rayOrg, rayDir,
                            bboxMin, bboxMax,
                            t0, t1);
    if (!hit) {
        gl_FragColor = vec4(0, 0, 0, 1);
        return;
    }

    t0 *= 0.999999;
    vec3 isectPoint;

    vec4 l = vec4(0);

    const float MAX_SAMPLES = 150.;
    float t = t0;
    float tStep = (t1 - t0) / MAX_SAMPLES;

    vec3 p0 = rayOrg + t0 * rayDir;
    vec3 p1 = rayOrg + t1 * rayDir;
    vec3 distP = p0 - p1;
    float dist = abs(dot(vec3(1, 0, 0), distP));
    dist = max(dist, abs(dot(vec3(0, 1, 0), distP)));
    dist = max(dist, abs(dot(vec3(0, 0, 1), distP)));
    float sliceWidth = .05;
    float samples = floor(dist / sliceWidth) + 1.;
    samples = MAX_SAMPLES;
    vec3 lastP;
    t = t * 1.0001;
    for (float i = 0.; i < MAX_SAMPLES; i++){
        if((min(min(l.x, l.y), l.z)) > 1.0 ||
           l.w > 0.999 ||
           t >= t1 ||
           samples < i) break;

        vec3 p = rayOrg + t * rayDir;

        vec4 volCol = volumeColor(sampleVolume(p));
        volCol.rgb *= volCol.a;
        l = (1. - l.a) * volCol + l;

        t += tStep;
        lastP = p;
    }

    gl_FragColor = vec4(l.rgb, 1);
}
