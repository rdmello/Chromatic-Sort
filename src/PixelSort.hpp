/*
 * PixelSort.hpp
 * Fundamental pixel sort algorithms and helpers
 */

#include <vector>

#include <Magick++.h>

#include "Coordinate.hpp"
#include "Matcher.hpp"
#include "Comparator.hpp"

#ifndef _PIXELSORT_PIXELSORT_HPP_
#define _PIXELSORT_PIXELSORT_HPP_

namespace PixelSort {

    /* reading Image into sortable type */
    void readImageToPixelVector(Magick::Image& image, PixelVector& pixels);

    /* write sortable type into Image */
    void writePixelVectorToImage(const PixelVector& pixels, Magick::Image& image);

    /* Apply matcher */
    void ApplyMatcher(PixelVector& pixels, Matcher matcher);

    /* Simple Sort pixels */
    void Sort(PixelVector& pixels, const PixelComparator& comp);

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
