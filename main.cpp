#include <iostream>
#include <cmath>
#include "AudioFile.h"
#include "bitmapImage.h"

struct RGB {
    char R;
    char G;
    char B;
};

RGB regulate(double from, double to, double value) {
    RGB rgb;
    long v = (value - to) / (from - to) * 0xffffff;
    rgb.R = v & 0xff;
    rgb.G = (v & (0xff << 8)) >> 8;
    rgb.B = (v & (0xff << 16)) >> 16;

    return rgb;
}

int main() {

    AudioFile<double> audioFile;
    if (!audioFile.load("test.wav")) {
        std::cerr << "fail to load test.wav";
    }

    std::cout << "channel count : " << audioFile.getNumChannels() << std::endl;
    std::cout << "sample count : " << audioFile.getNumSamplesPerChannel() << std::endl;
    unsigned int bitmapSize = sqrt(audioFile.getNumChannels() * audioFile.getNumSamplesPerChannel());
    std::cout << "bitmap size : " << bitmapSize << std::endl;

    double max = 0, min = 0;
    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++) {
        for (int channel = 0; channel < audioFile.getNumChannels(); channel++) {
            max = max > audioFile.samples[channel][i] ? max : audioFile.samples[channel][i];
            min = min < audioFile.samples[channel][i] ? min : audioFile.samples[channel][i];
        }
    }

    bitmap_image image(bitmapSize, bitmapSize);
    image_drawer drawer(image);
    int c = 0, s = 0;
    const int max_c = audioFile.getNumChannels();
    const int max_s = audioFile.getNumSamplesPerChannel();
    for (unsigned int i = 0; i < bitmapSize; i++) {
        for (unsigned int j = 0; j < bitmapSize; j++) {
            RGB rgb = regulate(max, min, audioFile.samples[c++][s++]);
            if (c == max_c) c = 0;
            if (s == max_s) s = 0;
            drawer.pen_color(rgb.R, rgb.G, rgb.B);
            drawer.plot_pixel(i, j);
        }
    }
    image.save_image("testout.bmp");

    return 0;
}