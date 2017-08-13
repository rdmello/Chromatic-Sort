
/*
 * Comparator.hpp
 * Defines comparator for std::sort with PixelVector
 */

#include "Coordinate.hpp"

#ifndef _PIXELSORT_COMPARATOR_HPP_
#define _PIXELSORT_COMPARATOR_HPP_

namespace PixelSort {

    struct Comparator {
        virtual bool operator()(const Pixel& p1, const Pixel& p2) const;
    };

    /* Adds R, G, and B components and then compares */
    struct SumPixelComparator : public Comparator {
        bool operator()(const Pixel& p1, const Pixel& p2) const;
    };

    struct WeightedComparator : public Comparator {
        WeightedComparator(double redX, double greenX, double blueX); 
        bool operator()(const Pixel& p1, const Pixel& p2) const;
        private:
        double redX;
        double greenX;
        double blueX;
    };

    /* The angle comparator can reorder a pixelvector 
     * in a way that makes it seem like the individual pixels are rotated.
     * Provide an angle argument (in degrees) when constructing.
     * Angles are measured counter-clockwise from the horizontal-right
     */
    struct AngleComparator : public Comparator {
        AngleComparator();
        AngleComparator(double angle);
        void setAngle(double angle);
        bool operator()(const Pixel& p1, const Pixel& p2) const;
    private:
        double angle;
    };

}

#endif /* _PIXELSORT_COMPARATOR_HPP_ */
