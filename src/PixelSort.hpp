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

    /* reading Image into sortable type */
    void read(PixelVector& pixels, const Magick::Image& image);
    void read(PixelVector& pixels, const Magick::Image& image, const BoxCoordinate& box);

    /* write sortable type into Image */
    void write(const PixelVector& pixels, Magick::Image& image);
    void write(const PixelVector& pixels, Magick::Image& image, const BoxCoordinate& box);

    /* Apply matcher 
     * MatcherFunc must be a PixelSort::Matcher, or a function 
     * which transforms a const PixelSort::Pixel& to a bool */
    template <typename MatcherFunc>
    void match(PixelVector& pixels, MatcherFunc func);

    /* Simple Sort pixels */
    void sort(PixelVector& pixels, PixelComparator comp);

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
