#ifndef RAYTRACER_MATERIALS_H
#define RAYTRACER_MATERIALS_H

#include "Types.h"
#include <optional>

std::optional<class Ray> scatter(const class Ray& ray, const Intersection& intersection, const Material& material);

#endif //RAYTRACER_MATERIALS_H
