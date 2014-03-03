#ifndef RAY_H
#define RAY_H

#include "preHeader.h"

class Ray {
public:
    Point position;
    Vector direction;
    float t_min, t_max;
    
    Ray() {
        position = Point(0,0,0);
        direction = Vector(0,0,0);
        t_min = 0.0;
        t_max = 1.0;
    }
    
    Ray(Point start, Vector direction, float min, float max) {
        position = start;
        this->direction = direction;
        t_min = min;
        t_max = max;
    }
};

#endif
