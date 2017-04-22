
/* Testing Asendorf sort */

#include <iostream>
#include <string>

#include <Magick++.h>

#include "../src/PixelSort.hpp"

using namespace PixelSort;

int main() {
    /* File definitions */
    std::string filename = "images/night";
    std::string fileext = "tiff";
    std::string filein = filename + "." + fileext; // define input filename
    std::string fileout = filename + "_out" + "." + fileext; // define output filename

    /* ImageMagick read image */
    Magick::Image image(filein); // read image

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "IMAGE SIZE: " << image.columns() << "x" << image.rows() << std::endl;

    /* Start PixelSorting */
    image.modifyImage();
    image.type(Magick::TrueColorType);

    int w = 128;
    int h = 128;
    Coordinate block(w, h);

    BlockPixelSort<Matcher, CompareFcn>(image, block, 
                    LineMatcher(Coordinate(0, 0), Coordinate(w-1, h-1), 8), 
                    [](const Pixel& p1, const Pixel& p2) {
                        return p1.red() < p2.red(); 
                    }, 
                    [](const Pixel& p1, const Pixel& p2) {
                        Pixel p(p1);
                        p.red(p2.blue());
                        p.blue(p2.red());
                        return p;
                });
 
    BlockPixelSort<Matcher, CompareFcn>(image, block, 
                    LineMatcher(Coordinate(w-1, 0), Coordinate(0, h-1), 8), 
                    [](const Pixel& p1, const Pixel& p2) {
                        return p1.blue() < p2.blue(); 
                    }, 
                    [](const Pixel& p1, const Pixel& p2) {
                        Pixel p(p1);
                        p.blue(p2.red());
                        p.green(p2.blue());
                        p.red(p2.green());
                        return p;
                });
 
    BlockPixelSort<Matcher, Comparator>(image, Coordinate(8, 8), AllMatcher(), 
                   SumPixelComparator(), [](const Pixel& p1, const Pixel& p2) {
        Pixel p(p1, p2);
        return p;
    });
    
    image.write(fileout);
    std::cout << "Written image" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    return 0;
}