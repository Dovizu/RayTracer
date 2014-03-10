#ifndef BRDF_CPP
#define BRDF_CPP

#include "preHeader.h"

class BRDF {
public:
    Color kd, ks, ka, kr;
    
    BRDF(Color d, Color s, Color a, Color r) {
        kd = d;
        ks = s;
        ka = a;
        kr = r;
    }
    
    BRDF()
    {
        kd = 0.0;
        ks = 0.0;
        ka = 0.0;
        kr = 0.0;
    }
};

#endif