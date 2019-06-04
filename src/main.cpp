#include "PNG.h"
#include <ctime>
#include <fmt/core.h>

int main()
{
    PNG png = PNG::make()
                  .withSize(200, 100)
                  .withFillColor({ 255, 0, 0 });

    for (int y = 0; y < png.height(); ++y) {
        for (int x = 0; x < png.width(); ++x) {
            const PNG::RGBA rgba {
                int((float)x / (float)png.width() * 255),
                int((float)y / (float)png.height() * 255),
                0
            };
            png.write(x, y, rgba);
        }
    }
    fmt::print("{}", png.save("test.png"));
    return 0;
}