#include "Circle.h"
#include "PNG.h"
#include "Ray.h"
#include <ctime>
#include <fmt/core.h>

RGBA colorOf(const Ray& ray)
{
    Circle circle(Origin(Eigen::Vector3f { 0, 0, -1 }), .5);
    if (circle.intersects(ray)) {
        return { 255, 0, 0 };
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

    Eigen::Vector3f bottomLeft(-2, -1, -1);
    Eigen::Vector3f horizontal(4, 0, 0);
    Eigen::Vector3f vertical(0, 2, 0);
    Eigen::Vector3f origin(0, 0, 0);

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