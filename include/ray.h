#pragma once

#include "vecmath.h"

class Ray
{
  public:
    Ray() = default;
    Ray(const Vec3 &origin, const Vec3 &direction) : _origin(origin), _direction(direction)
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
    const Vec3 unit_direction() const
    {
        return unit_vector(_direction);
    }

    Point3 at(double t) const;

  private:
    Vec3 _origin;
    Vec3 _direction;
};

inline Point3 Ray::at(double t) const
{
    return _origin + t * _direction;
}
