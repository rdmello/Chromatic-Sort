
/* Testing Asendorf sort */

#include <iostream>
#include <string>

#include <Magick++.h>

#include "../src/PixelSort.hpp"

using namespace PixelSort;

int main() {
    /* File definitions */
    std::string filename = "images/expo";
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

    int w = 50;
    int h = 50;
    for (int j(0); j < image.rows(); j+=h) {
        for (int i(0); i < image.columns(); i+=w) {
            /* Define region-of-interest and make PixelVector */
            BoundedCoordinate box(i, j, w, h, image.columns(), image.rows());
            PixelVector pv(image, box);

            /* Change sort direction to work in the y-direction */
            pv.sort([](const Pixel& p1, const Pixel& p2) {
                return p1.x < p2.x;
            });

            /* Define and apply a circular matcher */
            CircleMatcher mat(Coordinate(i+(w/2), j+(h/2)), 15);
            pv.match(mat);

            /* Sort the vector into a new vector */
            PixelVector pv2 = pv;
            pv2.sort([](const Pixel& p1, const Pixel& p2) {
                return p1.red() < p2.red();
            });

            /* Combine the new sorted vector into the unsorted one */ 
            pv.apply(pv2, [](const Pixel& p1, const Pixel& p2) {
                Pixel p = p1;
                p.red(p2.red());
                return p;
            });

            /* Write to image */
            pv.sync();
        }
    }
    
    image.write(fileout);
    std::cout << "Written image" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    return 0;
}