#ifndef MATERIAL_CPP
#define MATERIAL_CPP

#include "LocalGeo.cpp"
#include "BRDF.cpp"

class Material {
public:
    BRDF brdf;
    
    Material(){
    }
    Material(BRDF brdf){
        this->brdf = brdf;
    }
    
    /**
     *  Returns a constant BRDF, later when we want to support texture mapping, this needs to be modified
     *
     *  @param local LocalGeo (for texture mapping)
     *  @param brdf  BRDF pointer
     */
    void getBRDF(LocalGeo& local, BRDF* brdf) {
        *brdf = this->brdf;
    }
};

#endif