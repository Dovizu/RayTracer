#ifndef PRIM_CPP
#define PRIM_CPP

#include "preHeader.h"
#include "Ray.cpp"
#include "Intersection.cpp"
#include "BRDF.cpp"
#include "LocalGeo.cpp"
class Intersection;

class Primitive {
public:
    virtual bool intersect(Ray& ray, float* thit, Intersection* in) = 0;
    virtual bool intersectP(Ray& ray) = 0;
    virtual void getBRDF(LocalGeo& local, BRDF* brdf) = 0;
};

#endif