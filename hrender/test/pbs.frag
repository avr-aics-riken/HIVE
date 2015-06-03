//
// PBR shader for HIVE.
//
// Features
//   * Diffuse, diffuse indirect
//   * IBL map
//   * fresnel
//   * glossy reflection with GGX microfacet kernel.
//   * glossy refraction with GGX microfacet kernel.
//   * Energy conservation among diffuse/reflection/refraction factor.
//   * Quad area light.
//
// TODO
//   * [ ] MIS sampling for area light
//   * [x] Consider emission.
//   * [ ] Sampling optimization.
//     * [ ] Russian roulette
//   * [ ] Multiple area lights.
//   * [ ] Legacy light(directional, spot, point)
//

#extension GL_LSGL_trace : enable
#extension GL_LSGL_random : enable

#ifdef GL_ES
precision mediump float;
#endif

const float PIVAL = 3.14159265358979323846;
const float tFar = 1e6; // must be less than the distance to the envmap sphere.

uniform vec3 lightdir;
uniform sampler2D mytex0;
uniform float matid;

// PBS parameter
uniform sampler2D pbs_envtex;
uniform sampler2D pbs_mattex;       // material info as a texture
uniform sampler2D pbs_lighttex;     // light info as a texture
uniform float pbs_num_materials;
uniform float pbs_num_lights;

//uniform float numIteration;     // # of sample iteration.

varying vec3 mnormal;

float sqr(float x) { return x*x; }

struct Material {
    vec3  emission;
    vec3  diffuse;
    float fresnel;      // 0: off, 1: on
    vec3  reflection;
    float reflectionGlossiness;
    vec3  refraction;
    float refractionGlossiness;
    float ior;
};

struct Light {
    vec3  color;
    float intensityMultiplier;
    vec3  position;
    vec3  uDir;    // quad light
    vec3  vDir;    // quad light
    vec3  dir;
    float radius;   // sphere light
    int   type;     // 0: quad, 1: sphere
    float shadowBias;
    int   invisible;
    int   nodecay;
    int   doubleSided;
};

float MIS(float invpdf, float otherpdf) {
    // power heuristic is: a^2 / (a^2+b^2)
    // simplifying: 1 / (1 + (b/a)^2)
    return 1.0 / (1.0 + otherpdf * otherpdf * invpdf * invpdf);
}

vec2 XYZtoUV(vec3 n)
{
    const float PIVAL = 3.141592;

    // dir to theta,phi. Y up to Z up
    float theta = acos(n.y);
    float phi = 0.0;
    if (n.z == 0.0) {
    } else {
        phi = atan(n.x, -n.z);
    }

    // 0.99999 = Prevent texture warp around.
    vec2 coord = vec2(phi / (2.0 * PIVAL), clamp(1.0 - theta / PIVAL, 0.0, 0.999999));

    return coord;
}

vec3
RandomVectorHemisphereCosWeight(float r0, float r1, vec3 N, vec3 Tn, vec3 Bn)
{
    float r = sqrt( r0 );
    float t = 2.0*PIVAL*r1;
    float rCosT = r * cos(t);
    float rSinT = r * sin(t);

    float w = sqrt(max(0.0, 1.0 - rCosT*rCosT - rSinT*rSinT ));

    return normalize( rCosT * Tn + rSinT * Bn + w * N );
}


// Assume quad light with uniform intensity.
vec3 SampleAreaLight(vec3 center, vec3 u_dir, vec3 v_dir)
{
    float r0, r1;
    random(r0);
    random(r1);

    return center + (r0 - 0.5) * u_dir + (r1 - 0.5) * v_dir;
}


vec3 EvalAreaLight(Light light, vec3 P, vec3 N, vec3 U, vec3 B, vec3 kdRGB)
{
    vec3 Lo = vec3(0.0);

    SampleAreaLight(light.position, light.uDir, light.vDir);

    vec3 Lpos = SampleAreaLight(light.position, light.uDir, light.vDir);
    vec3 Ldir = normalize(light.dir);

    vec3 LP = Lpos - (P + light.shadowBias * N);
    float Ldist = max(length(LP), 0.0);
    vec3 L = normalize(LP);

    float lightCos = abs(dot(Ldir, L));
    float lightArea = length(cross(light.uDir, light.vDir));

    int frontfaced = 1;
    if (light.doubleSided == 0) {
        if (dot(L, light.dir) > 0.0) {
            frontfaced = 0;
        }
    }

    float G = 1.0;
    if (light.nodecay < 1) {
        // Apply attenuation;  
        G = Ldist * Ldist;
        if (G > 1e-6) {
            G = 1.0 / G;
        }
    }

    if (frontfaced > 0) {

        // Light sampling.
        float hitExplicit = trace(P+light.shadowBias*N, L);
        float visibility = 1.0;
        // hit distance returned by trace() is shorter by shadow bias, thus compensate the distance check 
        // taking into account shadow bias.
        if (hitExplicit < (Ldist - light.shadowBias)) {
            visibility = 0.0;
        }

        if (visibility > 0.0) {
            float weightLight = (1.0 / PIVAL) * lightArea * dot(N, L) * lightCos * G;

            // BRDF sampling(fixme).
            float randu, randv;
            random(randu);
            random(randv);
            vec3 wi = RandomVectorHemisphereCosWeight(randu, randv, N, U, B);
            float weightBRDF = (1.0 / PIVAL);

            // @todo { MIS }
            Lo = (1.0/PIVAL) * kdRGB * lightArea * G * lightCos * dot(L, N) * light.color * light.intensityMultiplier;
        }
    }

    return Lo;
}

