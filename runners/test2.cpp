
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
    int xp = 100;
    int yp = 100;
    for(int j = 0; j < image.rows(); j+=yp) {
        std::cout << j/yp << "|" << std::flush;
        
        for(int i = 0; i < image.columns(); i+=xp) {

            /* Set up coordinates */ 
            PixelSort::BoundedCoordinate myCoords(i, j, xp, yp, image.columns(), image.rows());

            /* Generate rowPixels */
            PixelSort::PixelVector rowPixels;
            PixelSort::readImageToPixelVector(image, rowPixels, myCoords);

            /* Matchers */
            PixelSort::CircleMatcher circ(PixelSort::Coordinate(i+(xp/2), j+(yp/2)), 35);
            PixelSort::ApplyMatcher(rowPixels, circ);

            /* Sort pixels */
            PixelSort::PixelVector rowPixels_sort = rowPixels;
            std::sort(rowPixels_sort.begin(), rowPixels_sort.end(), 
                [&](const PixelSort::Pixel& p1, const PixelSort::Pixel& p2) {
                    return p1.blue() > p2.blue();
                });

            /* Transform output arrays */
            std::transform(
                rowPixels.begin(), rowPixels.end(), rowPixels_sort.begin(), rowPixels.begin(), 
                [&](const PixelSort::Pixel& p1, const PixelSort::Pixel& p2){
                    PixelSort::Pixel p(p1, p2);
                    return p;
                });

            /* Write pixelvector to image */
            PixelSort::writePixelVectorToImage(rowPixels, image, myCoords);
        }
    }

    xp = 200;
    yp = 200;
    for(int j = 50; j < image.rows(); j+=yp) {
        std::cout << j/yp << "|" << std::flush;
        
        for(int i = 50; i < image.columns(); i+=xp) {

            /* Set up coordinates */ 
            PixelSort::BoundedCoordinate myCoords(i, j, xp, yp, image.columns(), image.rows());

            /* Generate rowPixels */
            PixelSort::PixelVector rowPixels;
            PixelSort::readImageToPixelVector(image, rowPixels, myCoords);

            /* Matchers */
            PixelSort::CircleMatcher circ(PixelSort::Coordinate(50+(i+(xp/4)), 50+(j+(yp/4))), 65);
            PixelSort::ApplyMatcher(rowPixels, circ);

            /* Reorganize to sort in y-direction */
            /* By default, the PixelVector is sorted in y-major direction */
            int weight_x = 1;
            int weight_y = 1;
            std::stable_sort(rowPixels.begin(), rowPixels.end(), 
                [&](const PixelSort::Pixel& p1, const PixelSort::Pixel& p2) {
                    return ((weight_x*p1.x)+(weight_y*p1.y)) < ((weight_x*p2.x)+(weight_y*p2.y)); 
                });

            /* Sort pixels */
            PixelSort::PixelVector sortedPixels = rowPixels;
            std::stable_sort(sortedPixels.begin(), sortedPixels.end(), 
                [&](const PixelSort::Pixel& p1, const PixelSort::Pixel& p2) {
                    return p1.blue() > p2.blue();
                });

            /* Transform output arrays */
            std::transform(
                rowPixels.begin(), rowPixels.end(), sortedPixels.begin(), rowPixels.begin(), 
                [&](const PixelSort::Pixel& p1, const PixelSort::Pixel& p2){
                    PixelSort::Pixel p = p1;
                    p.red(p2.red());
                    return p;
                });

            /* Write pixelvector to image */
            PixelSort::writePixelVectorToImage(rowPixels, image, myCoords);
        }
    }
    std::cout << std::endl;
}
