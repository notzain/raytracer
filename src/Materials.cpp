#include "Materials.h"
#include "Ray.h"
#include <random>
#include <variant>

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

std::optional<Ray> Lambertian(const Ray& ray, const Intersection& hit, const MaterialProperties& properties)
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

std::optional<Ray> Metal(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties)
{
    const float fuzz = [&properties]() -> float {
        if (auto fuzz = std::get_if<Fuzz>(&properties.variantProperties)) {
            return fuzz->value();
        }
        return 1.f;
    }();
    const auto reflected = reflect(ray.direction().value().normalized(), intersection.normal);
    const auto scattered = Ray(Origin(intersection.position), Direction(reflected + fuzz * randomUnitSphere()));
    if (scattered.direction().value().dot(intersection.normal) > 0) {
        return scattered;
    }

    return {};
}

std::optional<Vec3f> refract(const Vec3f& v, const Vec3f& n, float nt)
{
    auto uv = v.normalized();
    float dt = uv.dot(n);
    float disc = 1.f - nt * nt * (1 - dt * dt);
    if (disc > 0) {
        return { nt * (uv - n * dt) - n * sqrt(disc) };
    }
    return {};
}

std::optional<Ray> Dielectric(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties)
{
    Vec3f out_normal(0, 0, 0);
    auto reflected = reflect(ray.direction().value(), intersection.normal);
    if (ray.direction().value().dot(intersection.normal) > 0) {
        out_normal = -intersection.normal;
    } else {
    }
    return {};
}
