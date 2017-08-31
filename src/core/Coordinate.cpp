
#include "Coordinate.hpp"

#include <iostream>

namespace PixelSort {
    uint8_t RGBAColor::red() const {return (color & 0x00FF0000) >> 16;}
    uint8_t RGBAColor::green() const {return (color & 0x0000FF00) >> 8;}
    uint8_t RGBAColor::blue() const {return (color & 0x000000FF);}
    void RGBAColor::red(uint32_t r) { color = (color & 0xFF00FFFF) + ((r << 16) & 0x00FF0000); }
    void RGBAColor::green(uint32_t g) { color = (color & 0xFFFF00FF) + ((g << 8) & 0x0000FF00); }
    void RGBAColor::blue(uint32_t b) { color = (color & 0xFFFFFF00) + (b & 0x000000FF); }
};

namespace PixelSort {
    uint8_t Pixel::red() const {return (color & 0x00FF0000) >> 16;}
    uint8_t Pixel::green() const {return (color & 0x0000FF00) >> 8;}
    uint8_t Pixel::blue() const {return (color & 0x000000FF);}
    void Pixel::red(uint32_t r) { color = (color & 0xFF00FFFF) + ((r << 16) & 0x00FF0000); }
    void Pixel::green(uint32_t g) { color = (color & 0xFFFF00FF) + ((g << 8) & 0x0000FF00); }
    void Pixel::blue(uint32_t b) { color = (color & 0xFFFFFF00) + (b & 0x000000FF); }
};

PixelSort::BoxCoordinate::BoxCoordinate(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
        :Coordinate{x, y}, width(width), height(height) {};

PixelSort::BoundedCoordinate::BoundedCoordinate(unsigned int x, unsigned int y,
    unsigned int width, unsigned int height, unsigned int max_x, unsigned int max_y)
        :BoxCoordinate(x, y, width, height), max_x(max_x), max_y(max_y) {
    clampToMax(max_x, max_y);
}

PixelSort::BoundedCoordinate::BoundedCoordinate(PixelSort::BoxCoordinate box, 
    unsigned int max_x, unsigned int max_y)
        :BoxCoordinate(box), max_x(max_x), max_y(max_y) {
    clampToMax(max_x, max_y);
}

void PixelSort::BoundedCoordinate::clampToMax(unsigned int max_x, unsigned int max_y) {
    this->max_x = max_x;
    this->max_y = max_y;
    this->x = this->x < max_x ? this->x : max_x-1;
    this->y = this->y < max_y ? this->y : max_y-1;
    unsigned int x_max = this->x + this->width - 1;
    unsigned int y_max = this->y + this->height - 1;
    this->width = x_max < this->max_x ? this->width : this->max_x - this->x;
    this->height= y_max < this->max_y ? this->height : this->max_y - this->y;
}
