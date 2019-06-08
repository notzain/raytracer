#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include "Types.h"
#include <memory>
#include <vector>

class Scene : public Intersectable<Scene> {
public:
    Scene() = default;
    std::optional<Hit> intersects(const class Ray& ray, float min, float max) const;

    template <typename T, typename... Args>
    T& add(Args&&... args)
    {
        auto& uptr = objects_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        return static_cast<T&>(*uptr);
    }

private:
    using IntersectablePtr = std::unique_ptr<IIntersectable>;
    std::vector<IntersectablePtr> objects_;
};

#endif //RAYTRACER_SCENE_H
