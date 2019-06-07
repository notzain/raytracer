#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "Ray.h"
#include "Types.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Camera {
public:
    Camera(Origin origin, float fov, float aspectRatio);

    const Origin& origin() const { return origin_; }
    const Vec3f& bottomLeft() const { return bottomLeft_; }
    const Vec3f& right() const { return right_; }
    const Vec3f& up() const { return up_; }

    Ray asRay(float u, float v) const;

private:
    Origin origin_;
    Vec3f bottomLeft_;
    Vec3f right_;
    Vec3f up_;
};

#endif //RAYTRACER_CAMERA_H
