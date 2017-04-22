
#include <cmath>

#include "Matcher.hpp"

bool PixelSort::Matcher::operator() (const PixelSort::Pixel &pixel) const {
    return true;
}

PixelSort::RectangleMatcher::RectangleMatcher(BoxCoordinate bounds): bounds(bounds) {};

bool PixelSort::RectangleMatcher::operator()(const Pixel &pixel) const {
    if ((pixel.x >= bounds.x) &&
        (pixel.y >= bounds.y) &&
        (pixel.x <  bounds.x + bounds.width) &&
        (pixel.y <  bounds.y + bounds.height)) {
        return true;
    } else {
        return false;
    }
}

PixelSort::CircleMatcher::CircleMatcher(Coordinate center, double radius): center(center), radius(radius) {};

bool PixelSort::CircleMatcher::operator()(const Pixel &pixel) const {
    double xdiff = std::abs(double(pixel.x - center.x));
    double ydiff = std::abs(double(pixel.y - center.y));
    double length = std::sqrt((xdiff*xdiff) + (ydiff*ydiff));
    return length < radius;
}

PixelSort::LineMatcher::LineMatcher(PixelSort::Coordinate start, PixelSort::Coordinate end, double radius): start{start}, end{end}, radius{radius} {};

bool PixelSort::LineMatcher::operator() (const Pixel& pixel) const {
    if (end.x == start.x) {return pixel.x == start.x;}
    double diffY = double(pixel.x - start.x)*(double(end.y - start.y )/double(end.x - start.x));
    double midY = start.y + diffY;
    return (pixel.y >= midY-radius) && (pixel.y <= midY+radius);
}

bool PixelSort::ColorMatcher::operator() (const Magick::ColorRGB &color) const {
    return true;
}

bool PixelSort::AllMatcher::operator()(const Magick::ColorRGB &color) const {
    return true;
}

PixelSort::BWBandMatcher::BWBandMatcher(double th_low, double th_high):
    pool_threshold_low(th_low), pool_threshold_high(th_high) {};

bool PixelSort::BWBandMatcher::operator()(const Magick::ColorRGB &color) const {
    double colorSum = (color.red() + color.blue() + color.green()) / 3;
    return (colorSum >= pool_threshold_low) && (colorSum <= pool_threshold_high);
}

PixelSort::RGBBandMatcher::RGBBandMatcher(Magick::ColorRGB min, Magick::ColorRGB max): min(min), max(max) {};

bool PixelSort::RGBBandMatcher::operator()(const Magick::ColorRGB &color) const {
    if ((color.red() < max.red()) && (color.red() > min.red()) &&
        (color.blue() < max.blue()) && (color.blue() > min.blue()) &&
        (color.green() < max.green()) && (color.green() > min.green())) {
        return true;
    } else {
        return false;
    }
}

PixelSort::ColorThreshMatcher::ColorThreshMatcher(Magick::ColorRGB color, double radius): color{color}, radius{radius} {};

bool PixelSort::ColorThreshMatcher::operator()(const Magick::ColorRGB &color) const {
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
