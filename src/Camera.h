#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Types.h"

class Camera {
public:
    Camera(Direction forward, Direction right, Direction up)
        : forward_(std::move(forward))
        , right_(std::move(right))
        , up_(std::move(up))
    {
    }

    const Direction& forward() const { return forward_; }
    const Direction& right() const { return right_; }
    const Direction& up() const { return up_; }

private:
    Direction forward_;
    Direction right_;
    Direction up_;
};

#endif //RAYTRACER_CAMERA_H
