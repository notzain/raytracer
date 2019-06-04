#include "PNG.h"
#include <algorithm>
#include <cstdio>
#include <fmt/core.h>
#include <memory>
#include <png.h>

PNG::PNG(PNG::Options options)
    : options(options)
{
    imageBuffer.resize(options.width * options.height);
    std::fill(imageBuffer.begin(), imageBuffer.end(), RGBA {});
}

RGBA& PNG::pixel(int x, int y)
{
    return imageBuffer.at(x + options.width * y);
}

const RGBA& PNG::pixel(int x, int y) const
{
    return imageBuffer.at(x + options.width * y);
}

void PNG::write(int x, int y, const RGBA& rgba)
{
    pixel(x, y) = rgba;
}

bool PNG::save(std::string_view filename) const
{
    auto png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
        return false;

    auto info = png_create_info_struct(png);
    if (!info)
        return false;

    if (setjmp(png_jmpbuf(png)))
        return false;

    auto file = std::unique_ptr<FILE, decltype(&fclose)>(fopen(filename.data(), "wb"), &fclose);
    if (!file)
        return false;

    png_init_io(png, file.get());
    png_set_IHDR(
        png,
        info,
        options.width, options.height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    std::vector<png_byte> pixelRow(4 * options.width);
    for (int y = 0; y < options.height; ++y) {
        for (int x = 0; x < options.width; ++x) {
            std::copy(&pixel(x, y).r(), &pixel(x, y).a() + 1, pixelRow.begin() + x * 4);
        }
        png_write_row(png, pixelRow.data());
    }

    png_write_end(png, nullptr);

    return true;
}

PNGBuilder PNG::make()
{
    return PNGBuilder();
}

PNGBuilder& PNGBuilder::withSize(int width, int height)
{
    options.height = height;
    options.width = width;

    return *this;
}

PNGBuilder& PNGBuilder::withFillColor(const RGBA& rgba)
{
    fillColor = rgba;
    return *this;
}

PNGBuilder::operator PNG()
{
    PNG png(options);
    std::fill(png.imageBuffer.begin(), png.imageBuffer.end(), fillColor);

    return std::move(png);
}
