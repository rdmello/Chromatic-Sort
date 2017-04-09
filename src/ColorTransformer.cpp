
#include "ColorTransformer.hpp"


double ColorTransformer::transform(Magick::ColorRGB &color, Coordinate &coord) {
    return color.red();
}
void ColorTransformer::inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val) {
    color.red(val);
}
void ColorTransformer::reset() {
}


double RedFilter::transform(Magick::ColorRGB &color, Coordinate &coord) {
    return color.red();
}
void RedFilter::inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val) {
    color.red(val);
}
void RedFilter::reset() {
}


double BlueFilter::transform(Magick::ColorRGB &color, Coordinate &coord) {
    return color.blue();
}
void BlueFilter::inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val) {
    color.blue(val);
}
void BlueFilter::reset() {
}


double GreenFilter::transform(Magick::ColorRGB &color, Coordinate &coord) {
    return color.green();
}
void GreenFilter::inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val) {
    color.green(val);
}
void GreenFilter::reset() {
}


double AsendorfTransform::transform(Magick::ColorRGB &color, Coordinate &coord) {
    double val = 0;
    if ((coord.x - 3 == prev) || (coord.x == prev)) {
    } else {
        run += 1;
    }
    val = run + color.red();
    prev = coord.x;
    return val;
}

void AsendorfTransform::inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val) {
    if ((coord.x - 3 == prev) || (coord.x == prev)) {
    } else {
        run += 1;
    }
    color.red(val - run);
    prev = coord.x;
}
void AsendorfTransform::reset() {
    prev = 0;
    run = 0;
}
