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
    
    BRDF(){};
};
#endif