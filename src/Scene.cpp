#include "Scene.h"

std::optional<Intersection> Scene::intersects(const struct Ray& ray, float min, float max) const
{
    std::optional<Intersection> intersection {};
    float nearestHit = max;

    for (const auto& object : objects_) {
        if (auto hit = object->intersects(ray, min, nearestHit)) {
            nearestHit = hit->distance;
            intersection = hit;
        }
    }

    return intersection;
}
