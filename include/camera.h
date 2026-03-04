#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include "hittable.h"
#include "materials/material.h"
#include "rtweekend.h"
#include "rtweekend_defs.h"
#include "vecmath.h"

class Camera
{
  public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 256;
    double fov = 60.0;
    int channel = 3;
    int num_samples = 16;
    int max_trace_depth = 50;
    Point3 lookfrom = Point3(0.0, 0.0, 0.0);
    Point3 lookat = Point3(0.0, 0.0, -1.0);
    Vec3 vup = Vec3(0.0, 1.0, 0.0);
    bool enable_skybox = true;

    std::function<void(double)> progress_callback = nullptr;
    Camera() = default;
    ~Camera() = default;

    int get_image_height()
    {
        return image_height;
    }

    int image_byte_size()
    {
        return image_width * image_height * channel;
    }

    std::unique_ptr<uint8_t[]> render(const Hittable &scene)
    {
        auto data = std::make_unique<uint8_t[]>(image_byte_size());
        render(scene, data.get());
        return data; // RVO
    }

    void initialize()
    {
        focal_length = (lookat - lookfrom).length();
        w = unit_vector(lookat - lookfrom);
        u = unit_vector(cross(vup, w)); // left hand frame
        v = cross(w, u);
        image_height = static_cast<int>(image_width / aspect_ratio);
        viewport_height = 2.0 * std::tan(degrees_to_radians(fov) / 2.0) * focal_length;
        viewport_width = viewport_height * ((double)image_width / image_height);
        viewport_u = viewport_width * u;
        viewport_v = viewport_height * (-v);
        viewport_upper_left = lookfrom + (focal_length * w) - viewport_u / 2.0 - viewport_v / 2.0;
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
    }

    void render(const Hittable &scene, uint8_t *data)
    {
        if (!data)
        {
            return;
        }

        int num_pixels = image_width * image_height;
        for (int i = 0; i < image_height; ++i)    // row
            for (int j = 0; j < image_width; ++j) // column
            {
                auto aa_color = Color(0.0, 0.0, 0.0);
                for (int s = 0; s < num_samples; ++s)
                {
                    auto ray = get_offseted_ray(i, j, s);
                    auto color = ray_color(ray, scene);
                    aa_color += color;
                }

                aa_color /= (double)num_samples;
                auto clamp_interval = Interval(0.0, 1.0);
                double gamma = 1.0 / 2.2;
                auto clamped_color = Color(std::pow(clamp_interval.clamp(aa_color.r()), gamma),
                                           std::pow(clamp_interval.clamp(aa_color.g()), gamma),
                                           std::pow(clamp_interval.clamp(aa_color.b()), gamma));
                auto uint8_color = clamped_color.to_uint8_array();

                // Color tonemapped_color = Color(
                //     aa_color.r() / (aa_color.r() + 1.0),
                //     aa_color.g() / (aa_color.g() + 1.0),
                //     aa_color.b() / (aa_color.b() + 1.0)
                // );
                // auto uint8_color = tonemapped_color.to_uint8_array();

                int pixels = (j + i * image_width);
                int idx = pixels * channel;
                data[idx] = uint8_color[0];     // Red channel
                data[idx + 1] = uint8_color[1]; // Green channel
                data[idx + 2] = uint8_color[2]; // Blue channel

                if (progress_callback)
                {
                    progress_callback((pixels + 1) / (double)num_pixels);
                }
            }
    }

  private:
    int image_height = -1;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)image_width / image_height);
    Vec3 viewport_u;
    Vec3 viewport_v;
    Vec3 viewport_upper_left;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    Vec3 u, v, w;
    double focal_length;

    Color ray_color(Ray ray, const Hittable &scene)
    {
        HitRecord rec;

        Color radiance(0.0, 0.0, 0.0);
        Vec3 throughput(1.0, 1.0, 1.0);

        for (int b = 1; b <= max_trace_depth; ++b)
        {
            if (!scene.hit(ray, Interval(epsilon, infinity), rec))
            {
                double a = 0.5 * (unit_vector(ray.direction()).y() + 1.0);

                // White furnace test
                // Color sky_color = Color(1.0, 1.0, 1.0);

                Color sky_color;
                if (enable_skybox)
                    sky_color = (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
                else
                    sky_color = Color::zero();

                radiance += throughput * sky_color;
                break;
            }

            Ray ro;
            Color f_r;
            double p;

            radiance += throughput * rec.mat->get_emission();

            if (!rec.mat->scatter(ray, rec, ro, f_r, p) || p <= 0.0)
                break;

            Vec3 weight = f_r / p; // BRDF * cos(theta) / p
            throughput = throughput * weight;

            // Russian Roulette
            if (b >= 3)
            {
                // Survive probability
                double q = std::max(throughput.x(), std::max(throughput.y(), throughput.z()));
                q = std::min(1.0, q);
                if (random_double() > q)
                {
                    break;
                }
                throughput /= q;
            }

            ray = ro;
        }

        return radiance;
    }

    Ray get_offseted_ray(int i, int j, int s)
    {
        auto pixel_offset = Hammersley(s, num_samples);
        auto pixel_center =
            viewport_upper_left + (j + pixel_offset.second) * pixel_delta_u + (i + pixel_offset.first) * pixel_delta_v;
        return Ray(lookfrom, pixel_center - lookfrom);
    }
};