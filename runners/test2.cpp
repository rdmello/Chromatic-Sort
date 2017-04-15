
/*
 * test2.cpp: A test for V2 of the PixelSort library
 */

/* Standard Library includes */
#include <iostream>

/* Including Magick++ image processing library */
#include <Magick++.h>

/* Local includes */
#include "../src/PixelSort.hpp"

/* Forward declaration for REPL function used later */
void doREPL(Magick::Image& image);

int main () {

    /* File definitions */
    std::string filename = "images/pyramid";
    std::string fileext = "tiff";
    std::string filein = filename + "." + fileext; // define input filename
    std::string fileout = filename + "_out" + "." + fileext; // define output filename

    /* ImageMagick image manipulation */
    Magick::Image img(filein); // read image

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Starting REPL, Ctrl+C to exit" << std::endl;
    std::cout << "IMAGE SIZE: " << img.columns() << "x" << img.rows() << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    doREPL(img);

    img.write(fileout);
    std::cout << "Written image" << std::endl;
    return 0;
}

void doREPL(Magick::Image& image) {
    image.modifyImage();
    image.type(Magick::TrueColorType);
    int xp = 50;
    int yp = 50;
    for(int j = 0; j < image.rows(); j+=yp) {
        std::cout << "sorting " << j << "/" << image.rows() << std::endl;
        
        for(int i = 0; i < image.columns(); i+=xp) {

            /* Set up coordinates */ 
            PixelSort::BoundedCoordinate myCoords(i, j, xp, yp, image.columns(), image.rows());

            /* Generate rowPixels */
            PixelSort::PixelVector rowPixels;
            PixelSort::readImageToPixelVector(image, rowPixels, myCoords);

            /* Matchers */
            PixelSort::CircleMatcher circ(PixelSort::Coordinate(i+(xp/2), j+(yp/2)), 20);
            PixelSort::ApplyMatcher(rowPixels, circ);

            /* Sort pixels */
            PixelSort::PixelVector rowPixels_sort = rowPixels;
            std::sort(rowPixels_sort.begin(), rowPixels_sort.end(), PixelSort::SumPixelComparator());

            /* Transform output arrays */
            std::transform(
                rowPixels.begin(), rowPixels.end(), rowPixels_sort.begin(), rowPixels.begin(), 
                [&](PixelSort::Pixel p1, PixelSort::Pixel p2){
                    return PixelSort::Pixel(p1, p2);
                });

            /* Write pixelvector to image */
            PixelSort::writePixelVectorToImage(rowPixels, image, myCoords);
        }
    }
}
