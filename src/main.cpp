#include "Camera.h"
#include "Circle.h"
#include "PNG.h"
#include "Ray.h"
#include "Scene.h"
#include <fmt/core.h>
#include <memory>
#include <numeric>
#include <random>
#include <vector>

template <typename Intersectable>
RGBA colorOf(const Intersectable& intersectable, Ray& ray)
{
    if (auto hit = intersectable.intersects(ray, .0f, std::numeric_limits<float>::max())) {
        const auto color = .5f * Vec3f(hit->normal.x() + 1, -1 * hit->normal.y() + 1, hit->normal.z() + 1);
        return {
            int(color[0] * 255.f),
            int(color[1] * 255.f),
            int(color[2] * 255.f),
        };
    }

    const auto unitDirection = ray.direction().value().normalized();
    float t = .5 * (unitDirection.y() + 1.f);
    const auto color = (1.f - t) * Vec3f(1, 1, 1) + t * Vec3f(.5, .7, 1);
    return {
        int(color[0] * 255.f),
        int(color[1] * 255.f),
        int(color[2] * 255.f),
    };
}
template <typename Intersectable>
RGBA antialiasedColorOf(int samples, int x, int width, int y, int height, const Intersectable& intersectable, const Camera& camera)
{
    Vec3f sampledColor(0, 0, 0);

    std::uniform_real_distribution<float> realDistribution(.0f, 1.f);
    std::mt19937 rng(x + y * width);

#pragma omp parallel for
    for (int i = 0; i < samples; ++i) {
        float u = float(x + realDistribution(rng)) / (float)width;
        float v = float(y + realDistribution(rng)) / (float)height;

        Ray ray = camera.asRay(u, v);
        const auto color = colorOf(intersectable, ray);
        sampledColor[0] += color.r();
        sampledColor[1] += color.g();
        sampledColor[2] += color.b();
    }
    sampledColor /= (float)samples;

    return RGBA {
        int(sampledColor[0]),
        int(sampledColor[1]),
        int(sampledColor[2]),
    };
}

int main()
{
    PNG png = PNG::make()
                  .withSize(200, 100)
                  .withFillColor({ 255, 0, 0 });

    Camera camera(Origin({ 0, 0, 0 }),
        Direction({ -2, -1, -1 }),
        Direction({ 4, 0, 0 }),
        Direction({ 0, 2, 0 }));

    Scene scene;
    scene.add<Circle>(
        Origin({ 0, 0, -1 }), .5);
    scene.add<Circle>(
        Origin({ .5, 100.5, -1 }), 100);

#pragma omp parallel for collapse(2)
    for (int y = 0; y < png.height(); ++y) {
        for (int x = 0; x < png.width(); ++x) {
            png.write(x, y,
                antialiasedColorOf(100, x, png.width(), y, png.height(), scene, camera));
        }
    }

    fmt::print("{}", png.save("test.png"));
    return 0;
}