
/* 
 * interactive.cpp
 * 
 * using boost::program_options to parse inputs and
 * do some easy pixelsorting
 * 
 * This file is currently divided into three stages: 
 *  1. defining the command-line arguments for 
 *     boost::program_options
 *  2. reading the command-line arguments and creating
 *     the necessary data structures to represent them
 *  3. sorting the image using the PixelSort library
 *
 * currently supports: 
 *  - sorting different colors using -C flag
 *  - matching a color range using --red, --blue, etc
 *  - Asendorf sort by default
 *  - add geometry matchers like line, circle, subrect
 *
 * todos:
 *  - allow for changing the direction of a sort
 *  - support GIFs
 *  - add partial and tweened gif frame support 
 *
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

/* Helper function which generates the correct Applicator
 * for pixelsorting 
 */
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

/* Logging function
 * Verbosity:
 *   logLevel 3: very verbose
 *   logLevel 2: medium verbose
 *   logLevel 1: only overall information
 *   logLevel 0: silent, except for errors
 * maybe I should replace this with a real library at some point 
 */
class Logger {
    int level;
public:

    /* default ctor */
    Logger() {}; 

    /* set the global loglevel when initializing this object */
    Logger(int level): level(level) {
        this->setLevel(level);
    };

    void setLevel(int level) {
        this->level = level;
    }

    int getLevel() {
        return level;
    }

    void log(int level, std::string str) {
        if (level == -1) {
            std::cerr << "ERROR: " << str << std::endl;
        } else if (level == -2) {
            std::cerr << "WARNING: " << str << std::endl;
        } else if (level <= this->level) {
            std::cout << level << ": " << str << std::endl;
        }
    }
};

