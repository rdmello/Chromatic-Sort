
#include "PixelSort.hpp"

PixelSort::PixelVector::PixelVector(Magick::Image& image, const BoxCoordinate& box): image{image}, box{box} {
    const Magick::Quantum* a = image.getConstPixels(box.x, box.y, box.width, box.height);

    for(int j = 0; j < box.height; ++j) {
        for(int i = 0; i < 3 * box.width; i += 3) {
            int idx = i + (j * 3 * box.width);
            Magick::Color color(a[idx], a[idx+1], a[idx+2]);
            Magick::ColorRGB rgbColor(color);
            Coordinate coord(box.x + (i/3), box.y + j);
            pixels.push_back(Pixel(coord, color));
        }
    }
}

void PixelSort::PixelVector::sync() {
    image.modifyImage();
    Magick::Quantum* q = image.getPixels(box.x, box.y, box.width, box.height);

    for (const Pixel& p : pixels) {
        writeColor(p, &q[(3*(p.x-box.x))+(3*(p.y-box.y)*box.width)]);
    }

    image.syncPixels();
}

/* MATCH FUNCTION */
void PixelSort::PixelVector::match(PixelSort::Matcher& matcher) {
    pixels.erase(std::remove_if(pixels.begin(), pixels.end(), 
        [&](const Pixel& pixel) {
            return !matcher(pixel);
        }), pixels.end());
}
void PixelSort::PixelVector::match(bool (*matcher)(const PixelSort::Pixel&)) {
    pixels.erase(std::remove_if(pixels.begin(), pixels.end(), 
        [&](const Pixel& pixel) {
            return !matcher(pixel);
        }), pixels.end());
}

/* STABLE SORT */
void PixelSort::PixelVector::sort(const PixelSort::PixelComparator& comparator) {
    std::stable_sort(pixels.begin(), pixels.end(), comparator);
}
void PixelSort::PixelVector::sort(bool (*comparator)(const PixelSort::Pixel&, const PixelSort::Pixel&)) {
    std::stable_sort(pixels.begin(), pixels.end(), comparator);
}

/* UNSTABLE SORT */ 
void PixelSort::PixelVector::unstable_sort(const PixelSort::PixelComparator& comparator) {
    std::sort(pixels.begin(), pixels.end(), comparator);
}
void PixelSort::PixelVector::unstable_sort(bool (*comparator)(const PixelSort::Pixel&, const PixelSort::Pixel&)) {
    std::sort(pixels.begin(), pixels.end(), comparator);
}

/* APPLY */
void PixelSort::PixelVector::apply(void (*func)(PixelSort::Pixel&)) {
    for(PixelSort::Pixel& p: pixels) {
        func(p);
    }
}
void PixelSort::PixelVector::apply(const PixelSort::PixelVector& pv, PixelSort::Pixel (*func)(const PixelSort::Pixel&, const PixelSort::Pixel&)) {
    std::transform(pixels.begin(), pixels.end(), pv.pixels.begin(), pixels.begin(), func);
}

/* utility function */
void PixelSort::writeColor(Magick::Color color, Magick::Quantum* location) {
    *location = color.quantumRed();
    *(location+1) = color.quantumGreen();
    *(location+2) = color.quantumBlue();
}

