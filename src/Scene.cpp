#include "Scene.h"

std::optional<Hit> Scene::intersects(const Ray& ray, float min, float max) const
{
    std::optional<Hit> intersection {};
    float nearestHit = max;

    for (const auto& object : objects_) {
        if (auto hit = object->intersects(ray, min, nearestHit)) {
            nearestHit = hit->intersection.distance;
            intersection = hit;
        }
    }

    return intersection;
}
