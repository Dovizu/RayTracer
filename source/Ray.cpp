#ifndef RAY_H
#define RAY_H
#include <float.h>
#include "preHeader.h"

class Ray {
public:
    Point position;
    Vector direction; //This is not normalized
    float t_min, t_max;
    
    Ray() {
        position = Point(0,0,0);
        direction = Vector(0,0,0);
        t_min = 0.0;
        t_max = FLT_MAX;
    }
    
    Ray(Point start, Vector direction, float min, float max) {
        position = start;
        this->direction = direction;
        t_min = min;
        t_max = max;
    }
    
    bool valueAt(float t, Point* point) {
        if (t > t_min && t < t_max) {
            *point = position + t*(direction.array());
            return true;
        }
        return false;
    }
};

#endif
