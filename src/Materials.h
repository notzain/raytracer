#ifndef RAYTRACER_MATERIALS_H
#define RAYTRACER_MATERIALS_H

#include "Types.h"
#include <optional>

template <typename T>
std::function<std::optional<class Ray>(const class Ray& ray, const struct IIntersectable& intersection, const MaterialProperties& properties)> MakeMaterial()
{
    return t;
}

std::optional<class Ray> Lambertian(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties);
std::optional<class Ray> Metal(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties);
std::optional<class Ray> Dielectric(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties);

#endif //RAYTRACER_MATERIALS_H