// Find an intersection with area light(quad)
// Assume raydir is normalized.
float IntersectAreaLight(vec3 rayorg, vec3 raydir, vec3 center, vec3 u_dir, vec3 v_dir)
{
    vec3 Nn = normalize(cross(u_dir, v_dir));
    float d = -dot(center, Nn);
    if (abs(dot(raydir, Nn)) < 0.00001) {
        // ray is parallel to quad.
        return -1.0;
    }

    float t = -(dot(rayorg, Nn) + d) / dot(raydir, Nn);
    if (t < 0.0) {
        return -1.0;
    }

    // Ensure hit point is inside of quad.
    vec3 p = rayorg + t * raydir;
    vec3 po = p - center;

    if (abs(dot(po, 0.5 * u_dir)) > dot(0.5 * u_dir, 0.5 * u_dir)) {
        t = -1.0;
    }

    if (abs(dot(po, 0.5 * v_dir)) > dot(0.5 * v_dir, 0.5 * v_dir)) {
        t = -1.0;
    }

    return t;
}

float IntersectSphere(vec3 raydir, vec3 rayorg, vec3 spherepos, float sphereradius)
{
    vec3 oc = rayorg - spherepos;
    float b = 2.0 * dot(raydir, oc);
    float c = dot(oc, oc) - sphereradius*sphereradius;
    float disc = b * b - 4.0 * c;
 
    if (disc < 0.0)
        return -1.0;
 
    float q;
    if (b < 0.0)
        q = (-b - sqrt(disc))/2.0;
    else
        q = (-b + sqrt(disc))/2.0;
 
    float t0 = q;
    float t1 = c / q;
 
    if (t0 > t1) {
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }
 
    if (t1 < 0.0)
        return -1.0;
 
    if (t0 < 0.0) {
        return t1;
    } else {
        return t0; 
    }
}

// Analytic sphere light
// https://imdoingitwrong.wordpress.com/2011/01/31/light-attenuation/
vec3 DirectIllumination(vec3 P, vec3 N, vec3 lightCenter, float lightRadius, vec3 lightColour, float cutoff)
{
    // calculate normalized light vector and distance to sphere light surface
    float r = lightRadius;
    vec3 L = lightCenter - P;
    float distance = length(L);
    float d = max(distance - r, 0);
    L /= distance;
     
    // calculate basic attenuation
    float denom = d/r + 1.0;
    float attenuation = 1.0 / (denom*denom);
     
    // scale and bias attenuation such that:
    //   attenuation == 0 at extent of max influence
    //   attenuation == 1 when d == 0
    attenuation = (attenuation - cutoff) / (1.0 - cutoff);
    attenuation = max(attenuation, 0.0);
     
    float LdotN = max(dot(L, N), 0.0);
    return lightColour * LdotN * attenuation;
}

//
// --------- GGX microfacet ----------------
// Based on SPI's OSL and Blender cycles.
// See 
//
// - https://developer.blender.org/D572
//
// For details and copyrights.
//

float fresnel_dielectric(
        float eta, const vec3 N,
        vec3 I, out vec3 R, out vec3 T,
        out int is_inside)
{
	float cosval = dot(N, I), neta;
	vec3 Nn;

	// check which side of the surface we are on
	if(cosval > 0.0) {
		// we are on the outside of the surface, going in
		neta = 1.0 / eta;
		Nn   = N;
		is_inside = 0;
	}
	else {
		// we are inside the surface
		cosval  = -cosval;
		neta = eta;
		Nn   = -N;
		is_inside = 1;
	}
	
	// compute reflection
	R = (2.0 * cosval)* Nn - I;
	
	float arg = 1.0 -(neta * neta *(1.0 -(cosval * cosval)));
	if(arg < 0.0) {
		T = vec3(0.0, 0.0, 0.0);
		return 1.0; // total internal reflection
	}
	else {
		float dnp = sqrt(arg);
		float nK = (neta * cosval)- dnp;
		T = -(neta * I)+(nK * Nn);
		// compute Fresnel terms
		float cosTheta1 = cosval; // N.R
		float cosTheta2 = -dot(Nn, T);
		float pPara = (cosTheta1 - eta * cosTheta2)/(cosTheta1 + eta * cosTheta2);
		float pPerp = (eta * cosTheta1 - cosTheta2)/(eta * cosTheta1 + cosTheta2);
		return 0.5 * (pPara * pPara + pPerp * pPerp);
	}
}

