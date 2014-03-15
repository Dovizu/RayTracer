#ifndef FILM_CPP
#define FILM_CPP

#include "preHeader.h"

class Film
{
public:
    int xPixels;
    int yPixels;
    float rpp = 1.0f;
    Color* pixelColors;
    
public:
    Film(int xResolution, int yResolution);
    //Will write the color to (sample.x, sample.y) on the image
    void commit(Sample& sample, Color& color);
    //Output image to file
    void writeImage(const char* filename);
};

Film::Film(int xResolution, int yResolution)
{
    xPixels = xResolution;
    yPixels = yResolution;
    pixelColors = new Color[xPixels*yPixels];
    for (int i = 0; i < yPixels; i++) {
        for (int j = 0; j<xPixels; j++) {
            pixelColors[j + i*yPixels] = {0,0,0};
        }
    }
}

void Film::commit(Sample& sample, Color& color)
{
    int xcoord = (int) sample(0);
    int ycoord = (int) sample(1);
    pixelColors[xcoord + xPixels*ycoord] += color/rpp;
}

void Film::writeImage(const char* filename)
{
    CImg<float> image(xPixels, yPixels, 1, 3, 0);
    for (int i = 0; i < xPixels; i++) {
        for (int j = 0; j < yPixels; j++) {
            Color pixelColor = pixelColors[i + xPixels*j];
            image(i,j,0) = pixelColor(0); //sets red
            image(i,j,1) = pixelColor(1); //sets green
            image(i,j,2) = pixelColor(2); //sets blue
        }
    }
    //image.display("Image to be written");
    image.normalize(0,255);
    image.save(filename);
    
}

#endif