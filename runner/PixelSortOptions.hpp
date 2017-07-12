
#include <Magick++.h>

#include "PixelSort.hpp"
namespace PS = PixelSort;

#ifndef _PIXELSORT_RUNNER_SORTOPTIONS_
#define _PIXELSORT_RUNNER_SORTOPTIONS_

struct Notifiable
{
    virtual void notify(const char*) = 0;
};

struct PixelSortOptions 
{
    Magick::Image* img;

    bool sortColors[3];
    bool moveColors[3];
    double colorMatcher[6];

    double theta;
    double rect[4];

    int Xrepeat[3];
    int Yrepeat[3];

    // Call this function with status updates
    Notifiable* notifyMe;

    PixelSortOptions();
    ~PixelSortOptions();
    
    void setImage(Magick::Image* img);
    void doSort();
};

#endif /* _PIXELSORT_RUNNER_SORTOPTIONS_ */