// Reference for the importance sampling of GGX kernel: https://developer.blender.org/D572
float approx_erff(float z)
{
    float s = 1.0f;

    if(z < 0.0f) {
        s = -1.0f;
        z = -z;
    }

    float result;

    if(z < 0.5) {
        if(z < 1e-10) {
            if(z == 0.0) {
                result = 0.0;
            }
            else {
                float c = 0.0033791670;
                result = z * 1.125 + z * c;
            }
        }
        else {
            float Y = 1.044948577;

            float zz = z * z;
            float num = (((-0.007727583 * zz) + -0.050999073)*zz + -0.338165134)*zz + 0.083430589;
            float denom = (((0.000370900 * zz) + 0.008585719)*zz + 0.087522260)*zz + 0.455004033;
            result = z * (Y + num / denom);
        }
    }
    else if(z < 2.5) {
        if(z < 1.5) {
            float Y = 0.4059357643;
            float fz = z - 0.5;

            float num = (((0.088890036 * fz) + 0.191003695)*fz + 0.178114665)*fz + -0.098090592;
            float denom = (((0.123850974 * fz) + 0.578052804)*fz + 1.426280048)*fz + 1.847590709;

            result = Y + num / denom;
            result *= exp(-z * z) / z;
        }
        else  {
            float Y = 0.506728172;
            float fz = z - 1.5;
            float num = (((0.017567943 * fz) + 0.043948189)*fz + 0.038654037)*fz + -0.024350047;
            float denom = (((0.325732924 * fz) + 0.982403709)*fz + 1.539914949)*fz + 1.0;

            result = Y + num / denom;
            result *= exp(-z * z) / z;
        }

        result = 1.0 - result;
    }
    else {
        result = 1.0;
    }

    return s * result;
}

float approx_erfinvf_impl(float p, float q)
{
    float result = 0.0;

    if(p <= 0.5) {
        float Y = 0.089131474;
        float g = p * (p + 10.0);
        float num = (((-0.012692614 * p) + 0.033480662)*p + -0.008368748)*p + -0.000508781;
        float denom = (((1.562215583 * p) + -1.565745582)*p + -0.970005043)*p + 1.0;
        float r = num / denom;
        result = g * Y + g * r;
    }
    else if(q >= 0.25) {
        float Y = 2.249481201;
        float g = sqrt(-2.0 * log(q));
        float xs = q - 0.25;
        float num = (((17.644729840 * xs) + 8.370503283)*xs + 0.105264680)*xs + -0.202433508;
        float denom = (((-28.660818049 * xs) + 3.971343795)*xs + 6.242641248)*xs + 1.0;
        float r = num / denom;
        result = g / (Y + r);
    }
    else {
        float x = sqrt(-log(q));

        if(x < 3.0) {
            float Y = 0.807220458;
            float xs = x - 1.125;
            float num = (((0.387079738 * xs) + 0.117030156)*xs + -0.163794047)*xs + -0.131102781;
            float denom = (((4.778465929 * xs) + 5.381683457)*xs + 3.466254072)*xs + 1.0;
            float R = num / denom;
            result = Y * x + R * x;
        }
        else {
            float Y = 0.939955711;
            float xs = x - 3.0;
            float num = (((0.009508047 * xs) + 0.018557330)*xs + -0.002224265)*xs + -0.035035378;
            float denom = (((0.220091105 * xs) + 0.762059164)*xs + 1.365334981)*xs + 1.0;
            float R = num / denom;
            result = Y * x + R * x;
        }
    }

    return result;
}

float approx_erfinvf(float z)
{
   float p, q, s;

   if(z < 0.0) {
      p = -z;
      q = 1.0 - p;
      s = -1.0;
   }
   else {
      p = z;
      q = 1.0 - z;
      s = 1.0;
   }

   return s * approx_erfinvf_impl(p, q);
}

void microfacet_ggx_sample_slopes(
    float cos_theta_i, float sin_theta_i,
    float alpha_x, float alpha_y,
    float randu, float randv, out float slope_x, out float slope_y, out float G1i)
{
    /* special case (normal incidence) */
    if(cos_theta_i >= 0.99999) {
        float r = sqrt(max(0.0, randu/(1.0 - randu)));
        float phi = 2.0 * PIVAL * randv;
        slope_x = r * cos(phi);
        slope_y = r * sin(phi);
        G1i = 1.0;

        return;
    }

    /* precomputations */
    float tan_theta_i = sin_theta_i/cos_theta_i;
    float G1_inv = 0.5 * (1.0 + sqrt(max(0.0, 1.0 + tan_theta_i*tan_theta_i)));

    G1i = 1.0/G1_inv;

    /* sample slope_x */
    float A = 2.0*randu*G1_inv - 1.0;
    float AA = A*A;
    float tmp = 1.0/(AA - 1.0);
    float B = tan_theta_i;
    float BB = B*B;
    float D = sqrt(BB*(tmp*tmp) - (AA - BB)*tmp);
    float slope_x_1 = B*tmp - D;
    float slope_x_2 = B*tmp + D;
    slope_x = (A < 0.0 || slope_x_2*tan_theta_i > 1.0)? slope_x_1: slope_x_2;

    /* sample slope_y */
    float S;

    if(randv > 0.5) {
        S = 1.0;
        randv = 2.0*(randv - 0.5);
    }
    else {
        S = -1.0;
        randv = 2.0*(0.5 - randv);
    }

    float z = (randv*(randv*(randv*0.27385 - 0.73369) + 0.46341)) / (randv*(randv*(randv*0.093073 + 0.309420) - 1.000000) + 0.597999);
    slope_y = S * z * sqrt(max(0.0, 1.0 + slope_x*slope_x));
}

