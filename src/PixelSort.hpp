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

    using MatchFcn = bool (*)(const Pixel&);
    using CompareFcn = bool (*)(const Pixel&, const Pixel&);
    using ApplyFcn = Pixel (*)(const Pixel&, const Pixel&);

    /* GenerateFcn is a function which accepts an empty std::vector<Pixel>
     * and returns the Pixels matched 
     */
    // using GenerateFcn = void (*)(std::vector<Pixel>& pixels, const BoxCoordinate& box);

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
            
            /* CONSTRUCTOR from PV::iter [first, last) */
            PixelVector(const PixelVector& pv, std::vector<Pixel>::iterator first, 
                        std::vector<Pixel>::iterator last); 
            
            /* CONSTRUCTOR uses generator fcn to read part of Image into sortable type */
            PixelVector(Magick::Image& image, const BoxCoordinate& box, 
                        const GeometryMatcher& matcher); 

            PixelVector(const PixelVector& pv, int start, int end);

            /* Writes PixelVector to Magick::Image */
            void sync();

            /* Apply matcher 
             * MatcherFunc must be a PixelSort::Matcher, or a function 
             * which transforms a const PixelSort::Pixel& to a bool */
            void match(const Matcher& matcher);
            void match(const MatchFcn& matcher);

            /* Simple Sort pixels */
            void sort(const Comparator& comparator);
            void sort(const CompareFcn& compare);
            
            /* Unstable Sort pixels */
            void unstable_sort(const Comparator& comparator);
            void unstable_sort(const CompareFcn& compare);
 
            /* Apply unary/binary function on this PixelVector */
            void apply(void (*func)(Pixel& p));
            void apply(const PixelVector&, const ApplyFcn& applyfcn);

    };

    /* Apply pixelsort operation across block in an image */
    template <typename T1 = Matcher, typename T2 = Comparator>
    void BlockPixelSort(Magick::Image& image, Coordinate blocksize, 
                        const T1& match, const T2& compare, 
                        const ApplyFcn& applyfcn);

    /* Apply pixelsort operation across block in an image */
    template <typename T1 = Matcher, typename T2 = Comparator>
    void AsendorfSort(PixelVector& pv, const T1& match, const T2& compare, 
                      const ApplyFcn& applyfcn);

    /* writeColor is a utility function to help convert and write
     * a Magick::Color to a Quantum triplet
     */
    void writeColor(const Magick::Color& color, Magick::Quantum* location);

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
