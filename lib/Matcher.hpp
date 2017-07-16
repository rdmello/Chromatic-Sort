/*
 * ColorMatcher.hpp
 * This file defines the ColorMatcher class with a few
 * utility Matchers which implement the class
 */

#include <vector>

#include "Coordinate.hpp"

#ifndef _PIXELSORT_MATCHER_HPP_
#define _PIXELSORT_MATCHER_HPP_

namespace PixelSort {

    /* Generic Matcher */
    struct Matcher {
        virtual bool operator() (const Pixel& pixel) const;
    };

    /* Geometry Matcher is the parent class for matchers which only
     * need geometry information to match
     */
    struct GeometryMatcher : public Matcher {
        virtual bool operator() (const Pixel& coord) const;
    };

 
    /* Matches if ColorCoordinate within Rectangle */
    struct RectangleMatcher : public GeometryMatcher {
        RectangleMatcher();
        RectangleMatcher(BoxCoordinate bounds);
        virtual bool operator() (const Pixel& coord) const;

        BoxCoordinate bounds;
    };

    /* Matches if ColorCoordinate within Circle */
    struct CircleMatcher : public GeometryMatcher {
        CircleMatcher();
        CircleMatcher(Coordinate center, double radius);
        virtual bool operator() (const Pixel& coord) const;

        Coordinate center;
        double radius;
    };

    /* Matches line within rectangle */
    struct LineMatcher : public GeometryMatcher {
        LineMatcher();
        LineMatcher(Coordinate start, Coordinate end, double radius);
        virtual bool operator() (const Pixel& coord) const;

        Coordinate start;
        Coordinate end;
        double radius;
    };

    /*
     * ColorMatcher defines a function 'match' which selects pixels
     * from the input image that matches conditions
     */
    struct ColorMatcher : public Matcher {
        virtual bool operator()(const Pixel &color) const;
    };

    /* Matches all colors in an image */
    struct AllMatcher : public ColorMatcher {
        virtual bool operator()(const Pixel &color) const;
    };

    /* Constructor has two inputs to set internal state
     * of the matcher function */
    struct BWBandMatcher : public ColorMatcher {
        BWBandMatcher(double th_low, double th_high);
        virtual bool operator()(const Pixel &color) const;
    private:
        double pool_threshold_low;
        double pool_threshold_high;
    };

    /* Constructor has two ColorRGB inputs to set internal state
     * of the matcher function */
    struct RGBBandMatcher : public ColorMatcher {
        RGBBandMatcher(RGBColor min, RGBColor max);
        virtual bool operator()(const Pixel &color) const;
    private:
        RGBColor min;
        RGBColor max;
    };
    /* ColorThreshMatcher matches one specific RGB color within
     * the threshold specified by thresh */
    struct ColorThreshMatcher : public ColorMatcher {
        ColorThreshMatcher(RGBColor color, double radius);
        virtual bool operator()(const Pixel &color) const;
    private:
        RGBColor color;
        double radius;
    };
}

#endif /* _PIXELSORT_MATCHER_HPP_ */
