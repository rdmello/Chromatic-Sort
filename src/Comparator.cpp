
#include "Comparator.hpp"

bool PixelSort::Comparator::operator()(const Pixel& p1, const Pixel& p2) const {
    double d1 = p1.red() + p1.green() + p1.blue();
    double d2 = p2.red() + p2.green() + p2.blue();
    return d1 < d2;
}

bool PixelSort::SumPixelComparator::operator()(const Pixel& p1, const Pixel& p2) const {
    double d1 = p1.red() + p1.green() + p1.blue();
    double d2 = p2.red() + p2.green() + p2.blue();
    return d1 > d2;
}

PixelSort::WeightedComparator::WeightedComparator(double redX, double greenX, double blueX):
    redX(redX), greenX(greenX), blueX(blueX) {};

bool PixelSort::WeightedComparator::operator()(const Pixel& p1, const Pixel& p2) const {
    double total1 = (redX*p1.red()) + (greenX*p1.green()) + (blueX*p1.blue());
    double total2 = (redX*p2.red()) + (greenX*p2.green()) + (blueX*p2.blue());
    return total1 < total2;
}