
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
template PS::Pixel sendColor<true, true, true>(const PS::Pixel&, const PS::Pixel&);

PixelSortOptions::PixelSortOptions():
sortColors{true, true, true},
moveColors{true, true, true},
colorMatcher{0, 1, 0, 1, 0, 1},
theta{0.0},
rect{0, 0, 100, 100},
Xrepeat{0, 100, 1000},
Yrepeat{0, 100, 1000}
{};

void PixelSortOptions::setImage(Magick::Image* img)
{
    this->img = img;
    Xrepeat[2] = double(img->columns());
    Yrepeat[2] = double(img->rows());
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
    // PS::ApplyFcn applyFcn = sendColor<moveColors[0], moveColors[1], moveColors[2]>;
    PS::ApplyFcn applyFcn = sendColor<true, true, true>;

    /* Create color matcher */
    Magick::ColorRGB cMin(colorMatcher[0], colorMatcher[2], colorMatcher[4]);
    Magick::ColorRGB cMax(colorMatcher[1], colorMatcher[3], colorMatcher[5]);
    const PS::Matcher &mat = PS::RGBBandMatcher{cMin, cMax};

    /* Create quadMatcher */
    PS::RectangleMatcher quad(
        PS::BoxCoordinate(
            rect[0], rect[1], rect[2], rect[3]
        )
    );

    /* Create Xrepeat, Yrepeat parameters */
    double Xstart = Xrepeat[0], Xpitch = Xrepeat[1], Xend = Xrepeat[2];
    double Ystart = Yrepeat[0], Ypitch = Yrepeat[1], Yend = Yrepeat[2];

    /* 
     * Start PixelSorting 
     */
    img->modifyImage();
    img->type(Magick::TrueColorType);

    /* Select a minimal region within the image */
    for (double coordX = Xstart; coordX < Xend; coordX += Xpitch) {
        for (double coordY = Ystart; coordY < Yend; coordY += Ypitch) {

            quad.bounds.x = coordX;
            quad.bounds.y = coordY;

            /* Build a PixelVector from the image's pixels */
            PS::PixelVector pv(
                *img, PS::BoxCoordinate(
                    0, 0, img->columns(), img->rows()
                ), quad
            ); 
        
            /* Rotate pixelvector */
            pv.sort(PS::AngleComparator(theta));

            /* Sort and Apply */
            PS::AsendorfSort<PS::Matcher, PS::Comparator>(pv, mat, comp, applyFcn); 
        }
    }
}

