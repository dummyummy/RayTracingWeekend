#pragma once

#include "vecmath.h"

class Ray
{
  public:
    Ray() = default;

    Ray(const Vec3 &origin, const Vec3 &direction) : _origin(origin), _direction(direction), _time(0.0)
    {
    }

    Ray(const Vec3 &origin, const Vec3 &direction, double time) : _origin(origin), _direction(direction), _time(time)
    {
    }

    const Point3 &origin() const
    {
        return _origin;
    }

    const Vec3 &direction() const
    {
        return _direction;
    }

    double time() const
    {
        return _time;
    }

    const Vec3 unit_direction() const
    {
        return unit_vector(_direction);
    }

    Point3 at(double t) const;

  private:
    Vec3 _origin;
    Vec3 _direction;
    double _time;
};

inline Point3 Ray::at(double t) const
{
    return _origin + t * _direction;
}
