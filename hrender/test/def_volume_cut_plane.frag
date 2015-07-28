#extension GL_LSGL_trace : enable
#extension GL_OES_texture_3D : enable

#ifdef GL_ES
precision mediump float;
#endif

#define M_PI (3.14159265)

#define MAX_STEP_COUNT  (8192)

uniform sampler2D tf_tex;

uniform sampler3D tex0;
uniform vec2      resolution;
uniform vec3      eye;
uniform vec3      lookat;
uniform vec3      up;
uniform vec3      eyedir;
const float fov = 45.0;
uniform vec3 volumescale;
uniform vec3 volumedim;
uniform vec3 offset;

uniform float tf_min;
uniform float tf_max;
uniform float tf_opacity;
uniform vec4 cut_plane;
uniform float enable_cut_plane;
//uniform float ray_step; // raymarch step size

float ray_step = 0.01;
float maxValue = 120.0;

//
// -- contour --
//
uniform vec4 contour_param; // (min, max, step, border_weight)

#define CONTOUR_ENABLE_ANTI_ALIASING

float f(float x, float y) {
    return max(-1.0, 1.0 - sqrt(x * x + y * y));
}

vec3 hue(float hue)
{
    vec3 rgb = fract(hue + vec3(0.0, 2.0 / 3.0, 1.0 / 3.0));

    rgb = abs(rgb * 2.0 - 1.0);

    return clamp(rgb * 3.0 - 1.0, 0.0, 1.0);
}

vec3 to_rgb(float h, float s, float v)
{
    return ((hue(h) - 1.0) * s + 1.0) * v;
}

vec3 get_color(float x, float max_grad) {
    float ratio = (x - contour_param.x) / (contour_param.y - contour_param.x);
    float stepped = mod(ratio, contour_param.z / (contour_param.y - contour_param.x));
    ratio -= stepped;

    vec3 color = to_rgb(ratio, 1., 1.);

    float border = 1.;

    stepped /= contour_param.z / (contour_param.y - contour_param.x);

    float weight = contour_param.w / max_grad;
#ifdef CONTOUR_ENABLE_ANTI_ALIASING
    if (stepped < weight) {
        border = stepped / weight;
    } else if (stepped > 1. - weight) {
        border = 1. - (stepped - 1. + weight) / weight;
    }
#else
    if (stepped < weight || stepped > 1. - weight) {
        border = 0.;
    }
#endif

    return color * border;
}

//
// ---------------------
//



float RayPlaneIsect(vec3 rayOrg, vec3 rayDir, vec4 plane)
{
    float d = plane.w;
    float v = dot(rayDir, plane.xyz);

    if (abs(v) < 1.0e-30) return -1.0;

    float t = -((dot(rayOrg, plane.xyz) + d) / v);

    return t;
}

bool RayAABBIsect(
    out vec2    tRange,        // tmin, tmax
    float       rayMinT,
    float       rayMaxT,
    vec3        bmin,
    vec3        bmax,
    vec3        rayOrg,
    vec3        rayInvDir,
    ivec3       rayDirSign)
{

    // NOTE: rayInvDir must be less than < +-inf.
    //float tmin, tmax;

    vec3 od = rayOrg * rayInvDir; // ray.org / dir

    float x0 = bmin.x * rayInvDir.x - od.x;
    float x1 = bmax.x * rayInvDir.x - od.x;

    float y0 = bmin.y * rayInvDir.y - od.y;
    float y1 = bmax.y * rayInvDir.y - od.y;

    float z0 = bmin.z * rayInvDir.z - od.z;
    float z1 = bmax.z * rayInvDir.z - od.z;

    float minX = min(x0, x1);
    float minY = min(y0, y1);
    float minZ = min(z0, z1);

    float maxX = max(x0, x1);
    float maxY = max(y0, y1);
    float maxZ = max(z0, z1);

    float tmin = max(rayMinT, max(minX, max(minY, minZ)));
    float tmax = min(rayMaxT, min(maxX, min(maxY, maxZ)));

    // Write to tRange anyway

    tRange = vec2(tmin, tmax);
    //
    // Hit include (tmin == tmax) edge case(hit 2D plane).
    //
    return (tmin <= tmax);
}

