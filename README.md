
# C++ PixelSorting Library

## Goal

Provide a simple, easy-to-use, and performant C++ pixelsorting library

## Todos

1. Add another statusbar entry for image name, dimensions

2. Fix sort rotation selector to allow for more angles

3. Add ability to interface with QImage instead of Magick::Image. This will allow the GUI to be built and packaged in QtCreator, which will greatly help cross-platform deployment

4. Use colorpickers instead of three range sliders for min/max matchers

5. Add Zoom and pan controls to the image display

## Building

1. Install the ImageMagick and Magick++ libraries on your system. You must be able to use the `magick++-config` command on your command line.

2. Download or clone this project's source code using 
```
    git clone https://github.com/rdmello/PixelSort.git
```

3. Run `make all` and `make run file=interactive` from this directory

## Design

The PixelSorting interface in this code has gone through a large number of iterations to ensure that the resulting library is both generic and powerful.

Here is an overview of the interface:

1. An image can be read using the `Magick::Image` class. Then, it can be converted to a `PixelVector` which is ready for sort operations.

2. This Coordinate array is filtered and reduced using the `ColorMatcher` and `GeometryMatcher` subclasses.  

3. The `PixelVector` is then copied and the copy is sorted using `PixelSorter`. A `PixelSort::Comparator` can also be used to refine the sort.

4. The new sorted `PixelVector` is then zipped with the original unsorted `PixelVector`.

5. The resulting `PixelVector` is written back to the `Magick::Image` which then outputs back to the image file on your filesystem.

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

## Command-line interface

The PixelSort library should be accessible from a command-line interface. This interface is detailed below:

1. The command used to launch the pixelsorting application is: 
    "$ pixelsort inputfilename.png/tiff/gif 
