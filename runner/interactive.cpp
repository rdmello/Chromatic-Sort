
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
 *  - allow for changing the direction of a sort
 *
 * todos:
 *  - support GIFs
 *  - add partial and tweened gif frame support 
 *  - show progress bar
 * 
 * gui features:
 *  - 
 *
 * gui todos:
 *  - fix native macOS menubar
 *  - save and restore window size and position
 *
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include <Magick++.h>

#include "boost/program_options.hpp"
namespace po = boost::program_options;

#include "PixelSortApp.hpp"
#include "PixelSortOptions.hpp"

/* Logging function
 * Verbosity:
 *   logLevel 3: very verbose
 *   logLevel 2: medium verbose
 *   logLevel 1: only overall information
 *   logLevel 0: silent, except for errors
 * maybe I should replace this with a real library at some point 
 */
class Logger: public Notifiable {
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

    virtual void notify(const char* str) {
        log(3, str);
    }
};

int main (int argc, char* argv[]) {

    Magick::InitializeMagick("");

    /*
     * Branch into GUI app if no input arguments are given
     * never returns
     */
    if (argc <= 1) {
        QApplication app(argc, argv);
        PixelSortApp psapp(&app);

        psapp.show();
        return app.exec();
    }

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
        ("Xrepeat,X", po::value<std::string>(), "repeat in X-direction (start,pitch,end)")
        ("Yrepeat,Y", po::value<std::string>(), "repeat in Y-direction (start,pitch,end)")
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
    PixelSortOptions opts;

    /* 
     * Print input file name if one is found 
     */
    if (vm.count("input")) {
        infile = vm["input"].as<std::string>();
        logger.log(2, "Input file is: " + infile);

        /* Define a default output filename */ 
        outfile = infile;
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

    /* Set PixelSortOptions Image */
    opts.setImage(&img);
    opts.notifyMe = &logger;

    /* 
     * Read and set sort comparator
     */
    if (vm.count("sort")) {
        std::string colstr = vm["sort"].as<std::string>();
        if (colstr == "R") {
            opts.sortColors[1] = false;
            opts.sortColors[2] = false;
            logger.log(3, "Red Comparator set");
        } 
        else if (colstr == "G") {
            opts.sortColors[0] = false;
            opts.sortColors[2] = false;
            logger.log(3, "Green Comparator set");
        }
        else if (colstr == "B") {
            opts.sortColors[0] = false;
            opts.sortColors[1] = false;
            logger.log(3, "Blue Comparator set");
        }
        else if ((colstr == "RG")||(colstr == "GR")) {
            opts.sortColors[2] = false;
            logger.log(3, "Red+Green Comparator set");
        }
        else if ((colstr == "RB")||(colstr == "BR")) {
            opts.sortColors[1] = false;
            logger.log(3, "Red+Blue Comparator set");
        }
        else if ((colstr == "GB")||(colstr == "BG")) {
            opts.sortColors[0] = false;
            logger.log(3, "Green+Blue Comparator set");
        } 
        else {
            logger.log(3, "Comparator statement did not match, AllSumComparator set");
        }
    }
    
    /* 
     * Read and set color applied by sort comparator
     */
    if (vm.count("color")) {
        std::string colstr = vm["color"].as<std::string>();
        if (colstr == "R") {
            opts.moveColors[1] = false;
            opts.moveColors[2] = false;
            logger.log(3, "Red Applicator set");
        } 
        else if (colstr == "G") {            
            opts.moveColors[0] = false;
            opts.moveColors[2] = false;
            logger.log(3, "Green Applicator set");
        }
        else if (colstr == "B") {
            opts.moveColors[0] = false;
            opts.moveColors[1] = false;
            logger.log(3, "Blue Applicator set");
        }
        else if ((colstr == "RG")||(colstr == "GR")) {
            opts.moveColors[2] = false;
            logger.log(3, "Red+Green Applicator set");
        }
        else if ((colstr == "RB")||(colstr == "BR")) {
            opts.moveColors[1] = false;
            logger.log(3, "Red+Blue Applicator set");
        }
        else if ((colstr == "GB")||(colstr == "BG")) {
            opts.moveColors[0] = false;
            logger.log(3, "Green+Blue Applicator set");
        } 
        else {
            logger.log(3, "Applicator statement did not match, AllApplicator set");
        }
    }

    /* 
     * Read color matcher 
     */
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

        opts.colorMatcher[0] = redargs[0]; 
        opts.colorMatcher[1] = redargs[1]; 
        logger.log(3, "Red matcher definition found: "
                  + std::to_string(redargs[0])  + ", " + std::to_string(redargs[1]));
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
        opts.colorMatcher[2] = greenargs[0]; 
        opts.colorMatcher[3] = greenargs[1]; 

        logger.log(3, "Green matcher definition found: "
                  + std::to_string(greenargs[0]) + ", " + std::to_string(greenargs[1]) );
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
        opts.colorMatcher[4] = blueargs[0]; 
        opts.colorMatcher[5] = blueargs[1]; 

        logger.log(3, "Blue matcher definition found: "
                  + std::to_string(blueargs[0]) + ", " + std::to_string(blueargs[1]));
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
        opts.colorMatcher[0] = bwargs[0]; 
        opts.colorMatcher[1] = bwargs[1]; 
        opts.colorMatcher[2] = bwargs[0]; 
        opts.colorMatcher[3] = bwargs[1]; 
        opts.colorMatcher[4] = bwargs[0]; 
        opts.colorMatcher[5] = bwargs[1]; 

        logger.log(3, "Black-and-white matcher definition found: "
                  + std::to_string(bwargs[0]) + ", " + std::to_string(bwargs[1]));
    }

    /* Read geometric selectors, if defined */
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
            opts.rect[0] = boxargs[0];
            opts.rect[1] = boxargs[1]; 
            opts.rect[2] = boxargs[2]; 
            opts.rect[3] = boxargs[3];
        }
    }
    
    /* 
     * Parse and set sort angle directives
     */
    if (vm.count("theta")) 
    {
        std::string str = vm["theta"].as<std::string>();
        opts.theta = std::stod(str);
        logger.log(3, "Found theta directive: " + std::to_string(opts.theta));
    }

    /* 
     * Parse and set Xrepeat and Yrepeat parameters
     */
    if (vm.count("Xrepeat"))
    {
        std::string Xstr = vm["Xrepeat"].as<std::string>();
        for (auto&& i: Xstr) {
            if (i == ',') {
                i = ' ';
            }
        }
        std::istringstream iss{Xstr};
        std::vector<std::string> Xlist{std::istream_iterator<std::string>{iss},                                                   std::istream_iterator<std::string>{}};

        opts.Xrepeat[0] = std::stoi(Xlist[0]);
        opts.Xrepeat[1] = std::stoi(Xlist[1]);
        opts.Xrepeat[2] = std::stoi(Xlist[2]);

        logger.log(2, "X-Repeat definition found: (" + std::to_string(opts.Xrepeat[0])
            + "," + std::to_string(opts.Xrepeat[1]) + "," + std::to_string(opts.Xrepeat[2]) + ")");
    }

    if (vm.count("Yrepeat"))
    {
        std::string Ystr = vm["Yrepeat"].as<std::string>();
        for (auto&& i: Ystr) {
            if (i == ',') {
                i = ' ';
            }
        }
        std::istringstream iss{Ystr};
        std::vector<std::string> Ylist{std::istream_iterator<std::string>{iss},                                                   std::istream_iterator<std::string>{}};

        opts.Yrepeat[0] = std::stoi(Ylist[0]);
        opts.Yrepeat[1] = std::stoi(Ylist[1]);
        opts.Yrepeat[2] = std::stoi(Ylist[2]);

        logger.log(2, "Y-Repeat definition found: (" + std::to_string(opts.Yrepeat[0])
            + "," + std::to_string(opts.Yrepeat[1]) + "," + std::to_string(opts.Yrepeat[2]) + ")");

    }

    opts.doSort();

    /* Write to output file */
    logger.log(2, "Writing to outfile: " + outfile);
    img.write(outfile);

    return 0;
}
