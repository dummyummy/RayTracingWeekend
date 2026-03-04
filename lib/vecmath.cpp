#include <cmath>

#include "rtweekend_defs.h"

#include "vecmath.h"

// Vector Utility Functions
std::ostream &operator<<(std::ostream &out, const Vec3 &v)
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

Vec3 operator+(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

Vec3 operator-(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

Vec3 operator*(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

Vec3 operator*(double t, const Vec3 &v)
{
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

Vec3 operator*(const Vec3 &v, double t)
{
    return t * v;
}

Vec3 operator/(const Vec3 &v, double t)
{
    return (1 / t) * v;
}

double dot(const Vec3 &u, const Vec3 &v)
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

Vec3 cross(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

Vec3 lerp(const Vec3 &u, const Vec3 &v, double t)
{
    return (1 - t) * u + t * v;
}

Vec3 unit_vector(const Vec3 &v)
{
    return v / v.length();
}

Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

Vec3 refract(const Vec3 &v, const Vec3 &n, double ior)
{
    double cos_theta = std::fmin(dot(-v, n), 1.0);
    Vec3 r_out_perp = ior * (v + cos_theta * n);
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

// https://graphics.pixar.com/library/OrthonormalB/paper.pdf
void Vec3::branchlessONB(const Vec3 &n, Vec3 &b1, Vec3 &b2)
{
    float sign = copysign(1.0f, n.z());
    const float a = -1.0f / (sign + n.z());
    const float b = n.x() * n.y() * a;
    b1 = Vec3(1.0f + sign * n.x() * n.x() * a, sign * b, -sign * n.x());
    b2 = Vec3(b, sign + n.y() * n.y() * a, -n.y());
}

Vec3 Vec3::spherical_to_cartesian(double cos_theta, double phi)
{
    double sin_theta = std::sqrt(1.0 - sqr(cos_theta));
    double cos_phi = cos(phi);
    double sin_phi = sin(phi);
    return Vec3(cos_phi * sin_theta, sin_phi * sin_theta, cos_theta);
}
