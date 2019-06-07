#ifndef RAYTRACER_CIRCLE_H
#define RAYTRACER_CIRCLE_H
#include "Types.h"

class Circle : public Intersectable<Circle> {
public:
    Circle(Origin origin, float radius)
        : origin_(std::move(origin))
        , radius_(radius)
    {
    }

    constexpr const Origin& origin() const { return origin_; }
    constexpr float radius() const { return radius_; }

    std::optional<Intersection> intersects(const class Ray& ray, float min, float max) const;

private:
    Origin origin_;
    float radius_;
    Material material_;
};

#endif //RAYTRACER_CIRCLE_H
