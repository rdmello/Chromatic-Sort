
#include <cmath>

#include "Matcher.hpp"

bool PixelSort::Matcher::operator() (const Pixel &pixel) const {
    return true;
}


bool PixelSort::GeometryMatcher::operator() (const Pixel &coord) const {
    return true;
}


PixelSort::RectangleMatcher::RectangleMatcher(BoxCoordinate bounds): bounds(bounds) {};
PixelSort::RectangleMatcher::RectangleMatcher(): bounds(15, 24, 10, 10) {};

bool PixelSort::RectangleMatcher::operator()(const Pixel &coord) const {
    if ((coord.x >= bounds.x) &&
        (coord.y >= bounds.y) &&
        (coord.x <=  bounds.x + bounds.width) &&
        (coord.y <=  bounds.y + bounds.height)) {
        return true;
    } else {
        return false;
    }
}


PixelSort::CircleMatcher::CircleMatcher(Coordinate center, double radius): center(center), radius(radius) {};
PixelSort::CircleMatcher::CircleMatcher(): center{15, 24}, radius(10) {};

bool PixelSort::CircleMatcher::operator()(const Pixel &coord) const {
    double xdiff = std::abs(double(coord.x - center.x));
    double ydiff = std::abs(double(coord.y - center.y));
    double length = std::sqrt((xdiff*xdiff) + (ydiff*ydiff));
    return length < radius;
}


PixelSort::LineMatcher::LineMatcher(Coordinate start, Coordinate end, double radius): 
    start{start}, end{end}, radius{radius} {};
PixelSort::LineMatcher::LineMatcher(): start{15, 24}, end{25, 25}, radius(10) {};

bool PixelSort::LineMatcher::operator() (const Pixel& coord) const {
    /* Build coordinates into Ax + By + C = 0 */
    double ydiff = end.y - start.y;
    double xdiff = end.x - start.x;

    double dist = std::abs((ydiff*coord.x) - (xdiff*coord.y) + (end.x*start.y) - (start.x*end.y)) / std::sqrt((ydiff*ydiff)+(xdiff*xdiff));

    return dist <= radius;
}


bool PixelSort::ColorMatcher::operator() (const Pixel &color) const {
    return true;
}

bool PixelSort::AllMatcher::operator()(const Pixel &color) const {
    return true;
}


PixelSort::BWBandMatcher::BWBandMatcher(uint8_t th_low, uint8_t th_high):
    pool_threshold_low(th_low), pool_threshold_high(th_high) {};

bool PixelSort::BWBandMatcher::operator()(const Pixel &color) const {
    uint8_t colorSum = static_cast<uint8_t>((uint16_t(color.red()) + uint16_t(color.blue()) + uint16_t(color.green())) / 3);
    return (colorSum >= pool_threshold_low) && (colorSum <= pool_threshold_high);
}


PixelSort::RGBBandMatcher::RGBBandMatcher(RGBAColor min, RGBAColor max): 
rMin(min.red()), gMin(min.green()), bMin(min.blue()), 
rMax(max.red()), gMax(max.green()), bMax(max.blue()) {};

bool PixelSort::RGBBandMatcher::operator()(const Pixel &color) const {
    if ((color.red() <= rMax) && (color.red() >= rMin) &&
        (color.blue() <= bMax) && (color.blue() >= bMin) &&
        (color.green() <= gMax) && (color.green() >= gMin)) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}


PixelSort::ColorThreshMatcher::ColorThreshMatcher(RGBAColor color, double radius): color{color}, radius{radius} {};

bool PixelSort::ColorThreshMatcher::operator()(const Pixel &color) const {
    if ((color.red() < this->color.red() + radius) &&
        (color.red() > this->color.red() - radius) &&
        (color.blue() < this->color.blue() + radius) &&
        (color.blue() > this->color.blue() - radius) &&
        (color.green() < this->color.green() + radius) &&
        (color.green() > this->color.green() - radius)) {
        return true;
    } else {
        return false;
    }
};
