#pragma once

#include "material.h"

class Lambertian : public Material
{
  public:
    Lambertian(const Color &albedo, const Vec3 &emission) : Material(emission), albedo(albedo)
    {
    }

    bool scatter(const Ray &ray_in, const HitRecord &rec, Ray &scattered, Color &f_r, double &pdf) const override
    {
        f_r = albedo * inv_pi; // diffuse BRDF
        scattered = Ray(rec.p, Vec3::cosine_sample_hemisphere(rec.normal, random_double(), random_double(), pdf));
        return true;
    }

  private:
    Color albedo;
};
