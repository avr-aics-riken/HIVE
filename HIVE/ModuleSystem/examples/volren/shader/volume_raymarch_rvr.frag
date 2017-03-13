#ifdef LSGL_ES
#extension GL_LSGL_trace : enable
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
uniform vec3 offset;

// Transfer function parameters
uniform sampler2D tf_tex;
uniform float     tf_min;
uniform float     tf_max;

#define EPS        0.005
#define SAMPLES    256.0
#define num_steps  SAMPLES

uniform float u_samples;
uniform float bias;
uniform float u_enableLighting;
uniform vec3 u_lightPos;
uniform float u_lightIntensity;
uniform float u_constAttenuation;
uniform float u_linearAttenuation;
uniform float u_quadraticAttenuation;
uniform vec3 u_ambient;
uniform vec3 u_specular;
uniform float u_volumeDensity;

// gradient map
uniform float u_enableGradientMap;
uniform sampler2D u_gradTex;

vec3 computeGradient(vec3 t){
    vec3 sample1, sample2;
    //vec3 ofst = bias * volumedim / 2.;
    //vec3 ofst = offset;
    //vec3 ofst = volumescale / volumedim;
    vec3 ofst = vec3(0.01);
    sample1.x = texture3D(tex0, t-vec3(ofst.x, 0.0, 0.0)).x;
	sample2.x = texture3D(tex0, t+vec3(ofst.x, 0.0, 0.0)).x;
	sample1.y = texture3D(tex0, t-vec3(0.0, ofst.y, 0.0)).x;
	sample2.y = texture3D(tex0, t+vec3(0.0, ofst.y, 0.0)).x;
	sample1.z = texture3D(tex0, t-vec3(0.0, 0.0, ofst.z)).x;
	sample2.z = texture3D(tex0, t+vec3(0.0, 0.0, ofst.z)).x;
	vec3 N	= sample1 - sample2;
    //vec4 N4 = modelview_matrix_inverse * vec4(N, 0);
    return N;
}

vec4 samplingVolume(vec3 texpos) {
	vec4 col = texture3D(tex0, texpos);
	return col;
}

int IntersectP(vec3 rayorg, vec3 raydir, vec3 pMin, vec3 pMax, out float hit0, out float hit1) {
    float t0 = -10000.0, t1 = 10000.0;
    hit0 = t0;
    hit1 = t1;

    vec3 tNear = (pMin - rayorg) / raydir;
    vec3 tFar  = (pMax - rayorg) / raydir;
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
        return 1;
    }
    return 0;
}

vec4 densityToColor( float d, vec3 shadingPos, vec3 texPos ){
    vec4 color = texture2D(tf_tex, vec2(d, 0));
    if(u_enableLighting == 0. && u_enableGradientMap == 0.)
        return color;
    float a = color.a;
    vec3 n = computeGradient(texPos);
    if(u_enableGradientMap != 0.){
        //float w = length(n) / 1.73205;
        //        vec4 gCol = texture2D(u_gradTex, vec2(w, 0));
        vec4 gCol = texture2D(u_gradTex, vec2(d, 0));
        a *= gCol.x;
        color.a = a;
        //color *= gCol.x;
        if(u_enableLighting == 0.)
            return color;
    }
    //    return vec4(abs(n), 1);
    if(length(n) == 0.0)
        return color;

    n = normalize(n);
    vec3 lightVec = u_lightPos - shadingPos;
    return vec4(abs(n), 1);
    float dist = length(lightVec);
    //return vec4(abs(n), 1);
    lightVec = normalize(lightVec);
    vec3 revLightVec = -lightVec;
    vec3 viewVec = normalize(-shadingPos);
    vec3 halfVec = normalize(lightVec + viewVec);
    vec3 halfVec2 = normalize(revLightVec + viewVec);
    float specular = pow(max(dot(n, halfVec), 0.), u_lightIntensity);
    float specular2 = pow(max(dot(n, halfVec2), 0.), u_lightIntensity);
    float attenuation = 1.0 / u_constAttenuation
        + u_linearAttenuation * dist
        + u_quadraticAttenuation * dist * dist;
    float diffuse = dot(lightVec, n) * 0.66;
    float diffuse2 = dot(revLightVec, n) * 0.33;
    
    color.rgb += color.rgb * diffuse
        + color.rgb * diffuse2
        + u_ambient
        + u_specular * specular * attenuation
        + u_specular * specular2 * attenuation;
    color.a = a;
    return color;
}

void  main(void) {
    int depth;
    raydepth(depth);

    if (depth > 10) {
        gl_FragColor = vec4(0.01, 0.01, 0.01, 1.0);
        return;
    }

	vec3 p;
    vec3 n;
    vec3 dir;
    isectinfo(p, n, dir);

    vec3 rayorg = eye;
	vec3 raydir = p - eye;
    raydir = normalize(raydir);
    vec4 sum   = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 sum_k = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 pmin = vec3(-0.5, -0.5, -0.5) * volumescale + offset;
    vec3 pmax = vec3( 0.5,  0.5,  0.5) * volumescale + offset;
    float tmin, tmax;
    IntersectP(rayorg, raydir, pmin, pmax, tmin, tmax);
	tmin = max(0.0, tmin);
	tmax = max(0.0, tmax);
	
    // raymarch.
    float t = tmin;
    float tstep = (tmax - tmin) / u_samples;
    float cnt = 0.0;
    float subcnt = 0.0;
	float state = 0.0;
	float press  = 0.0;
	float kotai  = 0.0;
	float ekitai = 0.0;
	float count = 0.0;
	float pu  = 0.0;
	float pp  = 0.0;
	float phi = 0.0;
	float psi = 0.0;
	
	vec3 sumN = vec3(0.0);
	vec4 col  = vec4(0.0, 0.0, 0.0, 0.0);

	float i = 0.0;
	while((i < u_samples)
          && (min(min(col.x, col.y), col.z) < 1.0)
          && (col.w < 0.999)) {
        vec4 acol;
		float hit = 0.0;
		int pds;
		rayoption(pds, 0);
		hit = trace(rayorg+t*raydir, tstep*raydir, acol, 0.0);
		if (hit > 0.0 && hit < 1.0) {
			col += acol;
			break;
		}
        
		vec3 p = rayorg + t * raydir; // [-0.5*volscale, 0.5*volscale]^3 + offset
		vec3 texpos = (p - offset) / volumescale + 0.5; // [0, 1]^3
		vec4 temp = samplingVolume(texpos);

        if (abs(temp.x) < 0.00001 && abs(temp.x) < 0.00001 && abs(temp.y) < 0.00001 && abs(temp.z) < 0.00001) {
            // Skip
        } else if (temp.x < tf_min) {
            // Skip
        } else if (temp.x > tf_max) {
            // Skip
        } else {
            float f = clamp(temp.x, tf_min, tf_max);
            float x = (f - tf_min) / (tf_max - tf_min); // normalize
            //density to color
            //            vec4 tfCol = texture2D(tf_tex, vec2(x, 0));
            vec4 tfCol = densityToColor(x, p, texpos);
            tfCol.w *= u_volumeDensity;
            tfCol *= (1.0 - col.w); //blend
            // tf_opacity == thickness
            //tfCol = tf_opacity * vec4(tfCol.rgb, 1.0);
            //col += (1.0 - col.w) * tfCol;
            col += tfCol;
        }

		count = count + 1.0;
		t += tstep;
		i = i + 1.0;
	}
	//col.xyz  /= count;
    //	col.w = 1.0;
	col.w = min(1.0, col.w);
	gl_FragColor = col;
}
