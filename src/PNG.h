#ifndef RAYTRACER_PNG_H
#define RAYTRACER_PNG_H

#include "Types.h"
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
    PNGBuilder& withFillColor(const RGBA& rgba);

    operator PNG();

private:
    PNG::Options options;
    RGBA fillColor;
};

#endif //RAYTRACER_PNG_H
