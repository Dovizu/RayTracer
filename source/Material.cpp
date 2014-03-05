#ifndef MATERIAL_CPP
#define MATERIAL_CPP

#include "LocalGeo.cpp"
#include "BRDF.cpp"

class Material {
private:
    BRDF _brdf;
public:
    /**
     *  Returns a constant BRDF, later when we want to support texture mapping, this needs to be modified
     *
     *  @param local LocalGeo (for texture mapping)
     *  @param brdf  BRDF pointer
     */
    void getBRDF(LocalGeo& local, BRDF* brdf) {
        *brdf = _brdf;
    }
};

#endif