#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Types.h"
#include <Eigen/Dense>

class Ray {
public:
    Ray(Origin origin, Direction direction)
        : origin_(std::move(origin))
        , direction_(std::move(direction))
    {
    }

    constexpr const Origin& origin() const { return origin_; }
    constexpr const Direction& direction() const { return direction_; }
    Origin pointAlong(float length) const { return Origin { origin().value() + direction().value() * length }; }

private:
    Origin origin_;
    Direction direction_;
};

#endif //RAYTRACER_RAY_H
