
#include "ColorTransformer.hpp"

double ColorTransformer::transform(Magick::ColorRGB &color) {
    return color.red();
}
void ColorTransformer::inv_transform(Magick::ColorRGB &color, double val) {
    color.red(val);
}

double RedFilter::transform(Magick::ColorRGB &color) {
    return color.red();
}
void RedFilter::inv_transform(Magick::ColorRGB &color, double val) {
    color.red(val);
}

double BlueFilter::transform(Magick::ColorRGB &color) {
    return color.blue();
}
void BlueFilter::inv_transform(Magick::ColorRGB &color, double val) {
    color.blue(val);
}

double GreenFilter::transform(Magick::ColorRGB &color) {
    return color.green();
}
void GreenFilter::inv_transform(Magick::ColorRGB &color, double val) {
    color.green(val);
}


