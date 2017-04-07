/* 
 * testGif.cpp: a simple Magick++ GIF I/O test
 *
 * Rylan Dmello Apr 2017
 */

/* Standard Library includes */
#include <iostream>
#include <vector>

/* Including Magick++ image processing library */
#include <Magick++.h>

/* Local includes */
#include "../src/Coordinate.hpp"
#include "../src/PixelSorter.hpp"
#include "../src/ColorMatcher.hpp"
#include "../src/ColorTransformer.hpp"

void doREPL(std::vector<Magick::Image>& image, std::string fileout);

int main (int argc, char** argv) {
    std::string filename = "images/waterfall";
    std::string fileext = "gif";
    std::string filein = filename + "." + fileext; // define input filename
    std::string fileout = filename + "_out" + "." + fileext; // define output filename

    Magick::InitializeMagick(*argv);
    std::vector<Magick::Image> imageList;
    readImages( &imageList, filein );

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Starting REPL, Ctrl+C to exit" << std::endl;
    std::cout << "IMAGE SIZE: " << imageList.front().columns() << "x" << imageList.front().rows() << std::endl;
    std::cout << "Num frames: " << imageList.size() << std::endl;
    std::cout << "------------------------------------------" << std::endl;
 

    doREPL( imageList, fileout );

    Magick::writeImages(imageList.begin(), imageList.end(), fileout);

    return 0;
}

void doREPL(std::vector<Magick::Image>& image, std::string fileout) {
    AllMatcher am1 = AllMatcher{};
    ColorMatcher *cm1 = &am1;
    
    RedFilter rf1{};
    BlueFilter bf1{};
    GreenFilter gf1{};
    
    ColorTransformer *ct1 = &rf1;

    int replIter = 0;
    while (true) {
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "REPL Iter: " << replIter << std::endl;

        int borderx, bordery, borderwidth, borderheight;
        borderx = 0;
        bordery = 0;
        borderwidth = image.front().columns();
        borderheight = image.front().rows();
        
        int x, y, w, h;
        x = 0;
        y = 0;
       
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
        
        SortDirection dirX = SortDirection::Inc;
        char sdirx; 
        std::cout << "Enter sort direction x (i, d): ";
        std::cin >> sdirx;
        switch (sdirx) {
            case 'i':
                dirX = SortDirection::Inc;
                break;
            case 'd':
                dirX = SortDirection::Dec;
                break;
        }
    
        SortDirection dirY = SortDirection::Inc;
        char sdiry; 
        std::cout << "Enter sort direction y (i, d): ";
        std::cin >> sdiry;
        switch (sdiry) {
            case 'i':
                dirY = SortDirection::Inc;
                break;
            case 'd':
                dirY = SortDirection::Dec;
                break;
        }

        OuterLoop ol = OuterLoop::Y;
        char oloop; 
        std::cout << "Enter outer loop choice (x, y): ";
        std::cin >> oloop;
        switch (oloop) {
            case 'x':
                ol = OuterLoop::X;
                break;
            case 'y':
                ol = OuterLoop::Y;
                break;
        }

        std::cout << "Enter w: ";
        std::cin >> w;

        std::cout << "Enter h: ";
        std::cin >> h;

        std::cout << "Coalescing frames..." << std::endl;
       
        Magick::coalesceImages(&image, image.begin(), image.end());

        std::cout << "Sorting image: ";

        for (int i = 0; i < image.size(); ++i) {
            std::cout << i << " " << std::flush;
            sortPixelBlocks(image[i], borderx, bordery, borderwidth, borderheight, x, y, w, h, *cm1, *ct1, dirX, dirY, ol);
        }
        
        std::cout << "Writing image..." << std::endl;
        writeImages(image.begin(), image.end(), fileout); // write image

        char input; 
        std::cout << "Do you want to keep changes? (yY/nN)? ";
        std::cin >> input;

        std::cout << "Moving to next iteration..." << std::endl;
        replIter++;
    }

}
