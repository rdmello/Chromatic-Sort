
/*
 * Comparator.hpp
 * Defines comparator for std::sort with PixelVector
 */

#include <Magick++.h>

#include "Coordinate.hpp"

#ifndef _PIXELSORT_COMPARATOR_HPP_
#define _PIXELSORT_COMPARATOR_HPP_

namespace PixelSort {

    struct Comparator {
        virtual bool operator()(const Pixel& p1, const Pixel& p2);
    };

    /* Adds R, G, and B components and then compares */
    struct SumPixelComparator : public Comparator {
        virtual bool operator()(const Pixel& p1, const Pixel& p2);
    };

    struct WeightedComparator : public Comparator {
        WeightedComparator(double rX, double gX, double bX); 
        virtual bool operator()(const Pixel& p1, const Pixel& p2);
        private:
        double rX;
        double gX;
        double bX;
    };

}

#endif /* _PIXELSORT_COMPARATOR_HPP_ */
