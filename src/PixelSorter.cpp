
#include "PixelSorter.hpp"

void writeColor(Magick::Color color, Magick::Quantum* location) { 
    *location = color.quantumRed();
    *(location+1) = color.quantumGreen();
    *(location+2) = color.quantumBlue();
}

void colorPixels(Magick::Image& image, int x, int y, int runWidth, int runLength) {
    int width = runWidth;
    int height = runLength;

    image.modifyImage(); 
    image.type(Magick::TrueColorType); 

    // FORCE COPY of data
    // image.pixelColor(100, 100, image.pixelColor(100, 100));
    Magick::Quantum* pixels = image.getPixels(x, y, width, height); 
    // std::cout << "Num channels: " << image.channels() << std::endl;

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < 3*width; i = i + 3) {
            int idx = i + (j * 3 * width);
            // std::cout << i << " " << j << " " << idx << std::endl;
            Magick::Color color(pixels[idx], pixels[idx+1], pixels[idx+2]);
            Magick::ColorYUV myColor(color);
            myColor.u(myColor.v() + 0.3);
            myColor.v(myColor.u() + 0.3);
            writeColor(myColor, &pixels[idx]);
        }
    }
    image.syncPixels();
}


void sortPixels(Magick::Image& image, int x, int y, int runWidth, int runLength, 
        ColorMatcher &cm, ColorTransformer &ct, 
        SortDirection SDx, SortDirection SDy, OuterLoop ol) 
{
    int width = runWidth;
    int height = runLength;

    image.modifyImage(); 
    image.type(Magick::TrueColorType); 
    
    Magick::Quantum* pixels = image.getPixels(x, y, width, height); 
    double *rowArr = new double[height * width];
    size_t k1 = 0;
 
    int olInit = 0;
    int olTerm = height;
    int olStep = 1;

    int ilInit = 0;
    int ilTerm = 3*width;
    int ilStep = 3;


    if (ol == OuterLoop::X) {
        olInit = 0;
        olTerm = 3 * width;
        olStep = 3;

        ilInit = 0;
        ilTerm = height;
        ilStep = 1;
    }

    if (SDx == SortDirection::Dec) {
        if (ol == OuterLoop::Y) {
            int temp = ilInit;
            ilInit = ilTerm - ilStep;
            ilTerm = temp - ilStep;
            ilStep *= -1;
        } else {
            int temp = olInit;
            olInit = olTerm - olStep;
            olTerm = temp - olStep;
            olStep *= -1;
        }
    }

    if (SDy == SortDirection::Dec) {
        if (ol == OuterLoop::X) {
            int temp = ilInit;
            ilInit = ilTerm - ilStep;
            ilTerm = temp - ilStep;
            ilStep *= -1;
        } else {
            int temp = olInit;
            olInit = olTerm - olStep;
            olTerm = temp - olStep;
            olStep *= -1;
        }
    }

    for (int j = olInit; j != olTerm; j += olStep) {
        for (int i = ilInit; i != ilTerm; i += ilStep) {
            size_t idx = (ol==OuterLoop::Y) ? i + (j * 3 * width) : j + (i * 3 * width);
            Magick::Color color(pixels[idx], pixels[idx+1], pixels[idx+2]);
            Magick::ColorRGB myColor(color);
            if (cm.match(myColor)) {
                rowArr[k1] = ct.transform(myColor);
                ++k1;
            }
        }
    }
    
    if (k1 > 0) { 
        std::sort(rowArr, rowArr+k1-1);
    }
    
    size_t k2 = 0;
    for (int j = olInit; j != olTerm; j += olStep) {
        for (int i = ilInit; i != ilTerm; i += ilStep) {
            size_t idx = (ol==OuterLoop::Y) ? i + (j * 3 * width) : j + (i * 3 * width);
            Magick::Color color(pixels[idx], pixels[idx+1], pixels[idx+2]);
            Magick::ColorRGB myColor(color);
            if (cm.match(myColor)) {
                ct.inv_transform(myColor, rowArr[k2]);
                writeColor(myColor, &pixels[idx]); 
                ++k2;
            }
        }
    }

    image.syncPixels();
    delete [] rowArr;
}

void sortPixelBlocks(Magick::Image& image, int borderx, int bordery,
                        int borderwidth, int borderheight, 
                        int x, int y, int runWidth, int runHeight, 
                        ColorMatcher &cm, ColorTransformer &ct, 
                        SortDirection sx, SortDirection sy, OuterLoop ol) 
{
    for (int i = x; i < borderwidth; i = i + runWidth) {
        for (int j = y; j < borderheight; j = j + runHeight) {
            int newWidth = i + runWidth > image.columns() ? image.columns() - i : runWidth;
            int newHeight = j + runHeight > image.rows() ? image.rows() - j : runHeight;
            // std::cout << i << ", " << j << std::endl;
            sortPixels(image, i, j, newWidth, newHeight, cm, ct, sx, sy, ol);
        }
    }
}


