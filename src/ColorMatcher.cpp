
#include "ColorMatcher.hpp"

bool ColorMatcher::match(Magick::ColorRGB &color) {
    return true;
}

bool AllMatcher::match(Magick::ColorRGB &color) {
    return true;
}

bool CoolMatcher::match(Magick::ColorRGB &color) {
    return color.red() > 0.5 || color.blue() > 0.5 || color.green() > 0.5;
}

PooledMatcher::PooledMatcher(double th_low, double th_high) {
    this->pool_threshold_high = th_high;
    this->pool_threshold_low = th_low;
}

bool PooledMatcher::match(Magick::ColorRGB &color) {
    double colorSum = (color.red() + color.blue() + color.green()) / 3;
    return (colorSum >= this->pool_threshold_low) && (colorSum <= this->pool_threshold_high);
}

ColorThreshMatcher::ColorThreshMatcher(Magick::ColorRGB color, double radius)
    : color{color}, radius{radius} {};

bool ColorThreshMatcher::match(Magick::ColorRGB &color) {
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


