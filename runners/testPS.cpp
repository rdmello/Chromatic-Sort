
/* Testing Asendorf sort */

#include <iostream>
#include <string>
#include <cmath>

#include <Magick++.h>

#include "../src/PixelSort.hpp"

using namespace PixelSort;

int main() {
    /* File definitions */
    std::string filename = "images/earth";
    std::string fileext = "png";
    std::string filein = filename + "." + fileext; // define input filename
    std::string fileout = filename + "_out" + "." + fileext; // define output filename

    /* ImageMagick read image */
    Magick::Image image(filein); // read image

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "IMAGE SIZE: " << image.columns() << "x" << image.rows() << std::endl;

    /* Start PixelSorting */
    image.modifyImage();
    image.type(Magick::TrueColorType);

    // int lineWidth = 160;
    // int delta = 800;
    int lineWidth = 50;
    int delta = 200;

    for (int ix = -15; ix < 16; ++ix) {
        std::cout << "Operation " << ix << std::endl;
        PixelVector pv(image, BoxCoordinate(0, 0, image.columns(), image.rows()), 
            LineMatcher(Coordinate(0, 0+(ix*delta)), 
                Coordinate(image.columns()/2, (image.rows()/2)+(ix*delta)), lineWidth));
        pv.sort([](const Pixel& p1, const Pixel& p2){
            double theta = 0.7854;
            double val1 = (p1.x*std::cos(theta)) + (p1.y*std::sin(theta));
            double val2 = (p2.x*std::cos(theta)) + (p2.y*std::sin(theta));
            return val1 < val2;
        });
        AsendorfSort<Matcher, Comparator>(pv, BWBandMatcher(0.05, 0.7), 
            WeightedComparator(0, 0, 1), 
            [](const Pixel& p1, const Pixel& p2) {
                Pixel p(p1); 
                p.red(p2.blue());
                return p;
        });
    }
   
    image.write(fileout);
    std::cout << "Written image" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    return 0;
}