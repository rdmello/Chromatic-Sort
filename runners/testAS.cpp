/* Actually testing Asendorf sort */

#include <iostream>
#include <string>

#include <Magick++.h>

#include "../src/PixelSort.hpp"

using namespace PixelSort;

int main() {
    /* File definitions */
    std::string filename = "images/lake";
    std::string fileext = "tiff";
    std::string filein = filename + "." + fileext; // define input filename
    std::string fileout = filename + "_out" + "." + fileext; // define output filename

    /* ImageMagick read image */
    Magick::Image image_orig(filein); // read image

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "IMAGE SIZE: " << image_orig.columns() << "x" << image_orig.rows() << std::endl;

    /* Start PixelSorting */
    image_orig.modifyImage();
    image_orig.type(Magick::TrueColorType);

    std::vector<Magick::Image> imgvec(10);

    for (int num = 0; num < imgvec.size(); ++num) {
        std::cout << num << " " << std::flush;
        Magick::Image image(image_orig);
        double incr = 0.05;
        if (num > 6) incr = 0.07;
        BWBandMatcher mat(0.1, 0.2 + (incr*num)); /* Define Asendorf matcher */
        
        for (int j(0); j < image.columns(); ++j) {
            
            /* Define region-of-interest and make PixelVector */
            int maxLen = image.rows();
            // BoxCoordinate box(num*maxLen, j, maxLen, 1);
            // BoxCoordinate box(0, j, maxLen, 1);
            BoxCoordinate box(j, 0, 1, maxLen);
            PixelVector pv(image, box);

            int i = 0;
            /* Check for a matching pixel */
            while (i < maxLen) {

                int k = i;
                /* Test if this pixel matches mat */
                if (mat(pv.pixels.at(i))) {
                    do {
                        ++k;
                    } while ((k < maxLen) && (mat(pv.pixels.at(k))));
                    if (k > maxLen) k = maxLen;

                    PixelVector pv2(pv, i, k);

                    /* Sort the vector into a new vector */
                    PixelVector pv3 = pv2;
                    pv3.sort([](const Pixel& p1, const Pixel& p2) {
                        return p1.red() < p2.red();
                    });

                    /* Combine the new sorted vector into the unsorted one */ 
                    pv2.apply(pv3, [](const Pixel& p1, const Pixel& p2) {
                        Pixel p(p1, p2);
                        return p;
                    });

                    /* Write to image */
                    pv2.sync();
                    i = k+1;
                } else {
                    ++i;
                }
            }
        }
        imgvec[num] = image;
    }

    Magick::writeImages(imgvec.begin(), imgvec.end(), "images/lake_out.gif");

    std::cout << "Written image" << std::endl;
    return 0;
}