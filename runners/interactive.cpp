
/* 
 * interactive.cpp
 * 
 * using boost::program_options to parse inputs and
 * do some easy pixelsorting
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include <Magick++.h>

#include "PixelSort.hpp"
namespace PS = PixelSort;

#include "boost/program_options.hpp"
namespace po = boost::program_options;

template<bool T1, bool T2, bool T3>
PS::Pixel sendColor(const PS::Pixel& p1, const PS::Pixel& p2) {
    PS::Pixel p(p1);
    if (T1) { 
        p.red(p2.red());
    }
    if (T2) {
        p.green(p2.green());
    }
    if (T3) {
        p.blue(p2.blue());
    }
    return p;
}

int main (int argc, char* argv[]) {

    /* This defines the options for boost::program_options */
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produces help message")
        ("input-file", po::value<std::string>(), "input file")
        ("output-file", po::value<std::string>(), "output file")
        ("box,B", po::value<std::string>(), "sort bounding box")
        ("color,C", po::value<std::string>(), "color to be sorted (RGBA)")
        ("matchRed,MR", po::value<std::string>(), "match a range of Red values")
        ("matchGreen,MG", po::value<std::string>(), "match a range of Green values")
        ("matchBlue,MB", po::value<std::string>(), "match a range of Blue values")
        ("matchBW,MBW", po::value<std::string>(), "match a black-and-white range")
        /*("matchRect,MRt", po::value<std::string>(), "match a rectangular region x,y,w,h")*/
    ;

    /* Define a variable map and a positional argument for the input file */
    po::variables_map vm;
    po::positional_options_description p;
    p.add("input-file", -1);

    /* Try parsing the command line entries, catch any exception and rethrow */
    try {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);
    } catch (...) {
        std::cout << "Error: Could not parse input arguments" << std::endl;
        std::cout << "Run: " << argv[0] << " --help for syntax help" << std::endl;
        std::cout << "------ Error details: -----" << std::endl;
        throw;
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    std::string infile, outfile;

    /* Print input file name if one is found */
    if (vm.count("input-file")) {
        infile = vm["input-file"].as<std::string>();
        std::cout << "Input file is: " << infile << std::endl;
        outfile = infile;
        int dot_idx = outfile.find_last_of(".");
        outfile.insert(dot_idx, "_out");
    } else {
        std::cout << "No input file found, exiting...." << std::endl;
        return 1;
    }

    /* Print output file name if one is found */
    if (vm.count("output-file")) {
        outfile = vm["output-file"].as<std::string>();
        std::cout << "Output file is: " << outfile << std::endl;
    } 

    /* Create and populate image */
    Magick::Image img;
    try {
        std::cout << "Reading image file" << std::endl;
        img.read(infile);
    } catch (...) {
        std::cout << "Failed to read image." << std::endl;
        throw;
    }

    /* Prove that image has been successfully read */
    std::cout << "Image successfully read: " << img.fileName() << std::endl;
    std::cout << "Dimensions: " << img.columns() << " x " << img.rows() << std::endl;    

    /* Read bounding-box, if defined */
    PS::Coordinate coord(100, 100); 
    if (vm.count("box")) {
        std::string boxstr = vm["box"].as<std::string>();
        for (auto&& i: boxstr) {
            if (i == ',') {
                i = ' ';
            }
        }
        std::istringstream iss{boxstr};
        std::vector<std::string> boxlist{std::istream_iterator<std::string>{iss},                                                   std::istream_iterator<std::string>{}};
        std::vector<int> boxargs;
        for (auto&& i: boxlist) {
            boxargs.push_back(std::stoi(i));
        }

        std::cout << "Box definition found: ";
        std::copy(boxargs.begin(), boxargs.end(), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;

        if (boxargs.size() == 2) {
            coord.x = boxargs[0];
            coord.y = boxargs[1];
        }
    }

    /* Define defaults for Comparator, Matcher, and Applicator */
    PS::Comparator comp = PS::WeightedComparator(1, 1, 1);
    PS::ApplyFcn applyFcn = sendColor<true, true, true>;

    if (vm.count("color")) {
        std::string colstr = vm["color"].as<std::string>();
        if (colstr == "R") {
            comp = PS::WeightedComparator(1, 0, 0);
            applyFcn = sendColor<true, false, false>;
        } 
        else if (colstr == "G") {
            comp = PS::WeightedComparator(0, 1, 0);
            applyFcn = sendColor<false, true, false>;
        }
        else if (colstr == "B") {
            comp = PS::WeightedComparator(0, 0, 1);
            applyFcn = sendColor<false, false, true>;
        }
        else if ((colstr == "RG")||(colstr == "GR")) {
            comp = PS::WeightedComparator(1, 1, 0);
            applyFcn = sendColor<true, true, false>;
        }
        else if ((colstr == "RB")||(colstr == "BR")) {
            comp = PS::WeightedComparator(1, 0, 1);
            applyFcn = sendColor<true, false, true>;
        }
        else if ((colstr == "GB")||(colstr == "BG")) {
            comp = PS::WeightedComparator(0, 1, 1);
            applyFcn = sendColor<false, true, true>;
        }
    }

    double rMin = 0, rMax = 1;
    double gMin = 0, gMax = 1;
    double bMin = 0, bMax = 1;

    if (vm.count("matchRed")) {
        std::string redstr = vm["matchRed"].as<std::string>();
        
        for (auto&& i: redstr) {
            if (i == ',') i = ' ';
        }
        std::istringstream iss{redstr};
        std::vector<std::string> redlist{std::istream_iterator<std::string>{iss},                                                   std::istream_iterator<std::string>{}};
        std::vector<double> redargs;
        for (auto&& i: redlist) {
            redargs.push_back(std::stod(i));
        }

        rMin = redargs[0]; rMax = redargs[1];
        std::cout << "Red matcher definition found: "
                  << rMin << ", " << rMax << std::endl;
    }
    
    if (vm.count("matchGreen")) {
        std::string greenstr = vm["matchGreen"].as<std::string>();
        
        for (auto&& i: greenstr) {
            if (i == ',') i = ' ';
        }
        std::istringstream iss{greenstr};
        std::vector<std::string> greenlist{std::istream_iterator<std::string>{iss},                                                   std::istream_iterator<std::string>{}};
        std::vector<double> greenargs;
        for (auto&& i: greenlist) {
            greenargs.push_back(std::stod(i));
        }
        gMin = greenargs[0]; gMax = greenargs[1];
        std::cout << "Green matcher definition found: "
                  << gMin << ", " << gMax << std::endl;
    }
    
    if (vm.count("matchBlue")) {
        std::string bluestr = vm["matchBlue"].as<std::string>();
        
        for (auto&& i: bluestr) {
            if (i == ',') i = ' ';
        }
        std::istringstream iss{bluestr};
        std::vector<std::string> bluelist{std::istream_iterator<std::string>{iss},                                                   std::istream_iterator<std::string>{}};
        std::vector<double> blueargs;
        for (auto&& i: bluelist) {
            blueargs.push_back(std::stod(i));
        }
        bMin = blueargs[0]; bMax = blueargs[1];
        std::cout << "Blue matcher definition found: "
                  << bMin << ", " << bMax << std::endl;
    }

    if (vm.count("matchBW")) {
        std::string bwstr = vm["matchBW"].as<std::string>();
        
        for (auto&& i: bwstr) {
            if (i == ',') i = ' ';
        }
        std::istringstream iss{bwstr};
        std::vector<std::string> bwlist{std::istream_iterator<std::string>{iss},                                                   std::istream_iterator<std::string>{}};
        std::vector<double> bwargs;
        for (auto&& i: bwlist) {
            bwargs.push_back(std::stod(i));
        }
        rMin = bwargs[0]; rMax = bwargs[1];
        gMin = bwargs[0]; gMax = bwargs[1];
        bMin = bwargs[0]; bMax = bwargs[1];
        std::cout << "Black-and-white matcher definition found: "
                  << bMin << ", " << bMax << std::endl;
    }

    Magick::ColorRGB cMin(rMin, gMin, bMin);
    Magick::ColorRGB cMax(rMax, gMax, bMax);

    const PS::Matcher &mat = PS::RGBBandMatcher{cMin, cMax};

    /* Start PixelSorting */
    img.modifyImage();
    img.type(Magick::TrueColorType);
    PS::BlockPixelSort(img, coord, mat, comp, applyFcn);

    /* Write to output file */
    std::cout << "Writing to outfile: " << outfile << std::endl;
    img.write(outfile);

    return 0;
}
