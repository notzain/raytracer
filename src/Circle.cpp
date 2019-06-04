#include "Circle.h"
#include "Ray.h"

bool Circle::intersects(const Ray& ray) const
{
    const auto oc = ray.origin().value() - origin().value();
    const float a = ray.direction().value().dot(ray.direction().value());
    const float b = 2.f * oc.dot(ray.direction().value());
    const float c = oc.dot(oc) - radius() * radius();
    const float discr = b * b - 4 * a * c;
    return discr > 0;
}
