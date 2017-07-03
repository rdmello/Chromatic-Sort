
#include "PixelSort.hpp"

PixelSort::PixelVector::PixelVector(Magick::Image& image, const BoxCoordinate& box, const GeometryMatcher& matcher)
    : image{image}, box{BoundedCoordinate(box, image.columns(), image.rows())} {

    for (int j = 0; j < box.height; ++j) {
        for (int i = 0; i < box.width; ++i) {
            Pixel coord(Coordinate(i, j), Magick::ColorRGB(0, 0, 0));
            if (matcher(coord)) {
                pixels.push_back(coord);
            }
        }
    }
    
    const Magick::Quantum* a = image.getConstPixels(box.x, box.y, box.width, box.height);

    for (Pixel& p : pixels) {
         int idx = 3*(p.x + p.y*box.width);
         Magick::Color color(a[idx], a[idx+1], a[idx+2]);
         Magick::ColorRGB rgbColor(color);
         p.red(rgbColor.red());
         p.green(rgbColor.green());
         p.blue(rgbColor.blue());
     }
 }

PixelSort::PixelVector::PixelVector(Magick::Image& image, const BoxCoordinate& box)
    : PixelVector(image, box, GeometryMatcher()) {};

PixelSort::PixelVector::PixelVector(const PixelVector& pv, int start, int end)
    : image(pv.image), box(pv.box) {
    std::vector<Pixel>::const_iterator first = pv.pixels.begin() + start;
    std::vector<Pixel>::const_iterator last = pv.pixels.begin() + end;
    pixels = std::vector<Pixel>(first, last);
}

PixelSort::PixelVector::PixelVector(const PixelVector& pv, std::vector<Pixel>::iterator first,                                                std::vector<Pixel>::iterator last)
    : image(pv.image), box(pv.box) {
    pixels = std::vector<Pixel>(first, last);
}

void PixelSort::PixelVector::sync() {
    image.modifyImage();
    Magick::Quantum* q = image.getPixels(box.x, box.y, box.width, box.height);

    for (const Pixel& p : pixels) {
        // writeColor(p, &q[(3*(p.x-box.x))+(3*(p.y-box.y)*box.width)]);
        writeColor(p, &q[(3*p.x)+(3*p.y*box.width)]);
    }

    image.syncPixels();
}

/* MATCH FUNCTION */
void PixelSort::PixelVector::match(const Matcher& matcher) {
    pixels.erase(std::remove_if(pixels.begin(), pixels.end(), 
        [&](const Pixel& pixel) {
            return !matcher(pixel);
        }), pixels.end());
}
void PixelSort::PixelVector::match(const MatchFcn& matcher) {
    pixels.erase(std::remove_if(pixels.begin(), pixels.end(), 
        [&](const Pixel& pixel) {
            return !matcher(pixel);
        }), pixels.end());
}

/* STABLE SORT */
void PixelSort::PixelVector::sort(const Comparator& comparator) {
    std::stable_sort(pixels.begin(), pixels.end(), std::ref(comparator));
}
void PixelSort::PixelVector::sort(const CompareFcn& comparator) {
    std::stable_sort(pixels.begin(), pixels.end(), comparator);
}

/* UNSTABLE SORT */ 
void PixelSort::PixelVector::unstable_sort(const Comparator& comparator) {
    std::sort(pixels.begin(), pixels.end(), comparator);
}
void PixelSort::PixelVector::unstable_sort(const CompareFcn& comparator) {
    std::sort(pixels.begin(), pixels.end(), comparator);
}

/* APPLY */
void PixelSort::PixelVector::apply(void (*func)(Pixel&)) {
    for(PixelSort::Pixel& p: pixels) {
        func(p);
    }
}
void PixelSort::PixelVector::apply(const PixelVector& pv, const ApplyFcn& func) {
    std::transform(pixels.begin(), pixels.end(), pv.pixels.begin(), pixels.begin(), func);
}

