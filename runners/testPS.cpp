
/* Testing Asendorf sort */

#include <iostream>
#include <string>
#include <cmath>

#include <Magick++.h>

#include "../src/PixelSort.hpp"

using namespace PixelSort;

int main() {
    /* File definitions */
    std::string filename = "images/night";
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

    int w = 128;
    int h = 128;
    Coordinate block(w, h);

    std::cout << "Operation 1" << std::endl;
    PixelVector pv(image, BoxCoordinate(0, 0, image.columns(), image.rows()), 
                   LineMatcher(Coordinate(0, 0), Coordinate(image.columns()/2, image.rows()/2), 160));
    pv.sort([](const Pixel& p1, const Pixel& p2){
        double theta = 0.7854;
        double val1 = (p1.x*std::cos(theta)) + (p1.y*std::sin(theta));
        double val2 = (p2.x*std::cos(theta)) + (p2.y*std::sin(theta));
        return val1 < val2;
    });
    AsendorfSort<Matcher, Comparator>(pv, BWBandMatcher(0.0, 0.5), SumPixelComparator(), 
                                      [](const Pixel& p1, const Pixel& p2) {
                                          Pixel p(p1); 
                                          p.red(p2.blue());
                                          p.blue(p2.green());
                                          p.green(p2.red());
                                          return p;
    });
    
    std::cout << "Operation 2" << std::endl;
    PixelVector pv2(image, BoxCoordinate(0, 0, image.columns(), image.rows()), 
                   LineMatcher(Coordinate(0, 0+480), 
                               Coordinate(image.columns()/2, (image.rows()/2)+480), 160));
    pv2.sort([](const Pixel& p1, const Pixel& p2){
        double theta = 0.7854;
        double val1 = (p1.x*std::cos(theta)) + (p1.y*std::sin(theta));
        double val2 = (p2.x*std::cos(theta)) + (p2.y*std::sin(theta));
        return val1 < val2;
    });
    AsendorfSort<Matcher, Comparator>(pv2, BWBandMatcher(0.0, 0.5), SumPixelComparator(), 
                                      [](const Pixel& p1, const Pixel& p2) {
                                          Pixel p(p1, p2); return p;
    });

    std::cout << "Operation 3" << std::endl;
    PixelVector pv22(image, BoxCoordinate(0, 0, image.columns(), image.rows()), 
                   LineMatcher(Coordinate(0, 0+960), 
                               Coordinate(image.columns()/2, (image.rows()/2)+960), 160));
    pv22.sort([](const Pixel& p1, const Pixel& p2){
        double theta = 0.7854;
        double val1 = (p1.x*std::cos(theta)) + (p1.y*std::sin(theta));
        double val2 = (p2.x*std::cos(theta)) + (p2.y*std::sin(theta));
        return val1 < val2;
    });
    AsendorfSort<Matcher, Comparator>(pv22, BWBandMatcher(0.0, 0.5), SumPixelComparator(), 
                                      [](const Pixel& p1, const Pixel& p2) {
                                          Pixel p(p1, p2); return p;
    });
    
    std::cout << "Operation 4" << std::endl;
    PixelVector pv3(image, BoxCoordinate(0, 0, image.columns(), image.rows()), 
                   LineMatcher(Coordinate(0, 0-480), 
                               Coordinate(image.columns()/2, (image.rows()/2)-480), 160));
    pv3.sort([](const Pixel& p1, const Pixel& p2){
        double theta = 0.7854;
        double val1 = (p1.x*std::cos(theta)) + (p1.y*std::sin(theta));
        double val2 = (p2.x*std::cos(theta)) + (p2.y*std::sin(theta));
        return val1 < val2;
    });
    AsendorfSort<Matcher, Comparator>(pv3, BWBandMatcher(0.0, 0.5), SumPixelComparator(), 
                                      [](const Pixel& p1, const Pixel& p2) {
                                          Pixel p(p1, p2); return p;
    });
    
    std::cout << "Operation 5" << std::endl;
    PixelVector pv32(image, BoxCoordinate(0, 0, image.columns(), image.rows()), 
                   LineMatcher(Coordinate(0, 0-960), 
                               Coordinate(image.columns()/2, (image.rows()/2)-960), 160));
    pv32.sort([](const Pixel& p1, const Pixel& p2){
        double theta = 0.7854;
        double val1 = (p1.x*std::cos(theta)) + (p1.y*std::sin(theta));
        double val2 = (p2.x*std::cos(theta)) + (p2.y*std::sin(theta));
        return val1 < val2;
    });
    AsendorfSort<Matcher, Comparator>(pv32, BWBandMatcher(0.0, 0.5), SumPixelComparator(), 
                                      [](const Pixel& p1, const Pixel& p2) {
                                          Pixel p(p1, p2); return p;
    });
    
    std::cout << "Operation 6" << std::endl;
    PixelVector pv33(image, BoxCoordinate(0, 0, image.columns(), image.rows()), 
                   LineMatcher(Coordinate(0, 0-960-480), 
                               Coordinate(image.columns()/2, (image.rows()/2)-960-480), 160));
    pv33.sort([](const Pixel& p1, const Pixel& p2){
        double theta = 0.7854;
        double val1 = (p1.x*std::cos(theta)) + (p1.y*std::sin(theta));
        double val2 = (p2.x*std::cos(theta)) + (p2.y*std::sin(theta));
        return val1 < val2;
    });
    AsendorfSort<Matcher, Comparator>(pv33, BWBandMatcher(0.0, 0.5), SumPixelComparator(), 
                                      [](const Pixel& p1, const Pixel& p2) {
                                          Pixel p(p1, p2); return p;
    });
    
    image.write(fileout);
    std::cout << "Written image" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    return 0;
}