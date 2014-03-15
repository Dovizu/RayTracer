#ifndef __Assignment2__AABBNode__
#define __Assignment2__AABBNode__

#include "preHeader.h"
#include "GeometricPrimitive.cpp"

class AABBNode {
public:
    vector<GeometricPrimitive*> *shape;
    AABBNode *left;
    AABBNode *right;
    BoundingBox bb;
    int longestAxisIndex;
    bool isLeaf;
    
    AABBNode() {
        shape = NULL;
    }
    
    AABBNode(vector<GeometricPrimitive*>& primList, int depth) {
        Point average = {0,0,0};
        Point min = {FLT_MAX, FLT_MAX, FLT_MAX};
        Point max = {FLT_MIN, FLT_MIN, FLT_MIN};
        vector<GeometricPrimitive*> leftList;
        vector<GeometricPrimitive*> rightList;
        
        for (auto primPtr : primList) {
            Point center;
            primPtr->getCenter(&center);
            average += center;
            BoundingBox box;
            primPtr->getBoundingBox(&box);
            min(0) = minf(min(0), box.min(0));
            min(1) = minf(min(1), box.min(1));
            min(2) = minf(min(2), box.min(2));
            max(0) = maxf(max(0), box.max(0));
            max(1) = maxf(max(1), box.max(1));
            max(2) = maxf(max(2), box.max(2));
        }
        //set bounding box
        bb.min = min;
        bb.max = max;
        
        if (primList.size() <= 6) {
            for (auto primPtr : primList) {
                shape->push_back(primPtr);
            }
            isLeaf = true;
        }
        
        findLongestAxisIndex();
        average /= (float)primList.size();
        for (auto primPtr : primList) {
            if (primPtr->isLeftOf(average, longestAxisIndex)) {
                leftList.push_back(primPtr);
            }else{
                rightList.push_back(primPtr);
            }
        }
        left = new AABBNode(leftList, depth+1);
        right = new AABBNode(rightList, depth+1);
    }
    
    void findLongestAxisIndex() {
        float xDis = bb.max(0) - bb.min(0);
        float yDis = bb.max(1) - bb.min(1);
        float zDis = bb.max(2) - bb.max(2);
        
        float biggest = maxf(xDis, maxf(yDis, zDis));
        
        if (biggest == xDis) {
            longestAxisIndex = 0;
        }else if (biggest == yDis) {
            longestAxisIndex = 1;
        }else if (biggest == zDis) {
            longestAxisIndex = 2;
        }else {
            longestAxisIndex = 0;
        }
    }
};

#endif /* defined(__Assignment2__AABBNode__) */