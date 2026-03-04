#pragma once

#include "hittable.h"
#include "materials/material.h"
#include <memory>

class Sphere : public Hittable
{
  public:
    Sphere(const Point3 &center, double radius, std::shared_ptr<Material> mat)
        : center(center), radius(radius), mat(mat)
    {
    }
    virtual bool hit(const Ray &r, Interval t, HitRecord &rec) const override;

  private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> mat;
};