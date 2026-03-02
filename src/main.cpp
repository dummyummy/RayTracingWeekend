#include <memory>

#include "material.h"
#include "stb_image.h"
#include "stb_image_write.h"

#include "camera.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"
#include "lambertian.h"
#include "principled_brdf.h"
#include "vecmath.h"

int main(int argc, char *argv[])
{
    // using namespace indicators;
    // spdlog::info("Welcome to spdlog!");
    // spdlog::error("Some error message with arg: {}", 1);

    // spdlog::warn("Easy padding in numbers like {:08d}", 12);
    // spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin:
    // {0:b}", 42); spdlog::info("Support for floats {:03.2f}", 1.23456);
    // spdlog::info("Positional args are {1} {0}..", "too", "supported");
    // spdlog::info("{:<30}", "left aligned");
    CLI::App app{"App description"};
    argv = app.ensure_utf8(argv);
    float aspect_ratio = 16.0f / 9.0f;
    app.add_option("--aspect_ratio", aspect_ratio, "Aspect ratio of the output image");
    int image_width = 256;
    app.add_option("--image_width", image_width, "Width of the output image");
    int image_spp = 16;
    app.add_option("--image_spp", image_spp, "Samples per pixel");
    std::string output_name = "output";
    app.add_option("--output_name", output_name, "Name of the output image");

    CLI11_PARSE(app, argc, argv);

    Camera cam;
    cam.aspect_ratio = aspect_ratio;
    cam.image_width = image_width;
    cam.num_samples = image_spp;
    cam.focal_length = 10.0;
    cam.fov = 90.0;

    indicators::ProgressBar bar{
        indicators::option::BarWidth{50},
        indicators::option::Start{"["},
        indicators::option::Fill{"="},
        indicators::option::Lead{">"},
        indicators::option::Remainder{" "},
        indicators::option::End{"]"},
        indicators::option::PostfixText{"Rendering image"},
        indicators::option::ForegroundColor{indicators::Color::green},
        indicators::option::ShowPercentage{true},
        indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};
    int last_progress = 0;
    cam.progress_callback = [&bar, &last_progress](double progress) {
        int progress_int = static_cast<int>(progress * 100.0);
        if (progress_int != last_progress)
        {
            bar.set_progress(progress_int);
            last_progress = progress_int;
        }
    };

    std::shared_ptr<Material> lambertian = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.8), Vec3(0.0, 0.0, 0.0));

    std::shared_ptr<Material> principled1 = std::shared_ptr<PrincipledBRDF>(
        new PrincipledBRDF(
            Vec3(0.0, 0.0, 0.0), 
            Vec3(0.8, 0.1, 0.1), 
            0.0, 
            0.0, 
            0.0, 
            1.0, 
            0.0, 
            0.0, 
            0.0, 
            0.0, 
            0.0, 
            0.0
        )
    );

    std::shared_ptr<Material> principled2 = std::shared_ptr<PrincipledBRDF>(
        new PrincipledBRDF(
            Vec3(0.0, 0.0, 0.0), 
            Vec3(0.1, 0.8, 0.1), 
            0.9, 
            0.0, 
            0.5, 
            0.4, 
            0.0, 
            0.0, 
            0.0, 
            0.0, 
            0.0, 
            0.0
        )
    );

    std::shared_ptr<Material> principled3 = std::shared_ptr<PrincipledBRDF>(
        new PrincipledBRDF(
            Vec3(0.0, 0.0, 0.0), 
            Vec3(0.1, 0.8, 0.4), 
            1.0, 
            0.0, 
            0.5, 
            0.1, 
            0.0, 
            0.0, 
            0.0, 
            0.0, 
            0.0, 
            0.0
        )
    );

    HittableList scene;
    scene.add(std::shared_ptr<Hittable>(new Sphere(Point3(-1.0, 0.0, -1.0), 0.5, principled1)));
    scene.add(std::shared_ptr<Hittable>(new Sphere(Point3(0.0, 0.5, -2.0), 1.0, principled2)));
    scene.add(std::shared_ptr<Hittable>(new Sphere(Point3(1.0, 0.0, -1.0), 0.5, principled3)));
    scene.add(std::shared_ptr<Hittable>(new Sphere(Point3(0.0, -1000.5, -2.0), 1000.0, lambertian)));

    cam.initialize();
    auto image_data = cam.render(scene);

    // Save the image as PNG
    if (stbi_write_png((output_name + ".png").c_str(), cam.image_width, cam.get_image_height(), cam.channel,
                       image_data.get(), cam.image_width * cam.channel))
        spdlog::info("Image saved successfully!");
    else
        spdlog::error("Failed to save image!");

    return 0;
}