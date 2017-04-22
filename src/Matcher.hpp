/*
 * ColorMatcher.hpp
 * This file defines the ColorMatcher class with a few
 * utility Matchers which implement the class
 */

#include <vector>

#include <Magick++.h>

#include "Coordinate.hpp"

#ifndef _PIXELSORT_MATCHER_HPP_
#define _PIXELSORT_MATCHER_HPP_

namespace PixelSort {

    /* Generic Matcher */
    struct Matcher {
        virtual bool operator() (const Pixel& pixel) const;
    };

    /* Matches if ColorCoordinate within Rectangle */
    struct RectangleMatcher : public Matcher {
        RectangleMatcher(const BoxCoordinate bounds);
        virtual bool operator() (const Pixel& pixel) const;
    private:
        BoxCoordinate bounds;
    };

    /* Matches if ColorCoordinate within Circle */
    struct CircleMatcher : public Matcher {
        CircleMatcher(Coordinate center, double radius);
        virtual bool operator() (const Pixel& pixel) const;
    private:
        Coordinate center;
        double radius;
    };

    /* Matches line within rectangle */
    struct LineMatcher : public Matcher {
        LineMatcher(Coordinate start, Coordinate end, double radius);
        virtual bool operator() (const Pixel& pixel) const;
    private:
        Coordinate start;
        Coordinate end;
        double radius;
    };

    /*
     * ColorMatcher defines a function 'match' which selects pixels
     * from the input image that matches conditions
     */
    struct ColorMatcher : public Matcher {
        virtual bool operator()(const Magick::ColorRGB &color) const;
    };

    /* Matches all colors in an image */
    struct AllMatcher : public ColorMatcher {
        virtual bool operator()(const Magick::ColorRGB &color) const;
    };

    /* Constructor has two inputs to set internal state
     * of the matcher function */
    struct BWBandMatcher : public ColorMatcher {
        BWBandMatcher(double th_low, double th_high);
        virtual bool operator()(const Magick::ColorRGB &color) const;
    private:
        double pool_threshold_low;
        double pool_threshold_high;
    };

    /* Constructor has two ColorRGB inputs to set internal state
     * of the matcher function */
    struct RGBBandMatcher : public ColorMatcher {
        RGBBandMatcher(Magick::ColorRGB min, Magick::ColorRGB max);
        virtual bool operator()(const Magick::ColorRGB &color) const;
    private:
        Magick::ColorRGB min;
        Magick::ColorRGB max;
    };
    /* ColorThreshMatcher matches one specific RGB color within
     * the threshold specified by thresh */
    struct ColorThreshMatcher : public ColorMatcher {
        ColorThreshMatcher(Magick::ColorRGB color, double radius);
        virtual bool operator()(const Magick::ColorRGB &color) const;
    private:
        Magick::ColorRGB color;
        double radius;
    };
}

#endif /* _PIXELSORT_MATCHER_HPP_ */