// template <typename T1 = PixelSort::Matcher, typename T2 = PixelSort::Comparator>
template <typename T1 , typename T2 >
void PixelSort::BlockPixelSort(Magick::Image& image, Coordinate blocksize, 
                               const T1& matcher, const T2& compare, 
                               const ApplyFcn& applyfcn) {
    /* Normalize into bounded coordinates for safety */
    BoundedCoordinate bounds(0, 0, blocksize.x, blocksize.y, 
                             image.columns(), image.rows());

    while (bounds.y < (signed) image.rows()) {
        bounds.x = 0;
        
        while (bounds.x < (signed) image.columns()) {

            /* Define region-of-interest and make PixelVector */
            PixelVector pv(image, bounds);
            
            /* Define and apply a circular matcher */
            pv.match(matcher);

            /* Sort the vector into a new vector */
            PixelVector pv2 = pv;
            pv2.sort(compare);

            /* Combine the new sorted vector into the unsorted one */ 
            pv.apply(pv2, applyfcn);
            
            /* Write to image */
            pv.sync();

            bounds.x += bounds.width;
        }
        bounds.y += bounds.height;
    }
}

template void PixelSort::BlockPixelSort(Magick::Image& image, 
    Coordinate blocksize, const Matcher& mat, const Comparator& comp, 
    const ApplyFcn& applyfcn);

template void PixelSort::BlockPixelSort(Magick::Image& image, 
    Coordinate blocksize, const MatchFcn& mat, const Comparator& comp, 
    const ApplyFcn& applyfcn);

template void PixelSort::BlockPixelSort(Magick::Image& image, 
    Coordinate blocksize, const Matcher& mat, const CompareFcn& comp, 
    const ApplyFcn& applyfcn);

template void PixelSort::BlockPixelSort(Magick::Image& image, 
    Coordinate blocksize, const MatchFcn& mat, const CompareFcn& comp, 
    const ApplyFcn& applyfcn);

/* ASENDORFSORT TEMPLATE
 * template declaration and instantiations for AsendorfSort
 */
template <typename T1, typename T2>
void PixelSort::AsendorfSort(PixelVector& pv, 
    const T1& match, const T2& compare, const ApplyFcn& applyfcn) 
{
    std::vector<Pixel>::iterator i = pv.pixels.begin();

    for (; i < pv.pixels.end(); ++i) {
        if (match(*i)) {
            /* Find the 'j' where the match ends */
            std::vector<Pixel>::iterator j = i;
            do {
                ++j;
            } while ((j < pv.pixels.end()) && match(*j));

            PixelVector pv2(pv, i, j);
            
            PixelVector pv3(pv2);
            pv3.sort(compare);

            /* Combine the new sorted vector into the unsorted one */ 
            pv2.apply(pv3, applyfcn);
            
            /* Write to image */
            pv2.sync();

            /* Send i to the newest value */
            i = j;
        }
    }
}

template void PixelSort::AsendorfSort(PixelVector& pv, const Matcher& mat, 
    const Comparator& comp, const ApplyFcn& applyfcn);

template void PixelSort::AsendorfSort(PixelVector& pv, const MatchFcn& mat, 
    const Comparator& comp, const ApplyFcn& applyfcn);

template void PixelSort::AsendorfSort(PixelVector& pv, const Matcher& mat, 
    const CompareFcn& comp, const ApplyFcn& applyfcn);

template void PixelSort::AsendorfSort(PixelVector& pv, const MatchFcn& mat, 
    const CompareFcn& comp, const ApplyFcn& applyfcn);

void PixelSort::PixelVector::print() {
    std::vector<Pixel>::iterator i = pixels.begin();
    std::vector<Pixel>::iterator j = i+10;
    for ( ; i < j; ++i) {
        std::cout << " (" + std::to_string(i->red()) + "," + 
                            std::to_string(i->green()) + "," + 
                            std::to_string(i->blue()) + "," + 
                            std::to_string(i->x) + "," + 
                            std::to_string(i->y) + ")=" + 
                            std::to_string(i->red()+i->blue()+i->green()) << std::endl;
    }
}

/* utility function */
void PixelSort::writeColor(const Magick::Color& color, Magick::Quantum* location) {
    *location = color.quantumRed();
    *(location+1) = color.quantumGreen();
    *(location+2) = color.quantumBlue();
}

