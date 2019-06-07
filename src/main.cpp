#include "Camera.h"
#include "Circle.h"
#include "PNG.h"
#include "Ray.h"
#include "Scene.h"
#include <fmt/core.h>
#include <numeric>
#include <random>

Vec3f randomUnitSphere()
{
    Vec3f p(0, 0, 0);
    static std::uniform_real_distribution<float> realDistribution(.0f, 1.f);
    static std::mt19937 rng(std::random_device {}());

    do {
        p = 2.f * Vec3f(realDistribution(rng), realDistribution(rng), realDistribution(rng))
            - Vec3f(1, 1, 1);
    } while (p.squaredNorm() >= 1.f);

    return p;
}

template <typename Intersectable>
Vec3f colorOf(const Intersectable& intersectable, const Ray& ray)
{
    if (const auto hit = intersectable.intersects(ray, .0001f, std::numeric_limits<float>::max())) {
        const auto target = hit->position + hit->normal + randomUnitSphere();
        const auto nextRay = Ray(Origin(hit->position), Direction(target - hit->position));
        return .5 * colorOf(intersectable, nextRay);
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

        sampledColor += colorOf(intersectable, camera.asRay(u, v));
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
    scene.add<Circle>(Origin({ 0, 0, -1 }), .5);
    scene.add<Circle>(Origin({ 0, 100.5, -1 }), 100);

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