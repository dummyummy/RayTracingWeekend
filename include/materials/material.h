#pragma once

#include "../hittable.h"
#include "../vecmath.h"

class Material
{
  private:
    Vec3 emission;

  public:
    virtual ~Material() = default;
    Material() = delete;
    Material(const Material &) = default;
    Material &operator=(const Material &) = default;
    Material(const Vec3 &emission) : emission(emission)
    {
    }

    Vec3 get_emission() const
    {
        return emission;
    }

    // f_r equals BRDF * geometric factor
    virtual bool scatter(const Ray &ray_in, const HitRecord &rec, Ray &scattered, Color &f_r, double &pdf) const = 0;
};