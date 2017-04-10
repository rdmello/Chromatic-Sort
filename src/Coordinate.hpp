
#include <vector>

#include <Magick++.h>

/*
 * Coordinate.hpp
 * The Coordinate class and its subclasses provide
 * some simple structures to make it easier to work with Pixels
 * Colors, and boundaries.
 */

#ifndef _PIXELSORT_COORDINATE_HPP_
#define _PIXELSORT_COORDINATE_HPP_

namespace PixelSort {

    /* Store x, y coordinates in convenient way */
    struct Coordinate {
        int x, y;
        Coordinate(int x, int y);
    };

    /* Store x, y coordinates with Magick::Color in a convenient way */
    struct ColorCoordinate : public Coordinate, public Magick::ColorRGB {
        ColorCoordinate(const Coordinate& coord, const Magick::ColorRGB& color);
    };

    struct BoxCoordinate : public Coordinate {
        int width, height;
        BoxCoordinate(int x, int y, int width, int height);
    };

    /* Keeps x, y, w, h clamped to limits */
    struct BoundedCoordinate : public BoxCoordinate {
        int max_x, max_y;
        BoundedCoordinate(int x, int y, int width, int height, int max_x, int max_y);
        void clampToMax(int max_x, int max_y);
    };

    /* We'll be using Pixel a lot */
    typedef PixelSort::ColorCoordinate Pixel;

    /* Main storage type */
    typedef std::vector<Pixel> PixelVector;

}

#endif /* _PIXELSORT_COORDINATE_HPP_ */
