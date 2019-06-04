#include "Circle.h"
#include "PNG.h"
#include "Ray.h"
#include <fmt/core.h>
#include <omp.h>

RGBA colorOf(const Ray& ray)
{
    Circle circle(Origin(Eigen::Vector3f { 0, 0, -1 }), .5);
    if (auto hit = circle.intersects(ray)) {
        const auto hitPos = (ray.pointAlong(hit->distance).value() - Eigen::Vector3f(0, 0, -1))
                                .normalized();
        const auto color = .5f * Eigen::Vector3f(hitPos.x() + 1, hitPos.y() + 1, hitPos.z() + 1);
        return {
            255 - int(color[0] * 255.f),
            255 - int(color[1] * 255.f),
            255 - int(color[2] * 255.f),
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
                  .withSize(200, 100)
                  .withFillColor({ 255, 0, 0 });

    const Eigen::Vector3f bottomLeft(-2, -1, -1);
    const Eigen::Vector3f horizontal(4, 0, 0);
    const Eigen::Vector3f vertical(0, 2, 0);
    const Eigen::Vector3f origin(0, 0, 0);

#pragma omp parallel for collapse(2)
    for (int y = 0; y < png.height(); ++y) {
        for (int x = 0; x < png.width(); ++x) {
            const float u = x / (float)png.width();
            const float v = y / (float)png.height();

            Ray ray(Origin(origin), Direction(bottomLeft + u * horizontal + v * vertical));
            png.write(x, y, colorOf(ray));
        }
    }

    fmt::print("{}", png.save("test.png"));
    return 0;
}