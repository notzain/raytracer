#include "Camera.h"

Camera::Camera(Origin origin, float fov, float aspectRatio)
    : origin_(std::move(origin))
{
    float theta = fov * M_PI / 180;
    float halfHeight = tan(theta / 2);
    float halfWidth = aspectRatio * halfHeight;
    bottomLeft_ = Vec3f(-halfWidth, -halfHeight, -1.f);
    right_ = Vec3f(2.f * halfWidth, 0.f, 0.f);
    up_ = Vec3f(0.f, 2 * halfHeight, 0.f);
}

Ray Camera::asRay(float u, float v) const
{
    return {
        origin(),
        Direction(bottomLeft()
                  + (u * right())
                  + (v * up())
                  - origin().value())
    };
}
