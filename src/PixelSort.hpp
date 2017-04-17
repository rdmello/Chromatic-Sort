/*
 * PixelSort.hpp
 * Fundamental pixel sort algorithms and helpers
 */

#include <vector>
#include <iostream>

#include <Magick++.h>

#include "Coordinate.hpp"
#include "Matcher.hpp"
#include "Comparator.hpp"

#ifndef _PIXELSORT_PIXELSORT_HPP_
#define _PIXELSORT_PIXELSORT_HPP_

namespace PixelSort {

    /* PixelVector is the main class which maintains a vector of Pixel objects
     * and provides a set of methods to update pixels and keep them in sync
     * with the Magick::Image object
     */
    class PixelVector {
        private:
            Magick::Image& image;
            BoxCoordinate box;
        public:

            std::vector<Pixel> pixels;
            /* CONSTRUCTOR reading Image into sortable type */
            PixelVector(Magick::Image& image, const BoxCoordinate& box); 
            PixelVector(const PixelVector& pv, int start, int end);

            /* Writes PixelVector to Magick::Image */
            void sync();
    
            /* Apply matcher 
            * MatcherFunc must be a PixelSort::Matcher, or a function 
            * which transforms a const PixelSort::Pixel& to a bool */
            // void match(const Matcher& matcher);
            void match(Matcher& matcher);
            void match(bool (*matcher)(const Pixel&));

            /* Simple Sort pixels */
            void sort(const PixelComparator& comparator);
            void sort(bool (*comparator)(const Pixel&, const Pixel&));
            
            /* Unstable Sort pixels */
            void unstable_sort(const PixelComparator& comparator);
            void unstable_sort(bool (*comparator)(const Pixel&, const Pixel&));
 
            /* Apply unary/binary function on this PixelVector */
            void apply(void (*func)(Pixel& p));
            void apply(const PixelVector&, Pixel (*func)(const Pixel& p1, const Pixel& p2));
    };

    /* writeColor is a utility function to help convert and write
     * a Magick::Color to a Quantum triplet
     */
    void writeColor(Magick::Color color, Magick::Quantum* location);

    /* SortDirection specifies if the sort should be increasing or decreasing
     * in a specific direction
     */
    enum class SortDirection{Inc, Dec};

    /* Outerloop provides a way of distinguishing between sorts that
     * iterate over the y-direction, and ones that iter over x
     */
    enum class OuterLoop{X, Y};
}

#endif /* _PIXELSORT_PIXELSORT_HPP_ */
