#include "Circle.h"
#include "Ray.h"
#include <cmath>
#include <functional>

template <typename Operation>
float distance(float b, float discr, float a, Operation op)
{
    return op(b, std::sqrt(discr)) / a;
}

std::optional<Hit> Circle::intersects(const Ray& ray, float min, float max) const
{
    const auto oc = ray.origin().value() - origin().value();
    const float a = ray.direction().value().dot(ray.direction().value());
    const float b = oc.dot(ray.direction().value());
    const float c = oc.dot(oc) - radius() * radius();
    const float discr = b * b - a * c;

    if (discr > 0) {
        const auto makeIntersection = [&](float d) -> Hit {
            const auto pointAlong = ray.pointAlong(d).value();
            return {
                Intersection { d,
                    pointAlong,
                    (pointAlong - origin().value()) / radius() },
                material_
            };
        };

        if (float d = distance(-b, discr, a, std::minus<>()); d < max && d > min) {
            return makeIntersection(d);
        } else if (d = distance(-b, discr, a, std::plus<>()); d < max && d > min) {
            return makeIntersection(d);
        }
    }

    return {};
}
