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
uniform float     u_volumeMin;
uniform float     u_volumeMax;

uniform int u_sliceAxis; // 0 ... x, 1 ... y, 2 ... z
uniform float u_sliceRatio;
uniform int u_flipSlice;

uniform float u_contourStep;
uniform float u_eps;
uniform vec4 u_contourColor;

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

float sampleVolume(vec3 p) {
  return (texture3D(tex0, (p - u_offset) / volumescale + 0.5).x - u_volumeMin) / (u_volumeMax - u_volumeMin);
}

vec4 volumeColor(float v) {
  return texture2D(u_tfTexture, vec2(v, 0.5));
}

vec4 volumeColorPlane(float v) {
  return vec4(texture2D(u_tfTexture, vec2(v, 0.5)).rgb, 1.);
}

vec4 sampleVolumePlane(vec3 p, vec3 planeDirX, vec3 planeDirY,
                       vec2 rayDiff) {
  vec4 sum = vec4(0);
  vec3 orgP = p;
  float eps = 2. / resolution.x;

  const float sampleNum = 10.;
  for(float i = 0.; i < sampleNum ; i++) {
    float r1, r2;
    random(r1);
    random(r2);
    vec2 rand2 = eps * vec2(r1 - 0.5, r2 - 0.5);
    p = orgP + rand2.x * planeDirX + rand2.y * planeDirY;

    float volume = sampleVolume(p);
    //vec2 h = rayDiff;
    vec2 h = vec2(eps);
    vec2 grad =  vec2(sampleVolume(p + planeDirX * h.x) -
                      sampleVolume(p - planeDirX * h.x),
                      sampleVolume(p + planeDirY * h.y) -
                      sampleVolume(p - planeDirY * h.y)) / (2. * h);
    float de = (volume < u_contourStep * 0.01) ?
      1.:
      abs(mod(volume, u_contourStep)) / length(grad);
    float ee = 0.15;
    float t = smoothstep(u_eps, 2.0 * u_eps, de);
    sum +=  mix(vec4(u_contourColor.rgb, 1.0), volumeColorPlane(volume), t);
  }
  return sum / sampleNum;
}

void getSlicePlane(int sliceAxis, vec3 bboxMin, vec3 bboxMax,
                   out vec3 planeOrg, out vec3 planeNormal,
                   out vec3 planeDirX, out vec3 planeDirY) {
  if (sliceAxis == 0) {
    planeNormal = vec3(1, 0, 0);
    planeDirX = vec3(0, 1, 0);
    planeDirY = vec3(0, 0, 1);
  } else if (sliceAxis == 1) {
    planeNormal = vec3(0, 1, 0);
    planeDirX = vec3(0, 0, 1);
    planeDirY = vec3(1, 0, 0);
  } else {
    planeNormal = vec3(0, 0, 1);
    planeDirX = vec3(1, 0, 0);
    planeDirY = vec3(0, 1, 0);
  }

  if (u_flipSlice == 0) {
    planeNormal = -1. * planeNormal;
    planeOrg = bboxMax;
  } else {
    planeOrg = bboxMin;
  }
}

void computeBBox(out vec3 bboxMin, out vec3 bboxMax) {
  bboxMin = vec3(-0.5) * volumescale + u_offset;
  bboxMax = vec3(0.5) * volumescale + u_offset;
  if (u_sliceAxis == 0) {
    float bboxT = bboxMin.x + (bboxMax - bboxMin).x * u_sliceRatio;
    if(u_flipSlice == 1) {
      bboxMin.x = bboxT;
    }  else {
      bboxMax.x = bboxT;
    }
  } else if (u_sliceAxis == 1) {
    float bboxT = bboxMin.y + (bboxMax - bboxMin).y * u_sliceRatio;
    if(u_flipSlice == 1) {
      bboxMin.y = bboxT;
    }  else {
      bboxMax.y = bboxT;
    }
  } else {
    float bboxT = bboxMin.z + (bboxMax - bboxMin).z * u_sliceRatio;
    if(u_flipSlice == 1) {
      bboxMin.z = bboxT;
    }  else {
      bboxMax.z = bboxT;
    }
  }
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
  vec3 rayDiffX = (isectP + basisX / resolution.x) - eye;
  vec3 rayDiffY = (isectP + basisZ / resolution.x) - eye;

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

  vec3 planeNormal, planeOrg, planeDirX, planeDirY;
  getSlicePlane(u_sliceAxis, bboxMin, bboxMax,
                planeOrg, planeNormal, planeDirX, planeDirY);

  t0 *= 0.999999;
  vec3 isectPoint;
  hit = intersectPlane(planeNormal, planeOrg,
                       rayOrg, rayDir,
                       isectPoint, t0);
  if (hit && insideBBox(bboxMin, bboxMax, rayOrg + rayDir * (t0 * 1.0001))) {
    vec3 isectX, isectY;
    float tmp;
    hit = intersectPlane(planeNormal, planeOrg,
                         rayOrg, rayDiffX,
                         isectX, tmp);
    hit = intersectPlane(planeNormal, planeOrg,
                         rayOrg, rayDiffY,
                         isectY, tmp);
    vec2 diff = vec2(distance(isectPoint, isectX),
                     distance(isectPoint, isectY));
    gl_FragColor = vec4(sampleVolumePlane(isectPoint,
                                          planeDirX, planeDirY,
                                          diff).rgb,
                        1.);
    return;
  }

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
    volCol.a *= 1.;
    volCol.rgb *= volCol.a;
    l = (1. - l.a) * volCol + l;

    t += tStep;
    lastP = p;
  }

  if (insideBBox(bboxMin, bboxMax, isectPoint - rayDir * 0.0001)) {
    vec4 volCol = volumeColorPlane(sampleVolume(lastP));

    volCol.a *= 1.;
    volCol.rgb *= volCol.a;
    l = (1. - l.a) * volCol + l;
  }

  gl_FragColor = vec4(l.rgb, 1);
}
