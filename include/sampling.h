#pragma once

#include "rtweekend_defs.h"
#include "rtweekend.h"
#include <cmath>

inline double uniform_sample_hemisphere_pdf()
{
    return inv_two_pi;
}

inline Vec3 uniform_sample_hemisphere(const Vec3 &normal, double u, double v, double &p, const Vec3 &b1, const Vec3 &b2)
{
    double cos_theta = v;
    double phi = two_pi * u;
    Vec3 dir = Vec3::spherical_to_cartesian(cos_theta, phi);
    Vec3 l = dir.x() * b1 + dir.y() * b2 + dir.z() * normal;
    p = uniform_sample_hemisphere_pdf();
    return l;
}

inline double cosine_sample_hemisphere_pdf(double cos_theta)
{
    return cos_theta * inv_pi;
}

inline Vec3 cosine_sample_hemisphere(const Vec3 &normal, double u, double v, double &p, const Vec3 &b1, const Vec3 &b2)
{
    double cos_theta = std::sqrt(v);
    double phi = two_pi * u;
    Vec3 dir = Vec3::spherical_to_cartesian(cos_theta, phi);
    Vec3 l = dir.x() * b1 + dir.y() * b2 + dir.z() * normal;
    p = cosine_sample_hemisphere_pdf(cos_theta);
    return l;
}

inline double uniform_sample_disk_pdf()
{
    return inv_two_pi;
}

inline Vec3 uniform_sample_disk(double u, double v, double &p, const Vec3 &b1, const Vec3 &b2)
{
    double r = std::sqrt(u);
    double phi = two_pi * v;
    Vec3 dir = r * (std::cos(phi) * b1 + std::sin(phi) * b2);
    p = uniform_sample_disk_pdf();
    return dir;
}
