#include "rtweekend.h"
#include "vecmath.h"

namespace PBR
{
inline Color CalculateTint(const Color &base_color)
{
    double luminance = dot(Vec3(0.3, 0.6, 0.1), base_color);
    return luminance > 0.0 ? base_color * (1.0 / luminance) : Color::one();
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
}; // namespace PBR