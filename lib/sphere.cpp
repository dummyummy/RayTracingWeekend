#include "sphere.h"

bool Sphere::hit(const Ray &ray, Interval t, HitRecord &rec) const
{
    Point3 ct = center + movement * ray.time();
    Vec3 oc = ct - ray.origin();
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
        rec.normal = (rec.p - ct) / radius;
        rec.mat = mat;
        rec.set_face_normal(ray, rec.normal);
        return true;
    }
}