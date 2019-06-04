#ifndef RAYTRACER_TYPES_H
#define RAYTRACER_TYPES_H

#include <Eigen/Dense>
#include <array>

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

using Origin = StrongType<Eigen::Vector3f, detail::OriginTag>;
using Direction = StrongType<Eigen::Vector3f, detail::DirectionTag>;

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

    constexpr const int& r() const { return rgba[0]; }
    constexpr const int& g() const { return rgba[1]; }
    constexpr const int& b() const { return rgba[2]; }
    constexpr const int& a() const { return rgba[3]; }
};

template <typename T>
class Intersectable {
public:
    bool intersects(const class Ray& ray) const { return static_cast<T const&>(*this).intersects(ray); }
};

#endif //RAYTRACER_TYPES_H