// GGX only
vec3 microfacet_sample_stretched(vec3 omega_i,
    float alpha_x, float alpha_y,
    float randu, float randv, out float G1i)
{
    /* 1. stretch omega_i */
    vec3 omega_i_ = vec3(alpha_x * omega_i.x, alpha_y * omega_i.y, omega_i.z);
    omega_i_ = normalize(omega_i_);

    /* get polar coordinates of omega_i_ */
    float costheta_ = 1.0;
    float sintheta_ = 0.0;
    float cosphi_ = 1.0;
    float sinphi_ = 0.0;

    if(omega_i_.z < 0.99999) {
        costheta_ = omega_i_.z;
        sintheta_ = sqrt(max(0.0, 1.0 - costheta_*costheta_));

        float invlen = 1.0/sintheta_;
        cosphi_ = omega_i_.x * invlen;
        sinphi_ = omega_i_.y * invlen;
    }

    /* 2. sample P22_{omega_i}(x_slope, y_slope, 1, 1) */
    float slope_x, slope_y;

    microfacet_ggx_sample_slopes(costheta_, sintheta_,
        alpha_x, alpha_y, randu, randv, slope_x, slope_y, G1i);

    /* 3. rotate */
    float tmp = cosphi_*slope_x - sinphi_*slope_y;
    slope_y = sinphi_*slope_x + cosphi_*slope_y;
    slope_x = tmp;

    /* 4. unstretch */
    slope_x = alpha_x * slope_x;
    slope_y = alpha_y * slope_y;

    /* 5. compute normal */
    return normalize(vec3(-slope_x, -slope_y, 1.0));
} 

vec3 MicrofacetGGXEvalReflect(vec3 N, vec3 X, vec3 Y, vec3 I, float alpha_x, float alpha_y, vec3 omega_in, out float pdf)
{
	if(max(alpha_x, alpha_y) <= 1e-4f)
		return vec3(0, 0, 0);

	float cosNO = dot(N, I);
	float cosNI = dot(N, omega_in);

	if(cosNI > 0.0 && cosNO > 0.0) {
		/* get half vector */
		vec3 m = normalize(omega_in + I);
		float alpha2 = alpha_x * alpha_y;
		float D, G1o, G1i;

		if(alpha_x == alpha_y) {
			/* isotropic
			 * eq. 20: (F*G*D)/(4*in*on)
			 * eq. 33: first we calculate D(m) */
			float cosThetaM = dot(N, m);
			float cosThetaM2 = cosThetaM * cosThetaM;
			float cosThetaM4 = cosThetaM2 * cosThetaM2;
			float tanThetaM2 = (1.0 - cosThetaM2) / cosThetaM2;
			D = alpha2 / (PIVAL * cosThetaM4 * (alpha2 + tanThetaM2) * (alpha2 + tanThetaM2));

			/* eq. 34: now calculate G1(i,m) and G1(o,m) */
			G1o = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alpha2 * (1.0 - cosNO * cosNO) / (cosNO * cosNO))));
			G1i = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alpha2 * (1.0 - cosNI * cosNI) / (cosNI * cosNI)))); 
		}
		else {
			/* anisotropic */
			vec3 Z = N;

			/* distribution */
			vec3 local_m = vec3(dot(X, m), dot(Y, m), dot(Z, m));
			float slope_x = -local_m.x/(local_m.z*alpha_x);
			float slope_y = -local_m.y/(local_m.z*alpha_y);
			float slope_len = 1.0 + slope_x*slope_x + slope_y*slope_y;

			float cosThetaM = local_m.z;
			float cosThetaM2 = cosThetaM * cosThetaM;
			float cosThetaM4 = cosThetaM2 * cosThetaM2;

			D = 1.0 / ((slope_len * slope_len) * PIVAL * alpha2 * cosThetaM4);

			/* G1(i,m) and G1(o,m) */
			float tanThetaO2 = (1.0 - cosNO * cosNO) / (cosNO * cosNO);
			float cosPhiO = dot(I, X);
			float sinPhiO = dot(I, Y);

			float alphaO2 = (cosPhiO*cosPhiO)*(alpha_x*alpha_x) + (sinPhiO*sinPhiO)*(alpha_y*alpha_y);
			alphaO2 /= cosPhiO*cosPhiO + sinPhiO*sinPhiO;

			G1o = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alphaO2 * tanThetaO2)));

			float tanThetaI2 = (1.0 - cosNI * cosNI) / (cosNI * cosNI);
			float cosPhiI = dot(omega_in, X);
			float sinPhiI = dot(omega_in, Y);

			float alphaI2 = (cosPhiI*cosPhiI)*(alpha_x*alpha_x) + (sinPhiI*sinPhiI)*(alpha_y*alpha_y);
			alphaI2 /= cosPhiI*cosPhiI + sinPhiI*sinPhiI;

			G1i = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alphaI2 * tanThetaI2)));
		}

		float G = G1o * G1i;

		/* eq. 20 */
		float common = D * 0.25f / cosNO;
		float outval = G * common;

		/* eq. 2 in distribution of visible normals sampling
		 * pm = Dw = G1o * dot(m, I) * D / dot(N, I); */

		/* eq. 38 - but see also:
		 * eq. 17 in http://www.graphics.cornell.edu/~bjw/wardnotes.pdf
		 * pdf = pm * 0.25 / dot(m, I); */
		pdf = G1o * common;

		return vec3(outval, outval, outval);
	}

	return vec3(0, 0, 0);
}

