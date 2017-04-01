/*
 * PixelSorter.hpp
 * Fundamental pixel sort algorithms and helpers
 */

#include <Magick++.h>

#include "Coordinate.hpp"
#include "ColorMatcher.hpp"
#include "ColorTransformer.hpp"

#ifndef _PIXELSORTER_HPP_
#define _PIXELSORTER_HPP_

/* writeColor is a utility function to help convert and write 
 * a Magick::Color to a Quantum triplet
 */
void writeColor(Magick::Color color, Magick::Quantum* location);

/* colorPixels is a debugging function, which is sligly outdated now. 
 * update if you use it 
 */
void colorPixels(Magick::Image& image, int x, int y, int runWidth, int runLength);
    
/* SortDirection specifies if the sort should be increasing or decreasing
 * in a specific direction
 */
enum class SortDirection{Inc, Dec};

/* Outerloop provides a way of distinguishing between sorts that 
 * iterate over the y-direction, and ones that iter over x
 */
enum class OuterLoop{X, Y};

/* sortPixels is the main sort method.*/
void sortPixels(Magick::Image& image, int x, int y, int runWidth, int runLength, 
        ColorMatcher &cm, ColorTransformer &ct, 
        SortDirection SDx, SortDirection SDy, OuterLoop ol);

/* sortPixelBlocks divides an image up into blocks within a border area, 
 *      and sorts within that border.
 */
void sortPixelBlocks(Magick::Image& image, int borderx, int bordery,
        int borderwidth, int borderheight, 
        int x, int y, int runWidth, int runHeight, 
        ColorMatcher &cm, ColorTransformer &ct, 
        SortDirection sx, SortDirection sy, OuterLoop ol);

#endif /* _PIXELSORTER_HPP_ */

