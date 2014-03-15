#ifndef SAMPLER_CPP
#define SAMPLER_CPP

class Sampler {
public:
    int xPixels, yPixels;
    float xIndex, yIndex, unit, offset;
    int sqrt_rpp; //The square root of rays per pixel
    bool antialias = false;
    
public:
    Sampler();
    Sampler(int xPix, int yPix);
    Sampler(int xPix, int yPix,float sqrt_rpp);
    bool getSample(Sample* s);
    float jitter(float offset);
};

Sampler::Sampler()
{
    xPixels = 320;
    yPixels = 320;
    xIndex = 0.0f;
    yIndex = 0.0f;
    sqrt_rpp = 1.0f;
    unit = 1.0f;
    offset = 0.5f;
}

Sampler::Sampler(int xPix, int yPix)
{
    xPixels = xPix;
    yPixels = yPix;
    xIndex = 0.0f;
    yIndex = 0.0f;
    unit = 1.0f;
    offset = 0.5f;
    sqrt_rpp = 1.0f;
}

Sampler::Sampler(int xPix, int yPix, float sqrt_rpp)
{
    xPixels = xPix;
    yPixels = yPix;
    yIndex = 0;
    xIndex = 0;
    this->sqrt_rpp = sqrt_rpp;
    unit = 1.0f/sqrt_rpp;
    offset = unit/2;
}

bool Sampler::getSample(Sample* s)
{
    if (xIndex >= xPixels && yIndex >= yPixels) {
        return false;
    }
    if (xIndex < xPixels)
    {
        (*s)(0) = xIndex + offset;
        (*s)(1) = yIndex + offset;
        if(antialias){
            jitter(offset);
            jitter(offset);
        }
        xIndex+=unit;
        return true;
    }
    if (xIndex >= xPixels) {
        xIndex = 0;
        yIndex+=unit;
        if (yIndex < yPixels) {
            (*s)(0) = xIndex + offset;
            (*s)(1) = yIndex + offset;
            if (antialias) {
                (*s)(0)+=jitter(offset);
                (*s)(0)+=jitter(offset);
            }
            xIndex+=unit;
            return true;
        }
        return false;
    }
    else
    {
        return false;
    }
}

float Sampler::jitter(float offset)
{
    return -offset + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX))*2*offset;
}

#endif /* defined(SAMPLER_CPP) */