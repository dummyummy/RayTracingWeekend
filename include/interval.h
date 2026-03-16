#pragma once

#include "rtweekend_defs.h"
#include <cmath>

class Interval
{
  public:
    double min, max;

    Interval() : min(+infinity), max(-infinity)
    {
    }

    Interval(double min, double max) : min(min), max(max)
    {
    }

    Interval(const Interval &a, const Interval &b)
    {
        min = std::fmin(a.min, b.min);
        max = std::fmax(a.max, b.max);
    }

    double size() const
    {
        return max - min;
    }

    bool contains(double x) const
    {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const
    {
        return min < x && x < max;
    }

    [[nodiscard]] Interval expanded(double delta) const 
    {
        auto padding = delta / 2.0;
        return Interval(min - padding, max + padding);
    }

    Interval& expand(double delta)
    {
        auto padding = delta / 2.0;
        min -= padding;
        max += padding;
        return *this;
    }

    double clamp(double x) const
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }

    static const Interval empty, universe;
};