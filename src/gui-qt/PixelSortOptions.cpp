
#include <iostream>
#include <cmath>
#include <string>
#include "PixelSortOptions.hpp"

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

PixelSortOptions::PixelSortOptions():
sortColors{true, true, true},
moveColors{true, true, true},
colorMatcher{0, 1, 0, 1, 0, 1},
theta{0.0},
rect{0, 0, 200, 200},
Xrepeat{0, 200, 1000},
Yrepeat{0, 200, 1000}
{};

void PixelSortOptions::setImage(PS::Image* img)
{
    this->img  = img;
    Xrepeat[2] = int(img->columns());
    Yrepeat[2] = int(img->rows());
}

PixelSortOptions::~PixelSortOptions() {};

void PixelSortOptions::doSort() 
{
    /* Create comparator */
    double rComp = sortColors[0] ? 1 : 0;
    double gComp = sortColors[1] ? 1 : 0;
    double bComp = sortColors[2] ? 1 : 0;
    PS::WeightedComparator comp(rComp, gComp, bComp);

    /* Create applicator */
    PS::ApplyFcn applyFcn = sendColor<true, true, true>;
    if (moveColors[0]) {
        if (moveColors[1]) {
            if (moveColors[2]) {
                // do nothing
            } else {
                applyFcn = sendColor<true, true, false>;
            }
        } else {
            if (moveColors[2]) {
                applyFcn = sendColor<true, false, true>;
            } else {
                applyFcn = sendColor<true, false, false>;
            }
        }
    } else {
        if (moveColors[1]) {
            if (moveColors[2]) {
                applyFcn = sendColor<false, true, true>;
            } else {
                applyFcn = sendColor<false, true, false>;
            }
        } else {
            if (moveColors[2]) {
                applyFcn = sendColor<false, false, true>;
            } else {
                applyFcn = sendColor<false, false, false>;
            }
        }
    }

    /* Create color matcher */
    PS::RGBColor cMin(colorMatcher[0], colorMatcher[2], colorMatcher[4]);
    PS::RGBColor cMax(colorMatcher[1], colorMatcher[3], colorMatcher[5]);
    const PS::Matcher &mat = PS::RGBBandMatcher{cMin, cMax};

    /* Create quadMatcher */
    PS::RectangleMatcher quad(
        PS::BoxCoordinate(
            rect[0], rect[1], rect[2], rect[3]
        )
    );

    /* Create Xrepeat, Yrepeat parameters */
    int Xstart = Xrepeat[0];
    int Xpitch = Xrepeat[1];
    int Xend = img->columns(); // not controllable yet
    int Ystart = Yrepeat[0];
    int Ypitch = Yrepeat[1];
    int Yend = img->rows(); // not controllable yet

    /* 
     * Start PixelSorting 
     */
    int numBlocks = std::ceil(double(Xend - Xstart) / Xpitch) * std::ceil(double(Yend - Ystart) / Ypitch);
    int blocksCompleted = 0;

    /* Select a minimal region within the image */
    for (double coordX = Xstart; coordX < Xend; coordX += Xpitch) {
        for (double coordY = Ystart; coordY < Yend; coordY += Ypitch) {

            quad.bounds.x = coordX;
            quad.bounds.y = coordY;

            /* Update string */
            std::string str("PixelSorting, ");
            str += std::to_string(int(100 * double(blocksCompleted) / double(numBlocks)));
            str += "% done";
            notifyMe->notify(str.c_str());

            /* Build a PixelVector from the image's pixels */
            PS::PixelVector pv(*img, quad.bounds); 
            
            /* Rotate pixelvector */
            pv.sort(PS::AngleComparator(theta));

            /* Sort and Apply */
            PS::AsendorfSort<PS::Matcher, PS::Comparator>(pv, mat, comp, applyFcn); 
            
            ++blocksCompleted;
        }
    }
}

