#pragma once

#include <cmath>
#include <limits>
#include <random>
#include <utility>

constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double min_denom = std::numeric_limits<double>::epsilon();
constexpr double epsilon = 1e-8;
const double pi = std::acos(-1.0);
const double inv_pi = 1.0 / pi;
const double two_pi = 2.0 * pi;
const double inv_two_pi = 1.0 / two_pi;

// Utility Functions
inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline std::pair<double, double> Hammersley(int i, int numSamples)
{
    int b = i;
    b = (b << 16u) | (b >> 16u);
    b = ((b & 0x55555555u) << 1u) | ((b & 0xAAAAAAAAu) >> 1u);
    b = ((b & 0x33333333u) << 2u) | ((b & 0xCCCCCCCCu) >> 2u);
    b = ((b & 0x0F0F0F0Fu) << 4u) | ((b & 0xF0F0F0F0u) >> 4u);
    b = ((b & 0x00FF00FFu) << 8u) | ((b & 0xFF00FF00u) >> 8u);

    double radicalInverseVDC = b * 2.3283064365386963e-10;

    return std::make_pair((i / numSamples), radicalInverseVDC);
}

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max + 1));
}

template <typename T> T lerp(T u, T v, T w) = delete;

template <> inline float lerp(float u, float v, float w)
{
    return u * (1 - w) + v * w;
}

template <> inline double lerp(double u, double v, double w)
{
    return u * (1 - w) + v * w;
}

template <typename T> T sqr(T u) = delete;

template <> inline float sqr(float u)
{
    return u * u;
}

template <> inline double sqr(double u)
{
    return u * u;
}
