
#include "Comparator.hpp"
#include <cmath>

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

PixelSort::BoolWeightedComparator::BoolWeightedComparator(bool r, bool g, bool b):
    redX(r), greenX(g), blueX(b) {};

bool PixelSort::BoolWeightedComparator::operator()(const Pixel& p1, const Pixel& p2) const {
    uint16_t total1 = (redX ? p1.red() : 0) + (greenX ? p1.green() : 0) + (blueX ? p1.blue() : 0);
    uint16_t total2 = (redX ? p2.red() : 0) + (greenX ? p2.green() : 0) + (blueX ? p2.blue() : 0);
    return total1 < total2;
}

PixelSort::AngleComparator::AngleComparator(): angle(0) {};
PixelSort::AngleComparator::AngleComparator(double angle) { setAngle(angle); };

void PixelSort::AngleComparator::setAngle(double angle) 
{ 
    this->angle = 3.14159 * (angle + 90.0) / 180.0; 
}

bool PixelSort::AngleComparator::operator()(const Pixel& p1, const Pixel& p2) const 
{
    double val1 = (p1.x*std::cos(angle)) + (p1.y*std::sin(angle));
    double val2 = (p2.x*std::cos(angle)) + (p2.y*std::sin(angle));
    return val1 < val2;
}
