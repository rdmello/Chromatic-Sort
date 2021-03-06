
#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>

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
    PS::BoolWeightedComparator comp(sortColors[0], sortColors[1], sortColors[2]);

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
    PS::RGBAColor cMin, cMax;
    cMin.red(static_cast<uint8_t>(colorMatcher[0] * 255));
    cMax.red(static_cast<uint8_t>(colorMatcher[1] * 255));
    cMin.green(static_cast<uint8_t>(colorMatcher[2] * 255));
    cMax.green(static_cast<uint8_t>(colorMatcher[3] * 255));
    cMin.blue(static_cast<uint8_t>(colorMatcher[4] * 255));
    cMax.blue(static_cast<uint8_t>(colorMatcher[5] * 255));
    
    const PS::Matcher &mat = PS::RGBBandMatcher{cMin, cMax};

    /* Create quadMatcher */
    PS::RectangleMatcher quad(
        PS::BoxCoordinate(
            rect[0], rect[1], rect[2], rect[3]
        )
    );

    /* Create Xrepeat, Yrepeat parameters */
    int Xstart = rect[0];
    int Xpitch = Xrepeat[1];
    int Xend = img->columns(); 
    int Ystart = rect[1];
    int Ypitch = Yrepeat[1];
    int Yend = img->rows(); 

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

