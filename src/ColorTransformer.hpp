/*
 * ColorTransformer.hpp
 * Defines the ColorTransformer class which converts a Magick::Color
 * to a double which can be sorted.
 * An inverse transform also has to be defined to re-convert the
 * sorted double back to a Magick::Color.
 */

#include <Magick++.h>
#include "Coordinate.hpp"

#ifndef _COLORTRANSFORMER_HPP_
#define _COLORTRANSFORMER_HPP_

/* ColorTransformer defines a the 'transform' and 'inv_transform' functions
 * which can be used by the PixelSorter functions to transform any
 * matched colors in the image block.
 */
struct ColorTransformer {
    virtual double transform(Magick::ColorRGB &color, Coordinate &coord);
    virtual void inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val);
    virtual void reset();
};

/* Basic RGB Filter implemented as ColorTransformers */
struct RedFilter : public ColorTransformer {
    virtual double transform(Magick::ColorRGB &color, Coordinate &coord);
    virtual void inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val);
    virtual void reset();
};

struct BlueFilter : public ColorTransformer {
    virtual double transform(Magick::ColorRGB &color, Coordinate &coord);
    virtual void inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val);
    virtual void reset();
};

struct GreenFilter : public ColorTransformer {
    virtual double transform(Magick::ColorRGB &color, Coordinate &coord);
    virtual void inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val);
    virtual void reset();
};

/* Asendorf Transform produces something similar to Kim Asendorf's pixelsorting
 * method. Stores the value of the previous pixel matched, and the current run number.
 */

struct AsendorfTransform : public ColorTransformer {
    int prev = 0;
    int run = 0;
    virtual double transform(Magick::ColorRGB &color, Coordinate &coord);
    virtual void inv_transform(Magick::ColorRGB &color, Coordinate &coord, double val);
    virtual void reset();
};

#endif /* _COLORTRANSFORMER_HPP_ */
