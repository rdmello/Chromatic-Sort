
#include "Coordinate.hpp"

PixelSort::Coordinate::Coordinate(int x, int y) : x(x), y(y) {};

PixelSort::ColorCoordinate::ColorCoordinate(const PixelSort::Coordinate& coord,
    const Magick::ColorRGB& color):
    Coordinate(coord), Magick::ColorRGB(color) {};

PixelSort::ColorCoordinate::ColorCoordinate():
    Coordinate(0, 0), Magick::ColorRGB(0, 0, 0) {};

PixelSort::BoxCoordinate::BoxCoordinate(int x, int y, int width, int height)
        :Coordinate(x, y), width(width), height(height) {};

PixelSort::BoundedCoordinate::BoundedCoordinate(int x, int y,
    int width, int height, int max_x, int max_y)
        :BoxCoordinate(x, y, width, height), max_x(max_x), max_y(max_y) {
    clampToMax(max_x, max_y);
}

void PixelSort::BoundedCoordinate::clampToMax(int max_x, int max_y) {
    this->max_x = max_x;
    this->max_y = max_y;
    this->x = this->x < max_x ? this->x : max_x-1;
    this->y = this->y < max_y ? this->y : max_y-1;
    int x_max = this->x + this->width - 1;
    int y_max = this->y + this->height - 1;
    this->width = x_max < this->max_x ? this->width : this->max_x - this->x;
    this->height= y_max < this->max_y ? this->height : this->max_y - this->y;
}
