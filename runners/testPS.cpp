
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
            PixelVector pv;
            BoundedCoordinate box(i, j, w, h, image.columns(), image.rows());
            read(pv, image, box);
            
            PixelVector p_sort = pv;
            std::stable_sort(p_sort.begin(),p_sort.end(), 
                [&](const Pixel& p1, const Pixel& p2) {
                    return p1.red() < p2.red();
                });
            
            std::transform(pv.begin(), pv.end(), p_sort.begin(), pv.begin(), 
                [&](const Pixel& p1, const Pixel& p2) {
                    Pixel p = p1; 
                    p.red(p2.red());
                    return p;
                });

            write(pv, image, box);
        }
    }
    
    image.write(fileout);
    std::cout << "Written image" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    return 0;
}