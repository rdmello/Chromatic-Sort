
#include <Magick++.h>

#include "PixelSort.hpp"
namespace PS = PixelSort;

#ifndef _PIXELSORT_RUNNER_SORTOPTIONS_
#define _PIXELSORT_RUNNER_SORTOPTIONS_

struct PixelSortOptions 
{
    Magick::Image* img;

    bool sortColors[3];
    bool moveColors[3];
    double colorMatcher[6];

    double theta;
    double rect[4];

    double Xrepeat[3];
    double Yrepeat[3];

    PixelSortOptions();
    ~PixelSortOptions();
    
    void setImage(Magick::Image* img);
    void doSort();
};

#endif /* _PIXELSORT_RUNNER_SORTOPTIONS_ */
