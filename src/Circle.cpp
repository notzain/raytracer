#include "Circle.h"
#include "Ray.h"
#include <cmath>

std::optional<Intersection> Circle::intersects(const Ray& ray) const
{
    const auto oc = ray.origin().value() - origin().value();
    const float a = ray.direction().value().dot(ray.direction().value());
    const float b = 2.f * oc.dot(ray.direction().value());
    const float c = oc.dot(oc) - radius() * radius();
    const float discr = b * b - 4 * a * c;

    if (discr < 0)
        return {};

    const float d = (-b - std::sqrt(discr)) / (2 * a);
    const auto pointAlong = ray.pointAlong(d).value();
    return Intersection {
        d,
        pointAlong,
        (pointAlong - origin().value()).normalized()
    };
}
