#pragma once

#include "aabb.h"
#include "hittable.h"
#include "materials/material.h"
#include <memory>

class Sphere : public Hittable
{
  public:
    Sphere(const Point3 &center, double radius, std::shared_ptr<Material> mat)
        : center(center), movement(Vec3::zero()), radius(radius), mat(mat)
    {
        auto rvec = Vec3(radius, radius, radius);
        bbox = AABB(center - rvec, center + rvec);
    }
    Sphere(const Point3 &center, const Vec3 &movement, double radius, std::shared_ptr<Material> mat)
        : center(center), movement(movement), radius(radius), mat(mat)
    {
        auto rvec = Vec3(radius, radius, radius);
        AABB bbox1 = AABB(center - rvec, center + rvec);
        AABB bbox2 = AABB(center - rvec + movement, center + rvec + movement);
        bbox = AABB(bbox1, bbox2);
    }
    virtual bool hit(const Ray &r, Interval t, HitRecord &rec) const override;
    virtual AABB bounding_box() const override
    {
        return bbox;
    }

  private:
    Point3 center;
    Vec3 movement;
    double radius;
    AABB bbox;
    std::shared_ptr<Material> mat;
};