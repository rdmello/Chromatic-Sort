
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

    struct SumPixelComparator : public Comparator {
        virtual bool operator()(const Pixel& p1, const Pixel& p2);
    };

}

#endif /* _PIXELSORT_COMPARATOR_HPP_ */
