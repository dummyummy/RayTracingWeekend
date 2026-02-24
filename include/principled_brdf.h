#pragma once

#include "material.h"
#include "pbr.h"
#include "rtweekend_defs.h"
#include "vecmath.h"
#include <limits>

class PrincipledBRDF : public Material
{
  public:
    PrincipledBRDF(const Vec3 &emission, const Vec3 &baseColor, float metallic, float subsurface, float specular,
                   float roughness, float specularTint, float anisotropic, float sheen, float sheenTint,
                   float clearcoat, float clearcoatGloss) : Material(emission)
    {
        this->baseColor = baseColor;
        this->subsurface = subsurface;
        this->metallic = metallic;
        this->specular = specular;
        this->specularTint = specularTint;
        this->roughness = roughness;
        this->anisotropic = anisotropic;
        this->sheen = sheen;
        this->sheenTint = sheenTint;
        this->clearcoat = clearcoat;
        this->clearcoatGloss = clearcoatGloss;
    }

    bool scatter(const Ray &ray_in, const HitRecord &rec, Ray &scattered, Color &f_r, double &pdf) const override
    {
        Vec3 V = -ray_in.unit_direction();
        Vec3 N = rec.normal;
        double a = sqr(roughness);
        double a_clearcoat = lerp(.1, .001, clearcoatGloss);
        double aspect = std::sqrt(1.0 - anisotropic * 0.9);
        double ax = std::max(.001, sqr(roughness) / aspect);
        double ay = std::max(.001, sqr(roughness) * aspect);
        Vec3 L, H;
        Vec3 b1, b2;
        Vec3::branchlessONB(N, b1, b2);

        // MIS
        double diffuse_prob = 1.0;
        double specular_prob = 1.0;
        double clearcoat_prob = 1.0;
        double prob_sum = diffuse_prob + specular_prob + clearcoat_prob;
        diffuse_prob /= prob_sum;
        specular_prob /= prob_sum;
        clearcoat_prob /= prob_sum;
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
        Color tint_color = PBR::CalculateTint(baseColor);
        Color F0 = PBR::CalculateF0(baseColor, tint_color, metallic, specular, specularTint);
        Color sheen_color = PBR::CalculateSheenColor(tint_color, sheenTint);

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

        f_r = (inv_pi * lerp(Fd, ss, subsurface) * baseColor + Fsheen) * (1.0 - metallic) + Gs * Fs * Ds +
              0.25 * clearcoat * Gr * Fr * Dr * Vec3::one();

        return true;
    }

  private:
    Vec3 baseColor;
    double subsurface;
    double metallic;
    double specular;
    double specularTint;
    double roughness;
    double anisotropic;
    double sheen;
    double sheenTint;
    double clearcoat;
    double clearcoatGloss;
};