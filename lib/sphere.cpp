#include "sphere.h"

bool Sphere::hit(const Ray &ray, Interval t, HitRecord &rec) const
{
    Vec3 oc = center - ray.origin();
    auto a = ray.direction().length_squared();
    auto h = dot(ray.direction(), oc);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;
    else
    {
        double inv_denom = 1.0 / a;
        double sqrtd = sqrt(discriminant);
        double r = (h - sqrtd) * inv_denom;
        if (!t.contains(r))
        {
            r = (h + sqrtd) * inv_denom;
            if (!t.contains(r))
                return false;
        }

        rec.t = r;
        rec.p = ray.at(r);
        rec.normal = (rec.p - center) / radius;
        rec.mat = mat;
        return true;
    }
}