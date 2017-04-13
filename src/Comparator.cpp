
#include "Comparator.hpp"

bool PixelSort::PixelComparator::operator()(const PixelSort::Pixel& p1, const PixelSort::Pixel& p2) {
    double d1 = p1.red() + p1.green() + p1.blue();
    double d2 = p2.red() + p2.green() + p2.blue();
    return d1 < d2;
}

bool PixelSort::SumPixelComparator::operator()(const PixelSort::Pixel& p1, const PixelSort::Pixel& p2) {
    double d1 = p1.red() + p1.green() + p1.blue();
    double d2 = p2.red() + p2.green() + p2.blue();
    return d1 > d2;
}
