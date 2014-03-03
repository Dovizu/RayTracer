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
    virtual bool intersect(Ray& ray, float* thit, Intersection* in);
    virtual bool intersectP(Ray& ray);
    virtual void getBRDF(LocalGeo& local, BRDF* brdf);
};

#endif