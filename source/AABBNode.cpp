#ifndef __Assignment2__AABBNode__
#define __Assignment2__AABBNode__

#include "preHeader.h"
#include "GeometricPrimitive.cpp"

class AABBNode {
public:
    GeometricPrimitive *shape;
    AABBNode *left;
    AABBNode *right;
    BoundingBox bb;
    
    AABBNode() {
        shape = NULL;
    }
    
    AABBNode(vector<GeometricPrimitive*>& primList, int depth) {
        Point average = {0,0,0};
        Array3f min = {FLT_MAX, FLT_MAX, FLT_MAX};
        Array3f max = {FLT_MIN, FLT_MIN, FLT_MIN};
        for (auto primPtr : primList) {
            Point center;
            primPtr->getCenter(&center);
            average += center;
            
        }
    }
    
    bool isLeaf() {
        return shape == NULL;
    }
    
    bool intersect(Ray& ray, float* thit, Intersection* in)  {
        return true;
    }
    
    bool intersectP(Ray& ray) {
        Vector dir = ray.direction;
        Point pos = ray.position;
        Point min = bb.min;
        Point max = bb.max;
        float txmin, txmax, tymin, tymax, tzmin, tzmax;
        float ax = 1/X(dir);
        float ay = 1/Y(dir);
        float az = 1/Z(dir);
        if (ax >= 0) {
            txmin = (X(min) - X(pos))*ax;
            txmax = (X(max) - X(pos))*ax;
        } else {
            txmin = (X(max) - X(pos))*ax;
            txmax = (X(min) - X(pos))*ax;
        }
        
        if (txmin > txmax) swap(txmin, txmax);
        if (ay >= 0) {
            tymin = (Y(min) - Y(pos))*ay;
            tymax = (Y(max) - Y(pos))*ay;
        } else {
            tymin = (Y(max) - Y(pos))*ay;
            tymax = (Y(min) - Y(pos))*ay;
        }
        if (tymin > tymax) swap(tymin, tymax);
        
        if ((txmin > tymax) || (tymin > txmax))
            return false;
        if (tymin > txmin)
            txmin = tymin;
        if (tymax < txmax)
            txmax = tymax;
        if (az >= 0) {
            tzmin = (Z(min) - Z(pos))*az;
            tzmax = (Z(max) - Z(pos))*az;
        } else {
            tzmin = (Z(max) - Z(pos))*az;
            tzmax = (Z(min) - Z(pos))*az;
        }
        if (tzmin > tzmax) swap(tzmin, tzmax);
        if ((txmin > tzmax) || (tzmin > txmax))
            return false;
        if (tzmin > txmin)
            txmin = tzmin;
        if (tzmax < txmax)
            txmax = tzmax;
        if ((txmin > ray.t_max) || (txmax < ray.t_min)) return false;
        //if (ray.t_min < txmin) ray.t_min = txmin;
        //if (ray.t_max > txmax) ray.t_max = txmax;
        //Potentially unnecessary ray reassignment values.
        return true;
    }
};

#endif /* defined(__Assignment2__AABBNode__) */