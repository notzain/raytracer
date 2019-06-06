#include "Circle.h"
#include "PNG.h"
#include "Ray.h"
#include "Scene.h"
#include <fmt/core.h>
#include <memory>
#include <numeric>
#include <vector>

template <typename Intersectable>
RGBA colorOf(const Intersectable& intersectable, Ray& ray)
{
    if (auto hit = intersectable.intersects(ray, .0f, std::numeric_limits<float>::max())) {
        const auto color = .5f * Eigen::Vector3f(hit->normal.x() + 1, hit->normal.y() + 1, hit->normal.z() + 1);
        return {
            int(color[0] * 255.f),
            int(color[1] * 255.f),
            int(color[2] * 255.f),
        };
    }

    const auto unitDirection = ray.direction().value().normalized();
    float t = .5 * (unitDirection.y() + 1.f);
    const auto color = (1.f - t) * Eigen::Vector3f(1, 1, 1) + t * Eigen::Vector3f(.5, .7, 1);
    return {
        int(color[0] * 255.f),
        int(color[1] * 255.f),
        int(color[2] * 255.f),
    };
}

int main()
{
    PNG png = PNG::make()
                  .withSize(1024, 512)
                  .withFillColor({ 255, 0, 0 });

    const Eigen::Vector3f bottomLeft(-2, -1, -1);
    const Eigen::Vector3f horizontal(4, 0, 0);
    const Eigen::Vector3f vertical(0, 2, 0);
    const Eigen::Vector3f origin(0, 0, 0);

    Scene scene;
    scene.add<Circle>(
        Origin(Eigen::Vector3f { 0, 0, -1 }), .5);
    scene.add<Circle>(
        Origin(Eigen::Vector3f { .5, 100.5, -1 }), 100);

#pragma omp parallel for collapse(2)
    for (int y = 0; y < png.height(); ++y) {
        for (int x = 0; x < png.width(); ++x) {
            const float u = x / (float)png.width();
            const float v = y / (float)png.height();

            Ray ray(Origin(origin), Direction(bottomLeft + u * horizontal + v * vertical));
            png.write(x, y, colorOf(scene, ray));
        }
    }

    fmt::print("{}", png.save("test.png"));
    return 0;
}