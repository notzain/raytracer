#include "Materials.h"
#include "Ray.h"
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

std::optional<Ray> Lambertian(const Ray& ray, const Intersection& hit)
{
    const auto target = hit.position + hit.normal + randomUnitSphere();
    return Ray {
        Origin(hit.position),
        Direction(target - hit.position)
    };
}

Vec3f reflect(const Vec3f& v, const Vec3f& n)
{
    return v - 2 * v.dot(n) * n;
}

std::optional<class Ray> Metal(const class Ray& ray, const Intersection& intersection)
{
    const auto reflected = reflect(ray.direction().value().normalized(), intersection.normal);
    const auto scattered = Ray(Origin(intersection.position), Direction(reflected));
    if (scattered.direction().value().dot(intersection.normal) > 0) {
        return {};
    }
    return scattered;

}