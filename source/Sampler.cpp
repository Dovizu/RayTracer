

class Sampler {
    Point UL, UR, BR, BL;
    int xPixels, yPixels;
    int xIndex, yIndex;
    float imageHeight, imageWidth;
    float xUnit, yUnit;
    
public:
    Sampler(Point ul, Point ur, Point br, Point bl, int xPix, int yPix);
    bool getSample(Sample* s);
};

Sampler::Sampler(Point ul, Point ur, Point br, Point bl, int xPix, int yPix)
{
    UL = ul;
    UR = ur;
    BR = br;
    BL = bl;
    xPixels = xPix;
    yPixels = yPix;
    xIndex = 0;
    yIndex = 0;
    imageHeight = UL(0) - BL(0); //subtract xcoords
    imageWidth = UL(1) - BL(1); //subtract y coords
    xUnit = imageWidth / xPixels;
    yUnit = imageHeight / yPixels;
}

bool Sampler::getSample(Sample* s)
{
    if (xIndex >= xPixels && yIndex >= yPixels) {
        return FALSE;
    }
    if (xIndex < xPixels)
    {
        (*s)(0) = xIndex * xUnit;
        (*s)(1) = yIndex*yUnit;
        xIndex++;
        return TRUE;
    }
    if (xIndex >= xPixels) {
        xIndex = 0;
        yIndex++;
        if (yIndex < yPixels) {
            (*s)(0) = xIndex * xUnit;
            (*s)(1) = yIndex*yUnit;
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        return FALSE;
    }
}