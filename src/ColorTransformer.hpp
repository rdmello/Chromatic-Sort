/*
 * ColorTransformer.hpp
 * Defines the ColorTransformer class which converts a Magick::Color
 * to a double which can be sorted.
 * An inverse transform also has to be defined to re-convert the
 * sorted double back to a Magick::Color.
 */

#include <Magick++.h>

#ifndef _COLORTRANSFORMER_HPP_
#define _COLORTRANSFORMER_HPP_

/* ColorTransformer defines a the 'transform' and 'inv_transform' functions
 * which can be used by the PixelSorter functions to transform any 
 * matched colors in the image block.
 */
struct ColorTransformer {
    virtual double transform(Magick::ColorRGB &color);
    virtual void inv_transform(Magick::ColorRGB &color, double val);
};

/* Basic RGB Filter implemented as ColorTransformers */
struct RedFilter : public ColorTransformer {
    virtual double transform(Magick::ColorRGB &color);
    virtual void inv_transform(Magick::ColorRGB &color, double val);
};

struct BlueFilter : public ColorTransformer {
    virtual double transform(Magick::ColorRGB &color);
    virtual void inv_transform(Magick::ColorRGB &color, double val);
};

struct GreenFilter : public ColorTransformer {
    virtual double transform(Magick::ColorRGB &color);
    virtual void inv_transform(Magick::ColorRGB &color, double val);
};

#endif /* _COLORTRANSFORMER_HPP_ */

