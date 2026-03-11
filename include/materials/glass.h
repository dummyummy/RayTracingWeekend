#pragma once

#include <algorithm>

#include "material.h"
#include "pbr.h"

class Glass : public Material
{
  public:
    Glass(double ior) : Material(Color::zero()), _ior(ior)
    {
        f0 = sqr((1.0 - _ior) / (1.0 + _ior));
    }

    bool scatter(const Ray &ray_in, const HitRecord &rec, Ray &scattered, Color &f_r, double &pdf) const override
    {
        Vec3 ri = ray_in.unit_direction();
        double ior = rec.front_face ? 1.0 / _ior : _ior;
        double cos_theta = std::clamp(dot(-ri, rec.normal), 0.0, 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = ior * sin_theta > 1.0;
        Vec3 ro;
        double f = lerp(f0, 1.0, PBR::SchlickFresnel(cos_theta));
        if (cannot_refract || random_double() < f)
        {
            ro = reflect(ri, rec.normal);
            f_r = cannot_refract ? Color::one() : Color::one() * f;
            pdf = cannot_refract ? 1.0 : f;
        }
        else
        {
            ro = refract(ri, rec.normal, ior);
            f_r = Color::one() * (1.0 - f);
            pdf = 1.0 - f;
        }
        scattered = Ray(rec.p, ro, ray_in.time());
        return true;
    }

  private:
    double _ior;
    double f0;
};