int main (int argc, char* argv[]) {

    Magick::InitializeMagick("");

    /* 
     * SECTION 1: Parsing command-line options using boost::program_options
     */

    /* This defines the options for boost::program_options */
    po::options_description generic_options("Allowed options");
    generic_options.add_options()
        ("help", "produces help message")
        ("verbose,V", po::value<int>(), "change verbosity (0 - 3)")
        ("input,I", po::value<std::string>(), "input file")
        ("output,O", po::value<std::string>(), "output file")
    ;

    po::options_description matcher_options("Color matching options");
    matcher_options.add_options()
        ("red,R", po::value<std::string>(), "match a range of Red values (rmin,rmax)")
        ("green,G", po::value<std::string>(), "match a range of Green values (gmin,gmax)")
        ("blue,B", po::value<std::string>(), "match a range of Blue values (bmin,bmax)")
        ("mean,M", po::value<std::string>(), "match a black-and-white range (bwmin,bwmax)")
        ("all,A", po::value<std::string>(), "match all")
    ;

    po::options_description geometry_options("Geometry selection options");
    geometry_options.add_options()   
        ("quad,Q", po::value<std::string>(), "select a Rectangular region to sort (x,y,w,h)")
        ("line,L", po::value<std::string>(), "select a Linear region to sort (x1,y1,x2,y2,r)")
        ("disk,D", po::value<std::string>(), "select a Circular region to sort (x,y,r)")
    ;

    po::options_description sort_options("Sort options");
    sort_options.add_options()    
        ("sort,S", po::value<std::string>(), "color to be compared and sorted (R,G,B,A)")
        ("color,C", po::value<std::string>(), "color to be moved (R,G,B,A)")
        ("theta,T", po::value<std::string>(), "rotate sort direction theta (angle,in,out)")
        ("Xrepeat,X", po::value<std::string>(), "repeat in X-direction (Fill,+,-)")
        ("Yrepeat,Y", po::value<std::string>(), "repeat in Y-direction (Fill,+,-)")
    ;

    /* Visible options */
    po::options_description visible_options("Allowed options");
    visible_options.add(generic_options)
                   .add(matcher_options)
                   .add(geometry_options)
                   .add(sort_options);

    /* The input filename can be provided as a positional argument */
    po::positional_options_description p;
    p.add("input", -1);

    /* Define a variable map */
    po::variables_map vm;
    
    /* Try parsing the command line entries, catch any exception and rethrow */
    try {
        po::store(
            po::command_line_parser(argc, argv)
                .options(visible_options)
                .positional(p)
                .run(), 
            vm
        );
        po::notify(vm);
    } catch (...) {
        std::cout << "Error: Could not parse input arguments" << std::endl;
        std::cout << "Run: " << argv[0] << " --help for syntax help" << std::endl;
        std::cout << "------ Error details: -----" << std::endl;
        throw;
    }

    /* Check if the help text has been requested, exit if found */
    if (vm.count("help")) {
        std::cout << generic_options  << std::endl;
        std::cout << matcher_options  << std::endl;
        std::cout << geometry_options << std::endl;
        std::cout << sort_options     << std::endl;
        return 1;
    }

    /* 
     * Set up the logger
     * 
     * if a verbosity is provided, set the loglevel accordingly
     * else default to loglevel 1
     */
    Logger logger;
    if (vm.count("verbose")) {
        logger.setLevel(vm["verbose"].as<int>());    
    } else {
        logger.setLevel(1);
    }
    logger.log(2, "Logger initialized to loglevel: " + std::to_string(logger.getLevel()));

    /* Necessary variables */
    std::string infile, outfile;
    Magick::Image img;

    /* 
     * Print input file name if one is found 
     */
    if (vm.count("input")) {
        infile = vm["input"].as<std::string>();
        logger.log(2, "Input file is: " + infile);

        /* Define a default output filename */ 
        outfile = infile;
        int dot_idx = outfile.find_last_of(".");
        outfile.insert(dot_idx, "_out");
        logger.log(3, "Output filename default: " + outfile);
    } else {
        logger.log(-1, "No input file found, exiting....");
        return 1;
    }

    /* 
     * Print output file name if one is found 
     */
    if (vm.count("output")) {
        outfile = vm["output"].as<std::string>();
        logger.log(2, "Output file is: " + outfile);
    } 

    /* 
     * Read and set sort comparator
     */
    PS::WeightedComparator comp = PS::WeightedComparator(1, 1, 1);
    
    if (vm.count("sort")) {
        std::string colstr = vm["sort"].as<std::string>();
        if (colstr == "R") {
            comp = PS::WeightedComparator(1, 0, 0);
            logger.log(3, "Red Comparator set");
        } 
        else if (colstr == "G") {
            comp = PS::WeightedComparator(0, 1, 0);
            logger.log(3, "Green Comparator set");
        }
        else if (colstr == "B") {
            comp = PS::WeightedComparator(0, 0, 1);
            logger.log(3, "Blue Comparator set");
        }
        else if ((colstr == "RG")||(colstr == "GR")) {
            comp = PS::WeightedComparator(1, 1, 0);
            logger.log(3, "Red+Green Comparator set");
        }
        else if ((colstr == "RB")||(colstr == "BR")) {
            comp = PS::WeightedComparator(1, 0, 1);
            logger.log(3, "Red+Blue Comparator set");
        }
        else if ((colstr == "GB")||(colstr == "BG")) {
            comp = PS::WeightedComparator(0, 1, 1);
            logger.log(3, "Green+Blue Comparator set");
        } 
        else {
            logger.log(3, "Comparator statement did not match, AllSumComparator set");
        }
    }
    
    /* 
     * Read and set color applied by sort comparator
     */
    PS::ApplyFcn applyFcn = sendColor<true, true, true>;
    if (vm.count("color")) {
        std::string colstr = vm["color"].as<std::string>();
        if (colstr == "R") {
            applyFcn = sendColor<true, false, false>;
            logger.log(3, "Red Applicator set");
        } 
        else if (colstr == "G") {
            applyFcn = sendColor<false, true, false>;
            logger.log(3, "Green Applicator set");
        }
        else if (colstr == "B") {
            applyFcn = sendColor<false, false, true>;
            logger.log(3, "Blue Applicator set");
        }
        else if ((colstr == "RG")||(colstr == "GR")) {
            applyFcn = sendColor<true, true, false>;
            logger.log(3, "Red+Green Applicator set");
        }
        else if ((colstr == "RB")||(colstr == "BR")) {
            applyFcn = sendColor<true, false, true>;
            logger.log(3, "Red+Blue Applicator set");
        }
        else if ((colstr == "GB")||(colstr == "BG")) {
            applyFcn = sendColor<false, true, true>;
            logger.log(3, "Green+Blue Applicator set");
        } 
        else {
            logger.log(3, "Applicator statement did not match, AllApplicator set");
        }
    }

    double rMin = 0, rMax = 1;
    double gMin = 0, gMax = 1;
    double bMin = 0, bMax = 1;

    if (vm.count("red")) {
        std::string redstr = vm["red"].as<std::string>();
        
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
    
    if (vm.count("green")) {
        std::string greenstr = vm["green"].as<std::string>();
        
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
    
    if (vm.count("blue")) {
        std::string bluestr = vm["blue"].as<std::string>();
        
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

    if (vm.count("mean")) {
        std::string bwstr = vm["mean"].as<std::string>();
        
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

    /* Read geometric selectors, if defined */
    std::vector<PS::RectangleMatcher> quadMatchers;
    std::vector<PS::CircleMatcher>    diskMatchers;
    std::vector<PS::LineMatcher>      lineMatchers;

    if (vm.count("quad")) {
        std::string boxstr = vm["quad"].as<std::string>();
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

        logger.log(2, "Box definition found");

        if (boxargs.size() == 4) {
            quadMatchers.push_back(PS::RectangleMatcher(PS::BoxCoordinate(
                boxargs[0], boxargs[1], boxargs[2], boxargs[3]
            )));
        }
    }
    
    if (vm.count("disk")) {
        std::string boxstr = vm["disk"].as<std::string>();
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

        logger.log(2, "Disk definition found: " + 
                       std::to_string(boxargs[0]) + ", " +
                       std::to_string(boxargs[1]) + ", " +
                       std::to_string(boxargs[2]));

        if (boxargs.size() == 3) {
            diskMatchers.push_back(PS::CircleMatcher(
                PS::Coordinate(boxargs[0], boxargs[1]), boxargs[2]));
        }
    }
    
    if (vm.count("line")) {
        std::string boxstr = vm["line"].as<std::string>();
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

        logger.log(2, "Line definition found");

        if (boxargs.size() == 5) {
            lineMatchers.push_back(PS::LineMatcher(
                PS::Coordinate(boxargs[0], boxargs[1]), 
                PS::Coordinate(boxargs[2], boxargs[3]), boxargs[4]));
        }
    }

    /* 
     * Parse and set sort angle directives
     */
    double angle = 0;

    if (vm.count("theta")) 
    {
        std::string str = vm["theta"].as<std::string>();
        angle = std::stod(str);
        logger.log(3, "Found theta directive: " + std::to_string(angle));
    }

    /* 
     * Read image 
     */
    try 
    {
        logger.log(3, "Reading image file");
        img.read(infile);
        
        /* Prove that image has been successfully read */
        logger.log(2, "Image successfully read: " + img.fileName());
        logger.log(2, "Dimensions: " + std::to_string(img.columns())+ " x " + std::to_string(img.rows()));    
    } 
    catch(Magick::WarningCoder& warning) 
    {
        logger.log(-2, std::string("Warning: ") + warning.what());
    } 
    catch (Magick::ErrorFileOpen& error) 
    {
        logger.log(-1, std::string("Error while reading image file") + error.what());
    }

    Magick::ColorRGB cMin(rMin, gMin, bMin);
    Magick::ColorRGB cMax(rMax, gMax, bMax);

    const PS::Matcher &mat = PS::RGBBandMatcher{cMin, cMax};

    /* 
     * STAGE 3: Start PixelSorting 
     */
    img.modifyImage();
    img.type(Magick::TrueColorType);

    /* Select a minimal region within the image */
    if (quadMatchers.size() >= 1) {
        logger.log(3, "Using user-defined selector for minimal image read");
    } else {
        quadMatchers.push_back(PS::RectangleMatcher(PS::BoxCoordinate(0, 0, img.columns(), img.rows())));
        logger.log(3, "Using default selector for image read");
    }

    /* Build a PixelVector from the image's pixels */
    PS::PixelVector pv(img, 
                       PS::BoxCoordinate(0, 0, img.columns(), img.rows()),
                       quadMatchers[0]); 
   
    /* Apply more selectors */
    for (PS::CircleMatcher& circ : diskMatchers) {
        pv.match(circ);  
    }
    for (PS::LineMatcher& line : lineMatchers) {
        pv.match(line);  
    }

    /* Apply color matchers */
    // pv.match(mat);

    /* Rotate pixelvector */
    pv.sort(PS::AngleComparator(angle));

    /* Sort and Apply */
    PS::AsendorfSort<PS::Matcher, PS::Comparator>(pv, mat, comp, applyFcn); 
    
    /* Write to output file */
    logger.log(2, "Writing to outfile: " + outfile);
    img.write(outfile);

    return 0;
}
