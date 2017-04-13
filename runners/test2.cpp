
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
    return 0;
}

void doREPL(Magick::Image& image) {
    /* Define pixel array and populate */
    PixelSort::PixelVector pixels;
    PixelSort::readImageToPixelVector(image, pixels);
    std::cout << "Read image " << pixels.size() << std::endl;

    /* Run matcher workflow */
//    PixelSort::RectangleMatcher matcher(PixelSort::BoxCoordinate(100, 100, 200, 200));
//    PixelSort::ApplyMatcher(pixels, matcher);
//    std::cout << "Applied matcher " << pixels.size() << std::endl;
//
//    std::for_each(pixels.begin(), pixels.end(), [](PixelSort::Pixel& pixel){
//      pixel.red(1);
//    });
//

    int xp = 200;
    int yp = 200;
    for(int j = 0; j < image.rows(); j+=yp) {
        std::cout << "sorting " << j << std::endl;
        for(int i = 0; i < image.columns(); i+=xp) {
            PixelSort::BoundedCoordinate myCoords(i, j, xp, yp, image.columns(), image.rows());
            PixelSort::PixelVector rowPixels( myCoords.width*myCoords.height );
            PixelSort::RectangleMatcher mat(myCoords);
            PixelSort::CircleMatcher circ(PixelSort::Coordinate(i+(xp/2), j+(yp/2)), 90);

            std::copy_if(pixels.begin(), pixels.end(), rowPixels.begin(), mat);
            PixelSort::ApplyMatcher(rowPixels, circ);

            PixelSort::PixelVector rowPixels_sort = rowPixels;
            PixelSort::PixelVector rowPixels_orig = rowPixels;
            PixelSort::Sort(rowPixels_sort, PixelSort::SumPixelComparator());

            PixelSort::PixelVector::iterator j = rowPixels_orig.begin();
            for(auto i = rowPixels_sort.begin(); i != rowPixels_sort.end(); ++i){
                j->red(i->red());
                // j->blue(i->blue());
                // j->green(i->green());
                ++j;
            }
            PixelSort::writePixelVectorToImage(rowPixels_orig, image);
        }
    }
    xp = 100;
    yp = 100;
    for(int j = 0; j < image.rows(); j+=yp) {
        std::cout << "sorting " << j << std::endl;
        for(int i = 0; i < image.columns(); i+=xp) {
            PixelSort::BoundedCoordinate myCoords(i, j, xp, yp, image.columns(), image.rows());
            PixelSort::PixelVector rowPixels( myCoords.width*myCoords.height );
            PixelSort::RectangleMatcher mat(myCoords);
            PixelSort::CircleMatcher circ(PixelSort::Coordinate(i+(xp/2), j+(yp/2)), 30);

            std::copy_if(pixels.begin(), pixels.end(), rowPixels.begin(), mat);
            PixelSort::ApplyMatcher(rowPixels, circ);

            PixelSort::PixelVector rowPixels_sort = rowPixels;
            PixelSort::PixelVector rowPixels_orig = rowPixels;
            PixelSort::Sort(rowPixels_sort, PixelSort::SumPixelComparator());

            PixelSort::PixelVector::iterator j = rowPixels_orig.begin();
            for(auto i = rowPixels_sort.begin(); i != rowPixels_sort.end(); ++i){
                j->red(i->red());
                j->blue(i->blue());
                j->green(i->green());
                ++j;
            }
            PixelSort::writePixelVectorToImage(rowPixels_orig, image);
        }
    }

    /* Update image */
    // PixelSort::writePixelVectorToImage(pixels, image);
    std::cout << "Wrote image" << std::endl;
}
