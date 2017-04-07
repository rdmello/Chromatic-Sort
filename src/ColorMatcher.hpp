/*
 * ColorMatcher.hpp
 * This file defines the ColorMatcher class a few 
 * utility Matchers which implement the class
 */

#include <Magick++.h>

#ifndef _COLORMATCHER_HPP_
#define _COLORMATCHER_HPP_

/* ColorMatcher defines a function 'match' which selects pixels
 * from the input image that matches conditions
 */
struct ColorMatcher {
    virtual bool match(Magick::ColorRGB &color);
};

/* Matches all colors in an image */
struct AllMatcher : public ColorMatcher {
    virtual bool match(Magick::ColorRGB &color);    
};

/* Debug function, matches half of the RGB colorspace */
struct CoolMatcher : public ColorMatcher {
    virtual bool match(Magick::ColorRGB &color);    
};

/* Constructor has two inputs to set internal state 
 * of the matcher function */
struct PooledMatcher : public ColorMatcher {
    PooledMatcher(double th_low, double th_high);
    virtual bool match(Magick::ColorRGB &color);
private:
    double pool_threshold_high;
    double pool_threshold_low;
};

/* ColorThreshMatcher matches one specific RGB color within
 * the threshold specified by thresh */
struct ColorThreshMatcher : public ColorMatcher {
    ColorThreshMatcher(Magick::ColorRGB color, double radius);
    virtual bool match(Magick::ColorRGB &color);
private:
    Magick::ColorRGB color;
    double radius;
};

#endif /* _COLORMATCHER_HPP_ */

