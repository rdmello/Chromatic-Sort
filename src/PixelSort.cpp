
#include "PixelSort.hpp"

PixelSort::PixelVector::PixelVector(Magick::Image& image, const BoxCoordinate& box)
    : image{image}, box{BoundedCoordinate(box, image.columns(), image.rows())} {
    const Magick::Quantum* a = image.getConstPixels(box.x, box.y, box.width, box.height);

    for(int j = 0; j < box.height; ++j) {
        for(int i = 0; i < 3 * box.width; i += 3) {
            int idx = i + (j * 3 * box.width);
            Magick::Color color(a[idx], a[idx+1], a[idx+2]);
            Magick::ColorRGB rgbColor(color);
            // Coordinate coord(box.x + (i/3), box.y + j);
            Coordinate coord(i/3, j);
            pixels.push_back(Pixel(coord, color));
        }
    }
}

PixelSort::PixelVector::PixelVector(const PixelVector& pv, int start, int end)
    : image(pv.image), box(pv.box) {
    std::vector<Pixel>::const_iterator first = pv.pixels.begin() + start;
    std::vector<Pixel>::const_iterator last = pv.pixels.begin() + end;
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
    std::stable_sort(pixels.begin(), pixels.end(), comparator);
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

    while (bounds.y < image.rows()) {
        bounds.x = 0;
        while (bounds.x < image.columns()) {

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

/* utility function */
void PixelSort::writeColor(const Magick::Color& color, Magick::Quantum* location) {
    *location = color.quantumRed();
    *(location+1) = color.quantumGreen();
    *(location+2) = color.quantumBlue();
}

