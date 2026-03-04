#pragma once

#include "material.h"
#include "pbr.h"

class LightMat : public Material
{
  public:
    LightMat(const Vec3 &emission) : Material(emission)
    {
    }

    bool scatter(const Ray &ray_in, const HitRecord &rec, Ray &scattered, Color &f_r, double &pdf) const override
    {
        return false;
    }
};
