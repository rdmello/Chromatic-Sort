
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
    // PixelSort::Matcher matcher = PixelSort::AllMatcher();
    // PixelSort::ApplyMatcher(pixels, matcher);
    // std::cout << "Applied matcher " << pixels.size() << std::endl;

//     std::for_each(pixels.begin(), pixels.end(), [](PixelSort::Pixel& pixel){
//         pixel.red(1);
//     });

    for(int i = 0; i < 2906; i+=10) {
        PixelSort::PixelVector rowPixels(10 * image.columns());
        std::copy_if(pixels.begin(), pixels.end(), rowPixels.begin(),
            [=](const PixelSort::Pixel& pixel){return (pixel.y >= i) && (pixel.y < i + 10);});
        PixelSort::PixelVector rowPixels_orig(image.columns());
        rowPixels_orig = rowPixels;

        std::cout << "sorting " << i << " num: " << rowPixels.size() << std::endl;
        PixelSort::Sort(rowPixels, PixelSort::SumPixelComparator());

        PixelSort::PixelVector::iterator j = rowPixels_orig.begin();
        for(auto i = rowPixels.begin(); i != rowPixels.end(); ++i){
            j->red(i->red());
            j->blue(i->blue());
            j->green(i->green());
            ++j;
        }
        PixelSort::writePixelVectorToImage(rowPixels_orig, image);
    }

    /* Update image */
    // PixelSort::writePixelVectorToImage(pixels, image);
    std::cout << "Wrote image" << std::endl;
}