vec3 MicrofacetGGXEvalTransmit(vec3 N, vec3 I, float alpha_x, float alpha_y, float eta, vec3 omega_in, out float pdf)
{
	if(max(alpha_x, alpha_y) <= 1e-4)
		return vec3(0, 0, 0);

	float cosNO = dot(N, I);
	float cosNI = dot(N, omega_in);

	if(cosNO <= 0.0 || cosNI >= 0.0)
		return vec3(0, 0, 0); /* vectors on same side -- not possible */

	/* compute half-vector of the refraction (eq. 16) */
	vec3 ht = -(eta * omega_in + I);
	vec3 Ht = normalize(ht);
	float cosHO = dot(Ht, I);
	float cosHI = dot(Ht, omega_in);

	/* those situations makes chi+ terms in eq. 33, 34 be zero */
	if(dot(Ht, N) <= 0.0f || cosHO * cosNO <= 0.0f || cosHI * cosNI <= 0.0f)
		return vec3(0.0f, 0.0f, 0.0f);

	float D, G1o, G1i;

	/* eq. 33: first we calculate D(m) with m=Ht: */
	float alpha2 = alpha_x * alpha_y;
	float cosThetaM = dot(N, Ht);
	float cosThetaM2 = cosThetaM * cosThetaM;
	float tanThetaM2 = (1.0 - cosThetaM2) / cosThetaM2;
	float cosThetaM4 = cosThetaM2 * cosThetaM2;
	D = alpha2 / (PIVAL * cosThetaM4 * (alpha2 + tanThetaM2) * (alpha2 + tanThetaM2));

	/* eq. 34: now calculate G1(i,m) and G1(o,m) */
	G1o = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alpha2 * (1.0 - cosNO * cosNO) / (cosNO * cosNO))));
	G1i = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alpha2 * (1.0 - cosNI * cosNI) / (cosNI * cosNI)))); 

	float G = G1o * G1i;

	/* probability */
	float Ht2 = dot(ht, ht);

	/* eq. 2 in distribution of visible normals sampling
	 * pm = Dw = G1o * dot(m, I) * D / dot(N, I); */

	/* out = fabsf(cosHI * cosHO) * (eta * eta) * G * D / (cosNO * Ht2)
	 * pdf = pm * (eta * eta) * fabsf(cosHI) / Ht2 */
	float common = D * (eta * eta) / (cosNO * Ht2);
	float outval = G * abs(cosHI * cosHO) * common;
	pdf = G1o * cosHO * abs(cosHI) * common;

	return vec3(outval, outval, outval);
}

