#ifndef GEOPRIM_CPP
#define GEOPRIM_CPP

#include "preHeader.h"
#include "Transformation.cpp"
#include "Shape.cpp"
#include "Material.cpp"
#include "LocalGeo.cpp"
#include "BRDF.cpp"

class GeometricPrimitive : public Primitive {
public:
    Transformation *objToWorld;
    Transformation *worldToObj;
    Shape* shape;
    Material* mat;
    BoundingBox bb;
    bool bbSet;
    
    GeometricPrimitive(){
        shape = NULL;
        mat = NULL;
        objToWorld = NULL;
        worldToObj = NULL;
        bbSet = false;
    }
    GeometricPrimitive(Transformation *objToWorldTrans, Shape *shape, Material *mat) {
        objToWorld = objToWorldTrans;
        worldToObj = new Transformation((*objToWorld).m.inverse());
        this->shape = shape;
        this->mat = mat;
        bbSet = false;
    }

    void completeTransformationData(){
        if (objToWorld)
            worldToObj = new Transformation((*objToWorld).m.inverse());
    }
    bool intersect(Ray& ray, float* thit, Intersection* in)  {
        Ray oray = (*worldToObj)*ray;
        LocalGeo olocal;
        if (!shape->intersect(oray, thit, &olocal))  return false;
        in->primitive = this;
        in->localGeo = (*objToWorld)*olocal;
        return true;
    }
    
    bool intersectP(Ray& ray) {
        Ray oray = (*worldToObj)*ray;
        return shape->intersectP(oray);
    }
    
    void getBRDF(LocalGeo& local, BRDF* brdf) {
        mat->getBRDF(local, brdf);
    }
    
    void getCenter(Point *pt) {
        Point p;
        shape->getCenter(&p);
        *pt = (*objToWorld)*p;
    }
    
    void getBoundingBox(BoundingBox* bb) {
        if (!bbSet && shape) {
            BoundingBox unconverted;
            unconverted = shape->getBoundingBox();
            this->bb = (*objToWorld)*unconverted;
            bbSet = true;
        }
        *bb = (this->bb);
    }
    
    bool isLeftOf(Point& average, int axis) {
        Point transformed = (*worldToObj)*average;
        return shape->isLeftOf(transformed, axis);
    }

};

#endif