
#include "Matcher.hpp"

bool PixelSort::Matcher::operator() (const PixelSort::Pixel &pixel) {
    return true;
}

PixelSort::RectangleMatcher::RectangleMatcher(BoxCoordinate bounds): bounds(bounds) {};

bool PixelSort::RectangleMatcher::operator()(const Pixel &pixel) {
    if ((pixel.x >= bounds.x) &&
        (pixel.y >= bounds.y) &&
        (pixel.x <  bounds.x + bounds.width) &&
        (pixel.y <  bounds.y + bounds.height)) {
        return true;
    } else {
        return false;
    }
}

bool PixelSort::ColorMatcher::operator() (const Magick::ColorRGB &color) {
    return true;
}

bool PixelSort::AllMatcher::operator()(const Magick::ColorRGB &color) {
    return true;
}

PixelSort::BWBandMatcher::BWBandMatcher(double th_low, double th_high):
    pool_threshold_low(th_low), pool_threshold_high(th_high) {};

bool PixelSort::BWBandMatcher::operator()(const Magick::ColorRGB &color) {
    double colorSum = (color.red() + color.blue() + color.green()) / 3;
    return (colorSum >= pool_threshold_low) && (colorSum <= pool_threshold_high);
}

PixelSort::RGBBandMatcher::RGBBandMatcher(Magick::ColorRGB min, Magick::ColorRGB max): min(min), max(max) {};

bool PixelSort::RGBBandMatcher::operator()(const Magick::ColorRGB &color) {
    if ((color.red() < max.red()) && (color.red() > min.red()) &&
        (color.blue() < max.blue()) && (color.blue() > min.blue()) &&
        (color.green() < max.green()) && (color.green() > min.green())) {
        return true;
    } else {
        return false;
    }
}

PixelSort::ColorThreshMatcher::ColorThreshMatcher(Magick::ColorRGB color, double radius): color{color}, radius{radius} {};

bool PixelSort::ColorThreshMatcher::operator()(const Magick::ColorRGB &color) {
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