// NOTE: output `omega_in` vector might not be normalized.
int MicrofacetGGXSample(vec3 N, vec3 Ng, vec3 X, vec3 Y, vec3 I, float randu, float randv, float alpha_x, float alpha_y, float eta, int refractive, out vec3 eval, out vec3 omega_in, out float pdf)
{
	float cosNO = dot(N, I);
    if (cosNO > 0.0) {
		vec3 Z = N;

		/* importance sampling with distribution of visible normals. vectors are
		 * transformed to local space before and after */
		vec3 local_I = vec3(dot(X, I), dot(Y, I), cosNO);
		vec3 local_m;
		float G1o;

		local_m = microfacet_sample_stretched(local_I, alpha_x, alpha_y, randu, randv, G1o);

		vec3 m = X*local_m.x + Y*local_m.y + Z*local_m.z;
		float cosThetaM = local_m.z;

		/* reflection or refraction? */
		if(refractive == 0) { // reflection
			float cosMO = dot(m, I);

			if(cosMO > 0.0) {
				/* eq. 39 - compute actual reflected direction */
				omega_in = 2.0 * cosMO * m - I;

				if(dot(Ng, omega_in) > 0.0) {
					if(max(alpha_x, alpha_y) <= 1e-4) {
						/* some high number for MIS */
						pdf = 1.0e6;
						eval = vec3(1.0e6, 1.0e6, 1.0e6);
					}
					else {
						/* microfacet normal is visible to this ray */
						/* eq. 33 */
						float alpha2 = alpha_x * alpha_y;
						float D, G1i;

						if(alpha_x == alpha_y) {
							/* isotropic */
							float cosThetaM2 = cosThetaM * cosThetaM;
							float cosThetaM4 = cosThetaM2 * cosThetaM2;
							float tanThetaM2 = 1.0/(cosThetaM2) - 1.0;
							D = alpha2 / (PIVAL * cosThetaM4 * (alpha2 + tanThetaM2) * (alpha2 + tanThetaM2));

							/* eval BRDF*cosNI */
							float cosNI = dot(N, omega_in);

							/* eq. 34: now calculate G1(i,m) */
							G1i = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alpha2 * (1.0 - cosNI * cosNI) / (cosNI * cosNI)))); 
						}
						else {
							/* anisotropic distribution */
							vec3 local_m = vec3(dot(X, m), dot(Y, m), dot(Z, m));
							float slope_x = -local_m.x/(local_m.z*alpha_x);
							float slope_y = -local_m.y/(local_m.z*alpha_y);
							float slope_len = 1.0 + slope_x*slope_x + slope_y*slope_y;

							float cosThetaM = local_m.z;
							float cosThetaM2 = cosThetaM * cosThetaM;
							float cosThetaM4 = cosThetaM2 * cosThetaM2;

							D = 1.0 / ((slope_len * slope_len) * PIVAL * alpha2 * cosThetaM4);

							/* calculate G1(i,m) */
							float cosNI = dot(N, omega_in);

							float tanThetaI2 = (1.0 - cosNI * cosNI) / (cosNI * cosNI);
							float cosPhiI = dot(omega_in, X);
							float sinPhiI = dot(omega_in, Y);

							float alphaI2 = (cosPhiI*cosPhiI)*(alpha_x*alpha_x) + (sinPhiI*sinPhiI)*(alpha_y*alpha_y);
							alphaI2 /= cosPhiI*cosPhiI + sinPhiI*sinPhiI;

							G1i = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alphaI2 * tanThetaI2)));
						}

						/* see eval function for derivation */
						float common = (G1o * D) * 0.25 / cosNO;
						float outval = G1i * common;
						pdf = common;

						eval = vec3(outval, outval, outval);
					}

				}
			}
		}
		else {
			/* CAUTION: the i and o variables are inverted relative to the paper
			 * eq. 39 - compute actual refractive direction */
			vec3 R, T;
			float fresnel;
			int inside = 0;

			fresnel = fresnel_dielectric(eta, m, I, R, T, inside);

			if((inside < 1) && (fresnel < 1.0)) { // (fresnel < 1.0 => no TIR)

				omega_in = T;

				if(max(alpha_x, alpha_y) <= 1e-4 || abs(eta - 1.0) < 1e-4) {
					/* some high number for MIS */
					pdf = 1e6f;
					eval = vec3(1e6f, 1e6f, 1e6f);
				}
				else {
					/* eq. 33 */
					float alpha2 = alpha_x * alpha_y;
					float cosThetaM2 = cosThetaM * cosThetaM;
					float cosThetaM4 = cosThetaM2 * cosThetaM2;
					float tanThetaM2 = 1.0/(cosThetaM2) - 1.0;
					float D = alpha2 / (PIVAL * cosThetaM4 * (alpha2 + tanThetaM2) * (alpha2 + tanThetaM2));

					/* eval BRDF*cosNI */
					float cosNI = dot(N, omega_in);

					/* eq. 34: now calculate G1(i,m) */
					float G1i = 2.0 / (1.0 + sqrt(max(0.0, 1.0 + alpha2 * (1.0 - cosNI * cosNI) / (cosNI * cosNI)))); 

					/* eq. 21 */
					float cosHI = dot(m, omega_in);
					float cosHO = dot(m, I);
					float Ht2 = eta * cosHI + cosHO;
					Ht2 *= Ht2;

					/* see eval function for derivation */
					float common = (G1o * D) * (eta * eta) / (cosNO * Ht2);
					float outval = G1i * abs(cosHI * cosHO) * common;
					pdf = cosHO * abs(cosHI) * common;

					eval = vec3(outval, outval, outval);
				}
			}
		}
        return 1; // valid
	}

    return 0; // invalid
}

// Simplified GGX
float GGXKernel(float alpha, float cosThetaM)
{
    float CosSquared = cosThetaM*cosThetaM;
    float TanSquared = (1.0-CosSquared)/CosSquared;
    return (1.0/PIVAL) * sqr(alpha/(CosSquared * (alpha*alpha + TanSquared)));
}

vec3 BRDFMicrofacet( vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y, float alpha )
{
    vec3 H = normalize( L + V );
    float D = GGXKernel(alpha, dot(N,H));
    return vec3(D);
}

//
// --------------------------
//

// Get material information from a texture.
// Assume filtering mode of `pbs_mattex` is GL_NEAREST.
void GetMaterial(int i, out Material mat)
{
    const float tex_width = 8.0; // texture width = 8;
    //float y = (pbs_num_materials - 1.0 - float(i)) / pbs_num_materials;
    float y = float(i) / pbs_num_materials;

    vec4 diffuse = texture2D(pbs_mattex, vec2(0.5 / tex_width, y));
    vec4 reflection_and_glossiness = texture2D(pbs_mattex, vec2(1.5 / tex_width, y));
    vec4 refraction_and_glossiness = texture2D(pbs_mattex, vec2(2.5 / tex_width, y));
    vec4 fresnel_and_ior = texture2D(pbs_mattex, vec2(3.5 / tex_width, y));
    vec4 emission = texture2D(pbs_mattex, vec2(4.5 / tex_width, y));

    mat.diffuse = diffuse.rgb;
    mat.reflection = reflection_and_glossiness.rgb;
    mat.reflectionGlossiness = reflection_and_glossiness.w;
    mat.refraction = refraction_and_glossiness.rgb;
    mat.refractionGlossiness = refraction_and_glossiness.w;
    mat.ior = fresnel_and_ior.y;
    mat.fresnel = fresnel_and_ior.x;
    mat.emission = emission.rgb;
}

