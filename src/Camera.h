#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Ray.h"
#include "Types.h"

class Camera {
public:
    Camera(Origin origin, Direction forward, Direction right, Direction up)
        : origin_(std::move(origin))
        , forward_(std::move(forward))
        , right_(std::move(right))
        , up_(std::move(up))
    {
    }
    const Origin& origin() const { return origin_; }
    const Direction& forward() const { return forward_; }
    const Direction& right() const { return right_; }
    const Direction& up() const { return up_; }

    Ray asRay(float u, float v) const
    {
        return {
            origin(),
            Direction(forward().value() + u * right().value() + v * up().value() - origin().value())
        };
    }

private:
    Origin origin_;
    Direction forward_;
    Direction right_;
    Direction up_;
};

#endif //RAYTRACER_CAMERA_H
