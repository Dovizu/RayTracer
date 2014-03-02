#include "preHeader.h"

class Intersection {
public:
    LocalGeo localGeo;
    Primitive* primitive;
    
    Intersection(LocalGeo local, Primitive* primitive) {
        localGeo = local;
        this.primitive = primitive;
    }
};