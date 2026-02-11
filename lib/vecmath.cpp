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

Vec3 Vec3::uniform_sample_hemisphere(const Vec3 &normal, double u, double v, double &p)
{
    double cos_theta = v;
    double sin_theta = std::sqrt(1.0 - v * v);
    double phi = two_pi * u;
    auto dir = Vec3(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);
    Vec3 b1, b2;
    branchlessONB(normal, b1, b2);
    p = inv_two_pi;
    return dir.x() * b1 + dir.y() * b2 + dir.z() * normal;
}

Vec3 Vec3::cosine_sample_hemisphere(const Vec3 &normal, double u, double v, double &p)
{
    double cos_theta = std::sqrt(v);
    double sin_theta = std::sqrt(1.0 - v);
    double phi = two_pi * u;
    auto dir = Vec3(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);
    Vec3 b1, b2;
    branchlessONB(normal, b1, b2);
    p = cos_theta * inv_pi;
    return dir.x() * b1 + dir.y() * b2 + dir.z() * normal;
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