#include "preHeader.h"

class LocalGeo {
public:
    Point position;
    Vector surfaceNormal;
    
    LocalGeo(Point pos, Vector normal) {
        position = pos;
        surfaceNormal = normal;
    }
};