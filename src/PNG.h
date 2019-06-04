#ifndef RAYTRACER_PNG_H
#define RAYTRACER_PNG_H

#include <array>
#include <string_view>
#include <vector>

class PNG {
    friend class PNGBuilder;
    PNG() = default;

public:
    struct Options {
        int width {};
        int height {};
    };
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

    explicit PNG(Options options);
    static PNGBuilder make();

    RGBA& pixel(int x, int y);
    const RGBA& pixel(int x, int y) const;

    int width() const { return options.width; }
    int height() const { return options.height; }

    void write(int x, int y, const RGBA& rgba);
    bool save(std::string_view filename) const;

private:
    Options options;
    std::vector<RGBA> imageBuffer;
};

class PNGBuilder {
public:
    PNGBuilder& withSize(int width, int height);
    PNGBuilder& withFillColor(const PNG::RGBA& rgba);

    operator PNG();

private:
    PNG::Options options;
    PNG::RGBA fillColor;
};

#endif //RAYTRACER_PNG_H
