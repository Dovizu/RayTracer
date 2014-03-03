

class Sampler {
    int xPixels, yPixels;
    int xIndex, yIndex;
    
public:
    Sampler(int xPix, int yPix);
    bool getSample(Sample* s);
};

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
        return FALSE;
    }
    if (xIndex < xPixels)
    {
        (*s)(0) = xIndex + offset;
        (*s)(1) = yIndex + offset;
        xIndex++;
        return TRUE;
    }
    if (xIndex >= xPixels) {
        xIndex = 0;
        yIndex++;
        if (yIndex < yPixels) {
            (*s)(0) = xIndex + offset;
            (*s)(1) = yIndex + offset;
            xIndex++;
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        return FALSE;
    }
}