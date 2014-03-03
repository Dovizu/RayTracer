#ifndef SHAPE_CPP
#define SHAPE_CPP

#include "preHeader.h"
#include "Ray.cpp"
#include "LocalGeo.cpp"

class Shape {
public:
    // Test if ray intersects with the shape or not (in object space), if so,
    // return intersection point and normal
    bool intersect(Ray& ray, float* thit, LocalGeo* local);
    
    // Same as intersect, but just return whether there is any intersection or
    // not
    bool intersectP(Ray& ray);
    // Triangle and Sphere are probably best implemented here
    // The intersection with the ray at t outside the range [t_min, t_max]
    // should return false.
};

#endif