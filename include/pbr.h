#pragma once

#include "rtweekend.h"
#include "rtweekend_defs.h"
#include "vecmath.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace PBR
{
const double min_denom = std::numeric_limits<double>::epsilon();

inline double Luminance(const Color &color)
{
    return dot(Vec3(0.2126, 0.7152, 0.0722), color);
}

inline Color CalculateTint(const Color &base_color)
{
    double luminance = dot(Vec3(0.3, 0.6, 0.1), base_color);
    return luminance > 0.0 ? base_color / luminance : Color::one();
}

inline Color CalculateF0(const Color &base_color, const Color &tint_color, double metallic, double specular,
                         double specular_tint)
{
    return lerp(specular * 0.08 * lerp(Color::one(), tint_color, specular_tint), base_color, metallic);
}

inline Color CalculateSheenColor(const Color &tint_color, double sheen_tint)
{
    return lerp(Color::one(), tint_color, sheen_tint);
}

inline double SchlickFresnel(double cos_theta_d)
{
    double m = std::clamp(1.0 - cos_theta_d, 0.0, 1.0);
    float m2 = m * m;
    return m2 * m2 * m;
}

inline double GTR1(double NdotH, double a)
{
    if (a >= 1)
        return inv_pi;
    double a2 = sqr(a);
    double t = 1 + (a2 - 1) * sqr(NdotH);
    return (a2 - 1) / (pi * std::log(a2) * t);
}

inline double GTR2(double NdotH, double a)
{
    if (a >= 1)
        return inv_pi;
    double a2 = sqr(a);
    double t = 1 + (a2 - 1) * sqr(NdotH);
    return a2 / (pi * sqr(t));
}

inline double GTR2_aniso(double NdotH, double HdotX, double HdotY, double ax, double ay)
{
    double t = sqr(HdotX / ax) + sqr(HdotY / ay) + sqr(NdotH);
    return 1 / (pi * ax * ay * sqr(t));
}

inline double SmithG_GGX(double NdotV, double a)
{
    return 1.0 / (NdotV + std::sqrt(sqr(a) + sqr(NdotV) - sqr(a * NdotV)));
}

inline double SmithG_GGX_aniso(double NdotV, double VdotX, double VdotY, double ax, double ay)
{
    return 1.0 / (NdotV + std::sqrt(sqr(VdotX * ax) + sqr(VdotY * ay) + sqr(NdotV)));
}

/*** Sampling ***/

inline double uniform_sample_hemisphere_pdf(double cos_theta)
{
    return inv_two_pi;
}

inline Vec3 uniform_sample_hemisphere(const Vec3 &normal, double u, double v, double &p, const Vec3 &b1, const Vec3 &b2)
{
    double cos_theta = v;
    double phi = two_pi * u;
    Vec3 dir = Vec3::spherical_to_cartesian(cos_theta, phi);
    Vec3 l = dir.x() * b1 + dir.y() * b2 + dir.z() * normal;
    p = uniform_sample_hemisphere_pdf(cos_theta);
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

inline double sample_GTR1_pdf(double NdotH, double a)
{
    return GTR1(NdotH, a) * NdotH;
}

inline Vec3 sample_GTR1(const Vec3 &normal, double u, double v, double &pdf, double a, const Vec3 &b1, const Vec3 &b2)
{
    double phi = two_pi * u;
    double a2 = sqr(a);
    double denom = std::max(min_denom, 1.0 - a2);
    double cos_theta = std::min(std::sqrt((1.0 - std::pow(a2, 1.0 - v)) / denom), 1.0);
    Vec3 dir = Vec3::spherical_to_cartesian(cos_theta, phi);
    Vec3 h = dir.x() * b1 + dir.y() * b2 + dir.z() * normal;
    pdf = sample_GTR1_pdf(cos_theta, a);
    return h;
}

inline double sample_GTR2_pdf(double NdotH, double a)
{
    return GTR2(NdotH, a) * NdotH;
}

inline Vec3 sample_GTR2(const Vec3 &normal, double u, double v, double &pdf, double a, const Vec3 &b1, const Vec3 &b2)
{
    double phi = two_pi * u;
    double a2 = sqr(a);
    double denom = std::max(min_denom, 1.0 + (a2 - 1) * v);
    double cos_theta = std::min(std::sqrt((1.0 - v) / denom), 1.0);
    Vec3 dir = Vec3::spherical_to_cartesian(cos_theta, phi);
    Vec3 h = dir.x() * b1 + dir.y() * b2 + dir.z() * normal;
    pdf = sample_GTR2_pdf(cos_theta, a);
    return h;
}

inline double sample_GTR2_aniso_pdf(double NdotH, double HdotX, double HdotY, double ax, double ay)
{
    return GTR2_aniso(NdotH, HdotX, HdotY, ax, ay) * NdotH;
}

inline Vec3 sample_GTR2_aniso(const Vec3 &normal, double u, double v, double &pdf, double ax, double ay, const Vec3 &b1,
                              const Vec3 &b2)
{
    double px = ax * std::cos(two_pi * u);
    double py = ay * std::sin(two_pi * u);
    double denom = std::max(min_denom, 1.0 - v);
    Vec3 h_unorm = std::sqrt(v / denom) * (px * b1 + py * b2) + normal;
    Vec3 h = unit_vector(h_unorm);
    pdf = sample_GTR2_aniso_pdf(dot(h, normal), dot(h, b1), dot(h, b2), ax, ay);
    return h;
}
}; // namespace PBR