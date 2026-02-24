#pragma once

#include "material.h"
#include "pbr.h"

class Lambertian : public Material
{
  public:
    Lambertian(const Color &albedo, const Vec3 &emission) : Material(emission), albedo(albedo)
    {
    }

    bool scatter(const Ray &ray_in, const HitRecord &rec, Ray &scattered, Color &f_r, double &pdf) const override
    {
        f_r = albedo * inv_pi; // diffuse BRDF
        Vec3 b1, b2;
        Vec3::branchlessONB(rec.normal, b1, b2);
        scattered = Ray(rec.p, PBR::cosine_sample_hemisphere(rec.normal, random_double(), random_double(), pdf, b1, b2));
        return true;
    }

  private:
    Color albedo;
};