// Get light information from a texture.
// Assume filtering mode of `pbs_lighttex` is GL_NEAREST.
void GetLight(int i, out Light lt)
{
    const float tex_width = 8.0; // texture width = 8;
    float y = float(i) / pbs_num_lights;

    // [0:3]   3 floats: light color, 1 float: intensityMultiplier
    // [4:7]   3 floats: light center, 1 float: light type(0: quad, 1: sphere))
    // [8:11]  3 floats: light u dir(quad) or 1 float: light radius
    // [12:15] 3 floats: light v dir(quad)
    // [16:19] shadow bias(1 float), invisible(1 float), no decay(1 float), double sided(1 float)
    // [20:31] reserved.

    vec4 color_and_intensity = texture2D(pbs_lighttex, vec2(0.5 / tex_width, y));
    vec4 center_and_type = texture2D(pbs_lighttex, vec2(1.5 / tex_width, y));
    vec4 u_dir_or_radius = texture2D(pbs_lighttex, vec2(2.5 / tex_width, y));
    vec4 v_dir = texture2D(pbs_lighttex, vec2(3.5 / tex_width, y));
    vec4 lightattribs = texture2D(pbs_lighttex, vec2(4.5 / tex_width, y));

    lt.color = color_and_intensity.rgb;
    lt.intensityMultiplier = color_and_intensity.w;
    lt.position = center_and_type.xyz;
    lt.uDir = u_dir_or_radius.xyz;
    lt.vDir = v_dir.xyz;
    lt.dir = cross(lt.uDir, lt.vDir);
    lt.type = int(center_and_type.w);
    lt.radius = u_dir_or_radius.w;
    lt.shadowBias = lightattribs.x;
    lt.invisible = int(lightattribs.y);
    lt.nodecay = int(lightattribs.z);
    lt.doubleSided  = int(lightattribs.w);
}

// Assume RGB is in range [0, 1]
float RGB2Y(vec3 rgb)
{
    return 0.299 * rgb.x + 0.587 * rgb.y + 0.114 * rgb.z;
}


// I: towards shading point.
void ComputeFresnel(
    out vec3        refl,
    out vec3        refr,
    out float       kr,
    out float       kt,
    vec3            I,
    vec3            N,
    float           eta)
{
    float d = dot(I, N);

    refl = reflect(I, N);

    refr = refract(I, N, eta);
    if (abs(refr.x) < 0.0001 && abs(refr.y) < 0.0001 && abs(refr.z) < 0.0001) {
        // Total internal reflection.
        kr = 1.0;
        kt = 0.0;
        return;
    }

    float cos_r = dot(refl, N);
    float cos_t = -dot(refr, N);

    float rp = (cos_t - eta * cos_r) / (cos_t + eta * cos_r);
    float rs = (cos_r - eta * cos_t) / (cos_r + eta * cos_t);
    kr = (rp * rp + rs * rs) * 0.5;

    kr = clamp(kr, 0.0, 1.0);

    kt = 1.0f - kr;

}

// Shlick fresnel. Assume V and L are normalized.
float FresnelApprox(vec3 V, vec3 L, float ior) {
    // Assume n1 = air
    float n1 = 1.0;
    float R0 = (n1 - ior) / (n1 + ior);
    R0 = R0 * R0;

    vec3 H = normalize(V + L); //Halfway vector

    float c = clamp(1.0-max(dot(V, H), 0.0), 0.0, 1.0);
    float e = c * c * c * c * c;
    float f = mix(R0, 1.0, e);

    return f;
}

bool isNan(float val)
{
  return (val <= 0.0 || 0.0 <= val) ? false : true;
}

