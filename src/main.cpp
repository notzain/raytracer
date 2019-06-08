#include "Camera.h"
#include "Circle.h"
#include "Materials.h"
#include "PNG.h"
#include "Ray.h"
#include "Scene.h"
#include <fmt/core.h>
#include <numeric>
#include <random>

template <typename Intersectable>
Vec3f colorOf(const Intersectable& intersectable, const Ray& ray, int depth)
{
    if (auto hit = intersectable.intersects(ray, .0001f, std::numeric_limits<float>::max())) {
        auto& intersection = hit->intersection;
        auto& material = hit->material;
        if (auto scatterRay = material.scatter(ray, intersection); scatterRay && depth < 50) {
            const auto color = colorOf(intersectable, *scatterRay, depth + 1);
            return Vec3f {
                color[0] * material.attenuation[0],
                color[1] * material.attenuation[1],
                color[2] * material.attenuation[2],
            };
        } else {
            return Vec3f { 0, 0, 0 };
        }
    }

    const auto unitDirection = ray.direction().value().normalized();
    float t = .5 * (unitDirection.y() + 1.f);
    return (1.f - t) * Vec3f(1, 1, 1) + t * Vec3f(.5, .7, 1);
}
template <typename Intersectable>
Vec3f sampledColorOf(int samples, int x, int width, int y, int height,
    const Intersectable& intersectable, const Camera& camera)
{
    Vec3f sampledColor(0, 0, 0);

    std::uniform_real_distribution<float> realDistribution(.0f, 1.f);
    std::mt19937 rng(x + y * width);

#pragma omp parallel for
    for (int i = 0; i < samples; ++i) {
        float u = float(x + realDistribution(rng)) / (float)width;
        float v = float(y + realDistribution(rng)) / (float)height;

        sampledColor += colorOf(intersectable, camera.asRay(u, v), 0);
    }

    sampledColor /= (float)samples;

    return sampledColor;
}

int main()
{
    PNG png = PNG::make()
                  .withSize(512, 256)
                  .withFillColor({ 255, 0, 0 });

    Camera camera(
        Origin({ 0, 0, 0 }),
        90,
        float(png.width()) / png.height());

    Scene scene;
    scene.add<Circle>(Origin({ 0, 0, -1 }), .5,
        Material { Vec3f(.8, .3, .3), &Lambertian });
    scene.add<Circle>(Origin({ 1, 0, -1 }), .5,
        Material { Vec3f(.8, .8, .8), &Metal });
    scene.add<Circle>(Origin({ 0, 100.5, -1 }), 100,
        Material { Vec3f(.8, .8, .0), &Lambertian });

#pragma omp parallel for collapse(2)
    for (int y = 0; y < png.height(); ++y) {
        for (int x = 0; x < png.width(); ++x) {
            png.write(x, y,
                RGBA::fromVec3f(
                    sampledColorOf(30, x, png.width(), y, png.height(), scene, camera)));
        }
    }

    fmt::print("{}", png.save("test.png"));
    return 0;
}