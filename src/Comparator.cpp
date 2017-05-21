
#include "Comparator.hpp"

bool PixelSort::Comparator::operator()(const Pixel& p1, const Pixel& p2) {
    double d1 = p1.red() + p1.green() + p1.blue();
    double d2 = p2.red() + p2.green() + p2.blue();
    return d1 < d2;
}

bool PixelSort::SumPixelComparator::operator()(const Pixel& p1, const Pixel& p2) {
    double d1 = p1.red() + p1.green() + p1.blue();
    double d2 = p2.red() + p2.green() + p2.blue();
    return d1 > d2;
}

PixelSort::WeightedComparator::WeightedComparator(double rX, double gX, double bX):
    rX(rX), gX(gX), bX(bX) {};

bool PixelSort::WeightedComparator::operator()(const Pixel& p1, const Pixel& p2) {
    double total1 = (rX*p1.red()) + (gX*p1.green()) + (bX*p1.blue());
    double total2 = (rX*p2.red()) + (gX*p2.green()) + (bX*p2.blue());
    return total1 >= total2;
}