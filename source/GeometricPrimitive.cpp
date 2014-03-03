#ifndef GEOPRIM_CPP
#define GEOPRIM_CPP

#include "preHeader.h"
#include "Transformation.cpp"
#include "Shape.cpp"
#include "Material.cpp"
#include "LocalGeo.cpp"
#include "BRDF.cpp"

class GeometricPrimitive : Primitive {
public:
    Transformation objToWorld, worldToObj;
    Shape* shape;
    Material* mat;

    /*
    bool intersect(Ray& ray, float* thit, Intersection* in)  {
        Ray oray = worldToObj*ray;
        LocalGeo olocal;
        if (!shape->intersect(oray, thit, &olocal))  return false;
        in->primitive = this;
        in->localGeo = objToWorld*olocal;
        return true;
    }
    
    bool intersectP(Ray& ray) {
        Ray oray = worldToObj*ray;
        return shape->intersectP(oray);
    }
    
    void getBRDF(LocalGeo& local, BRDF* brdf) {
        mat->getBRDF(local, brdf);
    }
     */
};

#endif