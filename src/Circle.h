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

    std::optional<HitRecord> intersects(const class Ray& ray) const;

private:
    Origin origin_;
    float radius_;
};

#endif //RAYTRACER_CIRCLE_H
