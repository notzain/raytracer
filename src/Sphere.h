#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H
#include "Types.h"

class Sphere : public Intersectable<Sphere> {
public:
    Sphere(Origin origin, float radius, Material material)
        : origin_(std::move(origin))
        , radius_(radius)
        , material_(std::move(material))
    {
    }

    constexpr const Origin& origin() const { return origin_; }
    constexpr float radius() const { return radius_; }

    std::optional<Hit> intersects(const class Ray& ray, float min, float max) const;

private:
    Origin origin_;
    float radius_;
    Material material_;
};

#endif //RAYTRACER_SPHERE_H
