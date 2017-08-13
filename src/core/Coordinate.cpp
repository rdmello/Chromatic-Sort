
#include "Coordinate.hpp"

PixelSort::Coordinate::Coordinate(unsigned int x, unsigned int y) : x(x), y(y) {};
PixelSort::RGBColor::RGBColor(double r, double g, double b) : r(r), g(g), b(b) {};

namespace PixelSort {
    double RGBColor::red() const {return r;}
    double RGBColor::green() const {return g;}
    double RGBColor::blue() const {return b;}
    void RGBColor::red(double r) { this->r = r; }
    void RGBColor::green(double g) { this->g = g; }
    void RGBColor::blue(double b) { this->b = b; }
};

PixelSort::ColorCoordinate::ColorCoordinate(const PixelSort::Coordinate& coord,
    const RGBColor& color):
    Coordinate(coord), RGBColor(color) {};

PixelSort::ColorCoordinate::ColorCoordinate():
    Coordinate(0, 0), RGBColor(0, 0, 0) {};

PixelSort::BoxCoordinate::BoxCoordinate(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
        :Coordinate(x, y), width(width), height(height) {};

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