void main()
{
    int depth;
    raydepth(depth);

    if (depth > 9) {
        gl_FragColor = vec4(0.01, 0.01, 0.01, 1.0);
        return;
    }

    vec3 P;
    vec3 n;
    vec3 ng;
    vec3 tn;
    vec3 bn;
    vec3 dir;
    vec2 bary_uv;
    int nisect;
    numIntersects(nisect);
    queryIntersect(0, P, n, ng, tn, bn, dir, bary_uv);

    Material mat;
    GetMaterial(int(matid), mat);

    Light light;
    GetLight(0, light);

	vec3 N = normalize(mnormal);
    float Nnorm = length(N);
    if (isNan(Nnorm) || (Nnorm < 1e7)) { // mnormal attribute is not bound to this object.
        N = normalize(n);
    }
    vec3 Ng = normalize(ng);
	vec3 V = normalize(-dir); // dir: from eye(previous hit point) towards shading point.

    // Approximate tangent vector.
    vec3 U;
    if (abs(N[0]) > 0.001) {
        U = vec3(N[2], 0, -N[0]);
    } else {
        U = vec3(0, -N[2], N[1]);
    }
    U = normalize(U);
    vec3 B = normalize(cross(U, N));

    bool inside = false;

    vec3 I = V;
    // dot(N, I) must be positive.
    if (dot(N, V) < 0.0) {
        inside = true;
        I = -I;
    }

    //
    // Simple energy conservation model.
    //
    vec3 ksRGB0 = mat.reflection;
    vec3 ktRGB0 = mat.refraction;
    vec3 ksRGB  = ksRGB0;
    vec3 ktRGB  = clamp((1.0 - ksRGB0) * ktRGB0, 0.0, 1.0);
    vec3 kdRGB  = clamp((1.0 - ktRGB - ksRGB) * mat.diffuse, 0.0, 1.0);

    float ks = RGB2Y(ksRGB);
    float kt = RGB2Y(ktRGB);
    float kd = RGB2Y(kdRGB);

    vec3 Rvec, Tvec;
    float fresnelKr, fresnelKt;
    float eta = 1.0 / mat.ior;
    ComputeFresnel(Rvec, Tvec, fresnelKr, fresnelKt, -I, N, eta);

    if ((ks > 0.0 || kt > 0.0) && (mat.fresnel > 0.0)) {
        // Adjust reflectance with fresnel term.
        ksRGB = ksRGB0 * fresnelKr;
        ktRGB = ktRGB0 * fresnelKt;
        kdRGB = clamp((1.0 - ktRGB - ksRGB) * mat.diffuse, 0.0, 1.0);
        ks = RGB2Y(ksRGB);
        kt = RGB2Y(ktRGB);
        kd = RGB2Y(kdRGB);
    }

    vec3 Lo = vec3(0.0);

    //
    // Add self-emission term.
    //
    Lo += mat.emission;

    // @todo { Russian roulette. }

    if (ks > 0.0) {
        float randu; random(randu);
        float randv; random(randv);
        float alpha_x = mat.reflectionGlossiness;
        float alpha_y = alpha_x;
        int refractive = 0;
        vec3 throughput = vec3(0.0);
        vec3 wi;
        float pdf;
        int valid = MicrofacetGGXSample(N, Ng, U, B, V, randu, randv, alpha_x, alpha_y, eta, refractive, throughput, wi, pdf);
        //throughput = MicrofacetGGXEvalReflect(N, U, B, V, alpha_x, alpha_y, wi, pdf);

        if (valid > 0) {
            //vec4 reflcol = texture2D(pbs_envtex, XYZtoUV(wi));
            //Lo += ksRGB * reflcol.xyz;
#if 0
            // Implicit light
            float tLight = IntersectAreaLight(P + 0.001 * Ng, wi, lightCenter, lightU, lightV);
            if (tLight > 0.0) {
                // visilibity check
                float visilibity = 0.0;
                float t = trace(P + 0.001 * Ng, wi);
                if (t < 0.0 || (t > tLight)) {
                   visilibity = 1.0;
                }
                    
                Lo += ksRGB * visilibity * throughput * lightColor;
            }
#endif

            // trace reflect ray,
            vec4 traceCol = vec4(0.0);
            float t = trace(P + 0.001 * Ng, wi, traceCol, 0.0);
            if (t > 0.0) {
                Lo += ksRGB * traceCol.rgb;
            }
        }
    }

    if (kt > 0.0) {
        float randu; random(randu);
        float randv; random(randv);
        float alpha_x = mat.refractionGlossiness;
        float alpha_y = alpha_x;
        int refractive = 1;
        vec3 throughput = vec3(0.0);
        vec3 wi = vec3(0.0);
        float pdf = 0.0;

        // NOTE: eta is inverted for transmission event.
        eta = mat.ior;
        if (inside) {
            eta = 1.0/mat.ior;
        }
        int valid = MicrofacetGGXSample(N, Ng, U, B, I, randu, randv, alpha_x, alpha_y, eta, refractive, throughput, wi, pdf);
        //vec3 Ke = MicrofacetGGXEvalTransmit(N, V, alpha_x, alpha_y, eta, wi, pdf);

        if (inside) {
            wi = -wi;
        }

        if (valid > 0) {
            // trace refract ray
            vec4 reflCol = vec4(0.0);
            float t = trace(P + 0.01 * wi, wi, reflCol, 0.0);
            if (t > 0.0) {
                Lo += ktRGB * reflCol.xyz;
            }
        }
    }

    // Add direct lighting.
    if (kd > 0.0) {
        Lo += EvalAreaLight(light, P, N, U, B, kdRGB);

#if 0
        //
        // Add indirect
        //
        float randu; random(randu);
        float randv; random(randv);

        vec3 wi = RandomVectorHemisphereCosWeight(randu, randv, N, U, B);

        // Implicit light
        float tLight = IntersectAreaLight(P + 0.001 * Ng, wi, lightCenter, lightU, lightV);
        if (tLight > 0.0) {
            // visilibity check
            float visilibity = 0.0;
            float t = trace(P + 0.001 * Ng, wi);
            if (t < 0.0 || (t > tLight)) {
               visilibity = 1.0;
            }
                
            Lo += kdRGB * visilibity * lightColor;
        }

        vec4 traceCol = vec4(0.0);
        float t = trace(P + 0.001 * Ng, wi, traceCol, 0.0);
        Lo += kdRGB * traceCol.rgb;
#endif
    }

    if (depth == 0) {
        // apply gamma correction.
        Lo = pow(Lo, vec3(1.0/2.2));    
    }

    gl_FragColor = vec4(Lo, 1.0);
}
