#ifndef SAMPLER_CPP
#define SAMPLER_CPP

class Sampler {
    int xPixels, yPixels;
    int xIndex, yIndex;
    
public:
    Sampler();
    Sampler(int xPix, int yPix);
    bool getSample(Sample* s);
};

Sampler::Sampler()
{
    xPixels = 320;
    yPixels = 320;
    xIndex = 0;
    yIndex = 0;
}

Sampler::Sampler(int xPix, int yPix)
{
    xPixels = xPix;
    yPixels = yPix;
    xIndex = 0;
    yIndex = 0;
}

bool Sampler::getSample(Sample* s)
{
    float offset = 0.5;
    if (xIndex >= xPixels && yIndex >= yPixels) {
        return false;
    }
    if (xIndex < xPixels)
    {
        (*s)(0) = xIndex + offset;
        (*s)(1) = yIndex + offset;
        xIndex++;
        return true;
    }
    if (xIndex >= xPixels) {
        xIndex = 0;
        yIndex++;
        if (yIndex < yPixels) {
            (*s)(0) = xIndex + offset;
            (*s)(1) = yIndex + offset;
            xIndex++;
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}

#endif /* defined(SAMPLER_CPP) */