#ifndef RAYTRACER_MATERIALS_H
#define RAYTRACER_MATERIALS_H

#include "Types.h"
#include <optional>

std::optional<class Ray> Lambertian(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties);
std::optional<class Ray> Metal(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties);
std::optional<class Ray> Dielectric(const class Ray& ray, const Intersection& intersection, const MaterialProperties& properties);

#endif //RAYTRACER_MATERIALS_H
