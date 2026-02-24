#pragma once

#include "material.h"
#include "pbr.h"
#include "rtweekend_defs.h"
#include "vecmath.h"
#include <limits>

/*
analytic

# Copyright Disney Enterprises, Inc.  All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License
# and the following modification to it: Section 6 Trademarks.
# deleted and replaced with:
#
# 6. Trademarks. This License does not grant permission to use the
# trade names, trademarks, service marks, or product names of the
# Licensor and its affiliates, except as required for reproducing
# the content of the NOTICE file.
#
# You may obtain a copy of the License at
# http://www.apache.org/licenses/LICENSE-2.0


# variables go here...
# [type] [name] [min val] [max val] [default val]
::begin parameters
color baseColor .82 .67 .16
float metallic 0 1 0
float subsurface 0 1 0
float specular 0 1 .5
float roughness 0 1 .5
float specularTint 0 1 0
float anisotropic 0 1 0
float sheen 0 1 0
float sheenTint 0 1 .5
float clearcoat 0 1 0
float clearcoatGloss 0 1 1
::end parameters


::begin shader

const float PI = 3.14159265358979323846;

float sqr(float x) { return x*x; }

float SchlickFresnel(float u)
{
    float m = clamp(1-u, 0, 1);
    float m2 = m*m;
    return m2*m2*m; // pow(m,5)
}

float GTR1(float NdotH, float a)
{
    if (a >= 1) return 1/PI;
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return (a2-1) / (PI*log(a2)*t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a*a;
    float t = 1 + (a2-1)*NdotH*NdotH;
    return a2 / (PI * t*t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
    return 1 / (PI * ax*ay * sqr( sqr(HdotX/ax) + sqr(HdotY/ay) + NdotH*NdotH ));
}

float smithG_GGX(float NdotV, float alphaG)
{
    float a = alphaG*alphaG;
    float b = NdotV*NdotV;
    return 1 / (NdotV + sqrt(a + b - a*b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
    return 1 / (NdotV + sqrt( sqr(VdotX*ax) + sqr(VdotY*ay) + sqr(NdotV) ));
}

vec3 mon2lin(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}


vec3 BRDF( vec3 L, vec3 V, vec3 N, vec3 X, vec3 Y )
{
    float NdotL = dot(N,L);
    float NdotV = dot(N,V);
    if (NdotL < 0 || NdotV < 0) return vec3(0);

    vec3 H = normalize(L+V);
    float NdotH = dot(N,H);
    float LdotH = dot(L,H);

    vec3 Cdlin = mon2lin(baseColor);
    float Cdlum = .3*Cdlin[0] + .6*Cdlin[1]  + .1*Cdlin[2]; // luminance approx.

    vec3 Ctint = Cdlum > 0 ? Cdlin/Cdlum : vec3(1); // normalize lum. to isolate hue+sat
    vec3 Cspec0 = mix(specular*.08*mix(vec3(1), Ctint, specularTint), Cdlin, metallic);
    vec3 Csheen = mix(vec3(1), Ctint, sheenTint);

    // Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
    // and mix in diffuse retro-reflection based on roughness
    float FL = SchlickFresnel(NdotL), FV = SchlickFresnel(NdotV);
    float Fd90 = 0.5 + 2 * LdotH*LdotH * roughness;
    float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

    // Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
    // 1.25 scale is used to (roughly) preserve albedo
    // Fss90 used to "flatten" retroreflection based on roughness
    float Fss90 = LdotH*LdotH*roughness;
    float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
    float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - .5) + .5);

    // specular
    float aspect = sqrt(1-anisotropic*.9);
    float ax = max(.001, sqr(roughness)/aspect);
    float ay = max(.001, sqr(roughness)*aspect);
    float Ds = GTR2_aniso(NdotH, dot(H, X), dot(H, Y), ax, ay);
    float FH = SchlickFresnel(LdotH);
    vec3 Fs = mix(Cspec0, vec3(1), FH);
    float Gs;
    Gs  = smithG_GGX_aniso(NdotL, dot(L, X), dot(L, Y), ax, ay);
    Gs *= smithG_GGX_aniso(NdotV, dot(V, X), dot(V, Y), ax, ay);

    // sheen
    vec3 Fsheen = FH * sheen * Csheen;

    // clearcoat (ior = 1.5 -> F0 = 0.04)
    float Dr = GTR1(NdotH, mix(.1,.001,clearcoatGloss));
    float Fr = mix(.04, 1.0, FH);
    float Gr = smithG_GGX(NdotL, .25) * smithG_GGX(NdotV, .25);

    return ((1/PI) * mix(Fd, ss, subsurface)*Cdlin + Fsheen)
        * (1-metallic)
        + Gs*Fs*Ds + .25*clearcoat*Gr*Fr*Dr;
}

::end shader
*/

