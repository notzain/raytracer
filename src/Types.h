#ifndef RAYTRACER_TYPES_H
#define RAYTRACER_TYPES_H

#include <Eigen/Dense>
#include <array>
#include <functional>
#include <optional>
#include <utility>

namespace detail {
struct OriginTag;
struct DirectionTag;
}

template <typename T, typename Tag>
class StrongType {
public:
    constexpr StrongType()
        : value_()
    {
    }

    constexpr explicit StrongType(const T& value)
        : value_(value)
    {
    }

    constexpr T& value() { return value_; }
    constexpr const T& value() const { return value_; }

private:
    T value_;
};

using Vec3f = Eigen::Vector3f;
using Origin = StrongType<Vec3f, detail::OriginTag>;
using Direction = StrongType<Vec3f, detail::DirectionTag>;

class RGBA {
    std::array<int, 4> rgba { 0, 0, 0, 255 };

public:
    constexpr RGBA() = default;
    constexpr RGBA(int r, int g, int b, int a)
    {
        rgba[0] = r;
        rgba[1] = g;
        rgba[2] = b;
        rgba[3] = a;
    }

    constexpr RGBA(int r, int g, int b)
        : RGBA(r, g, b, 255)
    {
    }

    static RGBA fromVec3f(const Vec3f& vec)
    {
        return RGBA {
            int(vec[0] * 255.f),
            int(vec[1] * 255.f),
            int(vec[2] * 255.f),
        };
    }

    constexpr const int& r() const { return rgba[0]; }
    constexpr const int& g() const { return rgba[1]; }
    constexpr const int& b() const { return rgba[2]; }
    constexpr const int& a() const { return rgba[3]; }
};

struct Intersection {
    float distance {};
    Vec3f position {};
    Vec3f normal {};
};

struct Material {
    Vec3f attenuation {};
    std::optional<class Ray> (*scatter)(const class Ray& ray, const Intersection& intersection) {};
};

struct Hit {
    Intersection intersection;
    Material material;
};

struct IIntersectable {
    virtual std::optional<Hit> intersects(const class Ray& ray, float min, float max) const = 0;
};

template <typename T>
struct Intersectable : public IIntersectable {
    std::optional<Hit> intersects(const class Ray& ray, float min, float max) const override { return static_cast<T const&>(*this).intersects(ray, min, max); }
};

#endif //RAYTRACER_TYPES_H
