#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>

#include "rtweekend_defs.h"

class Vec3
{
  private:
    double e[3];

  public:
    Vec3() : e{0, 0, 0}
    {
    }
    Vec3(double e0, double e1, double e2) : e{e0, e1, e2}
    {
    }

    double x() const
    {
        return e[0];
    }
    double y() const
    {
        return e[1];
    }
    double z() const
    {
        return e[2];
    }

    Vec3 operator-() const
    {
        return Vec3(-e[0], -e[1], -e[2]);
    }
    double operator[](int i) const
    {
        return e[i];
    }
    double &operator[](int i)
    {
        return e[i];
    }

    Vec3 &operator+=(const Vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3 &operator*=(double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3 &operator/=(double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    static Vec3 one()
    {
        return Vec3(1.0, 1.0, 1.0);
    }

    static Vec3 zero()
    {
        return Vec3(0.0, 0.0, 0.0);
    }

    bool near_zero() const
    {
        return std::fabs(e[0]) < epsilon && std::fabs(e[1]) < epsilon && std::fabs(e[2]) < epsilon;
    }

    friend std::ostream &operator<<(std::ostream &out, const Vec3 &v);
    friend Vec3 operator+(const Vec3 &u, const Vec3 &v);
    friend Vec3 operator-(const Vec3 &u, const Vec3 &v);
    friend Vec3 operator*(const Vec3 &u, const Vec3 &v);
    friend Vec3 operator*(double t, const Vec3 &v);
    friend Vec3 operator*(const Vec3 &v, double t);
    friend Vec3 operator/(const Vec3 &v, double t);
    friend double dot(const Vec3 &u, const Vec3 &v);
    friend Vec3 cross(const Vec3 &u, const Vec3 &v);
    friend Vec3 unit_vector(const Vec3 &v);

    static void branchlessONB(const Vec3 &n, Vec3 &b1, Vec3 &b2);
    static Vec3 spherical_to_cartesian(double cos_theta, double phi);
};

// point3 is just an alias for Vec3, but useful for geometric clarity in the
// code.
using Point3 = Vec3;

class Color : public Vec3
{
  public:
    using Vec3::Vec3;

    Color(const Vec3 &v) : Vec3(v)
    {
    }

    double r() const
    {
        return x();
    }
    double g() const
    {
        return y();
    }
    double b() const
    {
        return z();
    }

    std::array<uint8_t, 3> to_uint8_array() const
    {
        return {static_cast<uint8_t>(255.999 * r()), static_cast<uint8_t>(255.999 * g()),
                static_cast<uint8_t>(255.999 * b())};
    }
};

std::ostream &operator<<(std::ostream &out, const Vec3 &v);
Vec3 operator+(const Vec3 &u, const Vec3 &v);
Vec3 operator-(const Vec3 &u, const Vec3 &v);
Vec3 operator*(const Vec3 &u, const Vec3 &v);
Vec3 operator*(double t, const Vec3 &v);
Vec3 operator*(const Vec3 &v, double t);
Vec3 operator/(const Vec3 &v, double t);
double dot(const Vec3 &u, const Vec3 &v);
Vec3 cross(const Vec3 &u, const Vec3 &v);
Vec3 lerp(const Vec3 &u, const Vec3 &v, double t);
Vec3 unit_vector(const Vec3 &v);
Vec3 reflect(const Vec3 &v, const Vec3 &n);
Vec3 refract(const Vec3 &v, const Vec3 &n, double ior);