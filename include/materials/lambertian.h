#pragma once

#include "material.h"
#include "../sampling.h"

class Lambertian : public Material
{
  public:
    Lambertian(const Color &albedo, const Vec3 &emission) : Material(emission), albedo(albedo)
    {
    }

    bool scatter(const Ray &ray_in, const HitRecord &rec, Ray &scattered, Color &f_r, double &pdf) const override
    {
        Vec3 b1, b2;
        Vec3::branchlessONB(rec.normal, b1, b2);
        Vec3 ro = cosine_sample_hemisphere(rec.normal, random_double(), random_double(), pdf, b1, b2);
        scattered = Ray(rec.p, ro);
        f_r = albedo * inv_pi * dot(ro, rec.normal); // diffuse BRDF
        return true;
    }

  private:
    Color albedo;
};
