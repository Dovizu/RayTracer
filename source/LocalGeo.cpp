#ifndef LOCALGEO_CPP
#define LOCALGEO_CPP

#include "preHeader.h"

class LocalGeo {
public:
    Point position;
    Vector surfaceNormal;
    
    LocalGeo() {
        position = Point(0,0,0);
        surfaceNormal = Vector(0,0,0);
    }
    LocalGeo(Point pos, Vector normal) {
        position = pos;
        surfaceNormal = normal;
    }
};

#endif