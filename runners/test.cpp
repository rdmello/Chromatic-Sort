/*
 * test.cpp: a simple Magick++ I/O test
 *
 */

/* Standard Library includes */
#include <iostream>

/* Including Magick++ image processing library */
#include <Magick++.h>

/* Local includes */
#include "../src/PixelSort.hpp"

void doREPL(Magick::Image& image, std::string fileout);

int main() {

    /* File definitions */
    std::string filename = "images/pyramid";
    std::string fileext = "tiff";
    std::string filein = filename + "." + fileext; // define input filename
    std::string fileout = filename + "_out" + "." + fileext; // define output filename

    /* ImageMagick image manipulation */
    Magick::Image& img(filein); // read image

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Starting REPL, Ctrl+C to exit" << std::endl;
    std::cout << "IMAGE SIZE: " << img.columns() << "x" << img.rows() << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    doREPL(img, fileout); // NEVER RETURNS

    return 0;
}

void doREPL(Magick::Image& image, std::string fileout) {

    AllMatcher am1 = AllMatcher{};
    ColorMatcher *cm1 = &am1;

    RedFilter rf1{};
    BlueFilter bf1{};
    GreenFilter gf1{};
    AsendorfTransform at1{};

    ColorTransformer *ct1 = &at1;

    int replIter = 0;
    while (true) {
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "REPL Iter: " << replIter << std::endl;

        int borderx, bordery, borderwidth, borderheight;
        borderx = 0;
        bordery = 0;
        borderwidth = image.columns();
        borderheight = image.rows();
        // std::cout << "Enter borderx: ";
        // std::cin >> borderx;

        // std::cout << "Enter bordery: ";
        // std::cin >> bordery;

        // std::cout << "Enter borderwidth: ";
        // std::cin >> borderwidth;

        // std::cout << "Enter borderheight: ";
        // std::cin >> borderheight;

        int x, y, w, h;
        x = 0;
        y = 0;
        // std::cout << "Enter x: ";
        // std::cin >> x;

        // std::cout << "Enter y: ";
        // std::cin >> y;
/*
        char scolor;
        std::cout << "Enter color to sort (r, g, b): ";
        std::cin >> scolor;
        switch (scolor) {
            case 'r':
                ct1 = &rf1;
                break;
            case 'g':
                ct1 = &gf1;
                break;
            case 'b':
                ct1 = &bf1;
                break;
        }
        */
/*
        double threshval_low = 0.5;
        std::cout << "Enter color floor (0.00 - 1.00): ";
        std::cin >> threshval_low;

        double threshval_high = 0.5;
        std::cout << "Enter color ceil (0.00 - 1.00): ";
        std::cin >> threshval_high;
        PooledMatcher pm{threshval_low, threshval_high};
        cm1 = &pm;

        double redval = 0.5;
        std::cout << "Enter red channel matcher (0.00 - 1.00): ";
        std::cin >> redval;

        double greenval = 0.5;
        std::cout << "Enter green channel matcher (0.00 - 1.00): ";
        std::cin >> greenval;

        double blueval = 0.5;
        std::cout << "Enter blue channel matcher (0.00 - 1.00): ";
        std::cin >> blueval;

        double radius = 0.1;
        std::cout << "Enter radius of matcher (0.00 - 1.00): ";
        std::cin >> radius;

        Magick::ColorRGB matchColor{redval, greenval, blueval};
        ColorThreshMatcher pm{ matchColor, radius };
        cm1 = &pm;
       */
        SortDirection dirX = SortDirection::Inc;
//        char sdirx;
//        std::cout << "Enter sort direction x (i, d): ";
//        std::cin >> sdirx;
//        switch (sdirx) {
//            case 'i':
//                dirX = SortDirection::Inc;
//                break;
//            case 'd':
//                dirX = SortDirection::Dec;
//                break;
//        }

        SortDirection dirY = SortDirection::Inc;
//        char sdiry;
//        std::cout << "Enter sort direction y (i, d): ";
//        std::cin >> sdiry;
//        switch (sdiry) {
//            case 'i':
//                dirY = SortDirection::Inc;
//                break;
//            case 'd':
//                dirY = SortDirection::Dec;
//                break;
//        }

        OuterLoop ol = OuterLoop::Y;
//        char oloop;
//        std::cout << "Enter outer loop choice (x, y): ";
//        std::cin >> oloop;
//        switch (oloop) {
//            case 'x':
//                ol = OuterLoop::X;
//                break;
//            case 'y':
//                ol = OuterLoop::Y;
//                break;
//        }

        std::cout << "Enter w: ";
        std::cin >> w;

        std::cout << "Enter h: ";
        std::cin >> h;

        Magick::Image newImg{image, Magick::Geometry(image.columns(), image.rows(), 0, 0)};
        // Magick::Image newImg{img};
        // colorPixels(newImg, x, y, w, h);
        // sortPixels(newImg, x, y, w, h);
        std::cout << "Sorting pixels..." << std::endl;
        sortPixelBlocks(newImg, borderx, bordery, borderwidth, borderheight, x, y, w, h, *cm1, *ct1, dirX, dirY, ol);

        std::cout << "Writing image..." << std::endl;
        newImg.write(fileout); // write image

        char input;
        while (true) {
            std::cout << "Do you want to keep changes? (yY/nN)? ";
            std::cin >> input;

            if (input == 'y' || input == 'Y') {
                image.read(fileout);
                break;
            } else if (input == 'n' || input == 'N') {
                image.write(fileout);
                break;
            } else {
                /* Do nothing */
            }
        }
        std::cout << "Moving to next iteration..." << std::endl;
        replIter++;
    }
}
