#pragma once

#include "interval.h"
#include "ray.h"
#include "vecmath.h"
#include <utility>

class AABB
{
  public:
    Interval x, y, z;
    AABB()
    {
    }
    AABB(const Interval &x, const Interval &y, const Interval &z) : x(x), y(y), z(z)
    {
    }
    AABB(const Point3 &a, const Point3 &b)
    {
        x = Interval(std::min(a.x(), b.x()), std::max(a.x(), b.x()));
        y = Interval(std::min(a.y(), b.y()), std::max(a.y(), b.y()));
        z = Interval(std::min(a.z(), b.z()), std::max(a.z(), b.z()));
    }
    AABB(const AABB &a, const AABB &b)
    {
        x = Interval(a.x, b.x);
        y = Interval(a.y, b.y);
        z = Interval(a.z, b.z);
    }
    const Interval &axis_interval(int n) const
    {
        if (n == 0)
            return x;
        if (n == 1)
            return y;
        return z;
    }
    bool hit(const Ray &r, Interval ray_t) const
    {
        const Point3 &ray_origin = r.origin();
        const Vec3 &ray_dir = r.direction();

        for (int i = 0; i < 3; i++)
        {
            const Interval &ax = axis_interval(i);
            const double dir_inv = 1.0 / ray_dir[i];

            double t0 = (ax.min - ray_origin[i]) * dir_inv;
            double t1 = (ax.max - ray_origin[i]) * dir_inv;

            if (dir_inv < 0.0)
            {
                std::swap(t0, t1);
            }

            if (t0 > ray_t.min)
                ray_t.min = t0;
            if (t1 < ray_t.max)
                ray_t.max = t1;

            // Any operation on NaN will return false
            if (ray_t.max <= ray_t.min)
                return false;
        }

        return true;
    }
};