#ifndef INTER_CPP
#define INTER_CPP

#include "preHeader.h"
#include "Primitive.cpp"
#include "LocalGeo.cpp"
class Primitive;

class Intersection {
public:
    LocalGeo localGeo;
    Primitive* primitive;
    Intersection(){
    }
    Intersection(LocalGeo local, Primitive* primitive) {
        localGeo = local;
        this->primitive = primitive;
    }
};

#endif