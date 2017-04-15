
#include "PixelSort.hpp"

void PixelSort::read(PixelVector& pixels, const Magick::Image& image) {
    read(pixels, image, BoxCoordinate(0, 0, image.columns(), image.rows()));
}

void PixelSort::read(PixelVector& pixels, const Magick::Image& image, const BoxCoordinate& box) {
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

void PixelSort::write(const PixelVector& pixels, Magick::Image& image) {
    write(pixels, image, BoxCoordinate(0, 0, image.columns(), image.rows())); 
}

void PixelSort::write(const PixelVector& pixels, Magick::Image& image, const BoxCoordinate& box) {
    image.modifyImage();
    Magick::Quantum* q = image.getPixels(box.x, box.y, box.width, box.height);

    for (const Pixel& p : pixels) {
        writeColor(p, &q[(3*(p.x-box.x))+(3*(p.y-box.y)*box.width)]);
    }

    image.syncPixels();
}

template <typename T>
void PixelSort::match(PixelVector& pixels, T matcher) {
    pixels.erase(std::remove_if(pixels.begin(), pixels.end(), 
        [&](const Pixel& pixel) {
            return !matcher(pixel);
        }), pixels.end());
}

/* Explicit instantiations */
template void PixelSort::match<PixelSort::Matcher>(PixelSort::PixelVector& pixels, PixelSort::Matcher matcher);
template void PixelSort::match<bool (*)(const PixelSort::Pixel&)>(PixelSort::PixelVector& pixels, bool (*matcher)(const PixelSort::Pixel&));

void PixelSort::sort(PixelVector& pixels,
                     PixelSort::PixelComparator comp) {
    std::stable_sort(pixels.begin(), pixels.end(), comp);
}

void PixelSort::writeColor(Magick::Color color, Magick::Quantum* location) {
    *location = color.quantumRed();
    *(location+1) = color.quantumGreen();
    *(location+2) = color.quantumBlue();
}

