#include "Materials.h"
#include "Ray.h"
#include <random>
#include <variant>

static std::uniform_real_distribution<float> realDistribution(.0f, 1.f);
static std::mt19937 rng(std::random_device {}());

Vec3f randomUnitSphere()
{
    Vec3f p(0, 0, 0);
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

float schlick(float cos, float refract)
{
    float r = (1 - refract) / (1 + refract);
    r *= r;
    return r + (1 - r) * pow(1 - cos, 5);
}

std::optional<Ray> Dielectric(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties)
{
    Vec3f out_normal(0, 0, 0);
    auto reflected = reflect(ray.direction().value(), intersection.normal);

    const float refractIndex = [&properties]() -> float {
        if (auto refract = std::get_if<RefractIndex>(&properties.variantProperties)) {
            return refract->value();
        }
        return 1.f;
    }();

    float nt = 0;
    float refProb = 1;
    float cos = 0;
    if (ray.direction().value().dot(intersection.normal) > 0) {
        out_normal = -intersection.normal;
        nt = refractIndex;
        cos = refractIndex * ray.direction().value().dot(intersection.normal) / ray.direction().value().norm();
    } else {
        out_normal = intersection.normal;
        nt = 1.f / refractIndex;
        cos = -ray.direction().value().dot(intersection.normal) / ray.direction().value().norm();
    }

    auto refracted = refract(ray.direction().value(), out_normal, nt);
    if (refracted) {
        refProb = schlick(cos, refractIndex);
    }

    if (realDistribution(rng) < refProb) {
        return Ray {
            Origin(intersection.position),
            Direction(reflected)
        };
    }

    return Ray {
        Origin(intersection.position),
        Direction(*refracted)
    };
}