class Principled : public Material
{
  public:
    bool scatter(const Ray &ray_in, const HitRecord &rec, Ray &scattered, Color &f_r, double &pdf) const override
    {
        Vec3 V = -ray_in.unit_direction();
        Vec3 N = rec.normal;
        double a = sqr(roughness);
        double a_clearcoat = lerp(.1, .001, clearcoat_gloss);
        double aspect = std::sqrt(1.0 - anisotropic * 0.9);
        double ax = std::max(.001, sqr(roughness) / aspect);
        double ay = std::max(.001, sqr(roughness) * aspect);
        Vec3 L, H;
        Vec3 b1, b2;
        Vec3::branchlessONB(N, b1, b2);

        // MIS
        double diffuse_prob = 1.0 / 3.0;
        double specular_prob = 1.0 / 3.0;
        double clearcoat_prob = 1.0 - diffuse_prob - specular_prob;
        double dummy_pdf;

        double r = random_double();
        double u = random_double();
        double v = random_double();

        if (r < diffuse_prob)
        {
            L = PBR::cosine_sample_hemisphere(N, u, v, dummy_pdf, b1, b2);
            H = unit_vector(V + L);
        }
        else if (r < diffuse_prob + specular_prob)
        {
            H = PBR::sample_GTR2_aniso(N, u, v, dummy_pdf, ax, ay, b1, b2);
            L = reflect(-V, H);
        }
        else
        {
            H = PBR::sample_GTR1(N, u, v, dummy_pdf, a_clearcoat, b1, b2);
            L = reflect(-V, H);
        }

        // Evaluate BRDF
        double NdotL = dot(N, L);
        double NdotV = dot(N, V);
        if (NdotL < 0 || NdotV < 0)
            return false;
        double LdotH = dot(L, H);
        double NdotH = dot(N, H);

        double pdf_diffuse = PBR::cosine_sample_hemisphere_pdf(NdotL);
        double pdf_specular = PBR::sample_GTR2_aniso_pdf(NdotH, dot(H, b1), dot(H, b2), ax, ay);
        double pdf_clearcoat = PBR::sample_GTR1_pdf(NdotH, a_clearcoat);
        double LdotH_safe = std::max(LdotH, std::numeric_limits<double>::epsilon());
        pdf_specular = pdf_specular / (4.0 * LdotH_safe);
        pdf_clearcoat = pdf_clearcoat / (4.0 * LdotH_safe);

        pdf = diffuse_prob * pdf_diffuse + specular_prob * pdf_specular + clearcoat_prob * pdf_clearcoat;
        if (pdf <= 0.0)
            return false;

        scattered = Ray(rec.p, L);

        // blend between dielectric and metal
        Color tint_color = PBR::CalculateTint(base_color);
        Color F0 = PBR::CalculateF0(base_color, tint_color, metallic, specular, specular_tint);
        Color sheen_color = PBR::CalculateSheenColor(tint_color, sheen_tint);

        // diffuse
        double FL = PBR::SchlickFresnel(NdotL), FV = PBR::SchlickFresnel(NdotV);
        double Fd90 = 0.5 + 2.0 * sqr(LdotH) * roughness;
        double Fd = lerp(1.0, Fd90, FL) * lerp(1.0, Fd90, FV);

        // specular
        double Ds = PBR::GTR2_aniso(NdotH, dot(H, b1), dot(H, b2), ax, ay);
        double FH = PBR::SchlickFresnel(LdotH);
        Color Fs = lerp(F0, Color::one(), FH);
        double Gs = PBR::SmithG_GGX_aniso(NdotL, dot(L, b1), dot(L, b2), ax, ay);
        Gs *= PBR::SmithG_GGX_aniso(NdotV, dot(V, b1), dot(V, b2), ax, ay);

        // sheen
        Vec3 Fsheen = FH * sheen * sheen_color;

        // clearcoat
        double Dr = PBR::GTR1(NdotH, a_clearcoat);
        double Fr = lerp(.04, 1.0, FH);
        double Gr = PBR::SmithG_GGX(NdotV, 0.25) * PBR::SmithG_GGX(NdotL, 0.25);

        // subsurface
        // NOT IMPLEMENTED
        double ss = Fd;

        f_r = (inv_pi * lerp(Fd, ss, subsurface) * base_color + Fsheen) * (1.0 - metallic) + 
              Gs * Fs * Ds + 0.25 * clearcoat * Gr * Fr * Dr * Vec3::one();

        return true;
    }

  private:
    Vec3 base_color;
    double subsurface;
    double metallic;
    double specular;
    double specular_tint;
    double roughness;
    double anisotropic;
    double sheen;
    double sheen_tint;
    double clearcoat;
    double clearcoat_gloss;
};