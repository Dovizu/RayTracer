#ifndef __Assignment2__AABBNode__
#define __Assignment2__AABBNode__

#include "preHeader.h"
#include "GeometricPrimitive.cpp"

class AABBNode {
public:
    GeometricPrimitive *shape;
    AABBNode *left
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
};

#endif /* defined(__Assignment2__AABBNode__) */