
# C++ PixelSorting

## Goals

1. A thin interface to ImageMagick libraries for PixelSorting

2. Encapsulating common workflows in a generic interface

3. Ensuing that performance and accuracy are maintained

## Building

1. Install the ImageMagick and Magick++ libraries on your system. You must be able to use the `magick++-config` command on your command line.

2. Run "make all" and "make run file=test" from this directory

## Design

The PixelSorting interface in this code has gone through a large number of iterations to ensure that the resulting library is both generic and powerful.

Here is an overview of the interface:

1. The pixelbuffer from an image (or GIF frame) is converted to a 2D  `Coordinate` array (see `src/Coordinate.hpp`).

2. This Coordinate array is filtered and reduced using the `ColorMatcher` and `GeometryMatcher` subclasses.  

3. The filtered Coordinate array is then copied and the copy is sorted using `PixelSorter`. A `PixelSortComparator` can also be used to refine the sort.

4. The sorted Coordinate array is written back to the Image using the `ColorMapper` subclasses. Here, the whole color, or a component of a color can be defined to be written back to the Image.

## Code Structure

Classes:

* PixelSort::Matcher: AllMatcher, ColorMatcher, BWBandMatcher, RGBBandMatcher, RectangleMatcher, CircleMatcher, LineMatcher

* PixelSort::Coordinate: Coordinate, ColorCoordinate, BoxCoordinate, BoundedCoordinate

* PixelSort::Comparator: SumComparator, RedComparator, GreenComparator, BlueComparator

* PixelSort::Mapper: AllMapper, RedMapper, GreenMapper, BlueMapper

Functions:

* void PixelSort::readImageToColorCoordinateList(
        Magick::Image& image,
        std::vector<PixelSort::ColorCoordinate>& pixels
    )

* PixelSort::writeColorCoordinateListToImage(
        std::vector<PixelSort::ColorCoordinate>& pixels,
        Magick::Image& image
    )

* PixelSort::transformColorCoordinates(
        std::vector<PixelSort::ColorCoordinate>& pixels,
        PixelSort::BlockSortDirection& dir
    )

Main user-facing operations:

1. void PixelSort::ApplyMatcher(
        std::vector<PixelSort::ColorCoordinate>& pixels,
        PixelSort::Matcher& matcher
    )
    Converts a Magick::Image to a black/white image based on whether each pixel
    was matched, or if the match failed.

2. void PixelSort::ApplyMapper(
        std::vector<PixelSort::ColorCoordinate>&  in_pixels,
        std::vector<PixelSort::ColorCoordinate>& out_pixels,
        PixelSort::Mapper& mapper
    )
    Converts a Magick::Image to a black/white image based on whether each pixel
    was matched, or if the match failed.

3. void PixelSort::Sort(
        std::vector<PixelSort::ColorCoordinate>& pixels,
        PixelSort::Comparator& comp
    );
    Carries out a naive block-based pixelsort

4. void PixelSort::AsendorfSort(
        std::vector<PixelSort::ColorCoordinate>& pixels,
        PixelSort::Comparator& comp,
        PixelSort::Matcher& matcher
    );
    Does an Asendorf-style sort. Needs a Matcher since AsendorfSort does internal matching

## TBD

1. Refactor Magick++ Image methods to a separate abstract function so that
other ImProc libraries can be used easily.