vec4 Raymarch(vec3 rayOrg, vec3 rayDir, vec3 volBMin, vec3 volBMax)
{
    // ensure ray dir is normalized
    rayDir = normalize(rayDir);

    vec3 rayInvDir;
    rayInvDir = 1.0 / rayDir;
    //rayInvDir.x = (abs(rayDir.x) > 1.0e-6) ? 1.0 / rayDir.x : 0.0;
    //rayInvDir.y = (abs(rayDir.y) > 1.0e-6) ? 1.0 / rayDir.y : 0.0;
    //rayInvDir.z = (abs(rayDir.z) > 1.0e-6) ? 1.0 / rayDir.z : 0.0;

    ivec3 rayDirSign;
    rayDirSign.x = (rayDir.x < 0.0) ? 1 : 0;
    rayDirSign.y = (rayDir.y < 0.0) ? 1 : 0;
    rayDirSign.z = (rayDir.z < 0.0) ? 1 : 0;

    vec2 tRange;
    float minT = 0.0;
    float maxT = 1.0e+30; // @fixme

    if (!RayAABBIsect(tRange, minT, maxT, volBMin, volBMax, rayOrg, rayInvDir, rayDirSign)) {
        return vec4(0);
    }

    float rayT0 = max(0.0, tRange[0]);
    float rayT1 = tRange[1];

    float rayStep = ray_step;

    rayStep = max(1.0e-6, rayStep);

    float t = rayT0;
    float lengthT = rayT1 - rayT0;
    float stepT = rayStep;

    vec4 sum = vec4(0.0);
    int i = 0;

    float cutPlaneT = -1.0;

    if (enable_cut_plane > 0.0) {
        cutPlaneT = RayPlaneIsect(rayOrg, rayDir, cut_plane);
        //t = cutPlaneT;
    }

    if (cutPlaneT > rayT0) {
        // advance ray's entry point to cut plane.
        t = cutPlaneT;
    }

    for (i = 0; i < MAX_STEP_COUNT; i++) {
        if (t > rayT1) {
            break;
        }

        vec3 p = rayOrg + t * rayDir;

        if ((enable_cut_plane > 0.0) && (dot(cut_plane.xyz, p) + cut_plane.w < -0.0001)) {
            continue;
        }

        t += stepT;

        // local P;
        vec3 texCoord = (p - volBMin) / (volBMax - volBMin); 

        vec4 rgba = texture3D(tex0, texCoord);

        float x = max(0.0, (rgba.x - tf_min) / (tf_max - tf_min));
        vec4 tfCol = texture2D(tf_tex, vec2(x, 0));
        tfCol *= tf_opacity;

        // Highlight cut plane
        if ((enable_cut_plane > 0.0) && (i < 1) && (dot(cut_plane.xyz, p) <= -(cut_plane.w-0.001))) {
            //tfCol.x += 0.25; //tfCol.y *= 2.0;
            //tfCol.y += 0.25; //tfCol.y *= 2.0;
            //tfCol.z += 0.25; //tfCol.y *= 2.0;
            //tfCol.w += 0.005; //tfCol.y *= 2.0;
            tfCol.rgb *= 50.0;
            tfCol.w = 0.5; //tfCol.y *= 2.0;

            //tfCol.rgb = get_color(rgba.x, 1.0);
            //return vec4(tfCol.rgb, 1.0);

            sum += tfCol;

        } else {

            if (rgba.x > maxValue) {
                continue;
            }

            sum += (1.0 - sum.w) * tfCol;
        }

        if (sum.w >= 1.0) {
            break;
        }

        t += stepT;
        
    }

    return sum;

}

void  main(void) {


    vec3 p;
    vec3 n;
    vec3 dir;
    isectinfo(p, n, dir);

    vec3 rayorg = eye;
    vec3 raydir = p - eye;
    raydir = normalize(raydir);
    vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 pmin = vec3(-0.5, -0.5, -0.5)*volumescale + offset;
    vec3 pmax = vec3( 0.5,  0.5,  0.5)*volumescale + offset;

    vec4 col = Raymarch(rayorg, raydir, pmin, pmax);

    gl_FragColor = col;
}
	
