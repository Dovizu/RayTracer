#ifndef BRDF_CPP
#define BRDF_CPP

#include "preHeader.h"

class BRDF {
public:
    Color kd, ks, ka, kr;
    int sp;
    
    BRDF(){}
    BRDF(Color d, Color s, Color a, Color r, int p) {
        kd = d;
        ks = s;
        ka = a;
        kr = r;
        sp = p;
    }
};

#endif