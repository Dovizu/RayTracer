#ifndef __Assignment2__AABBNode__
#define __Assignment2__AABBNode__

#include "preHeader.h"
#include "GeometricPrimitive.cpp"

class AABBNode {
public:
    vector<Primitive*> *shapes;
    AABBNode *left;
    AABBNode *right;
    BoundingBox bb;
    int longestAxisIndex;
    bool isLeaf;
    
    AABBNode() {
        shapes = NULL;
        left = NULL;
        right = NULL;
    }
    
    AABBNode(vector<Primitive*>& primList, int depth) {
        Point average = {0,0,0};
        Point min = {FLT_MAX, FLT_MAX, FLT_MAX};
        Point max = {FLT_MIN, FLT_MIN, FLT_MIN};
        vector<Primitive*> leftList;
        vector<Primitive*> rightList;
        shapes = new vector<Primitive*>();
        
        for (auto & primPtr : primList) {
            Point center;
            primPtr->getCenter(&center);
            average += center;
            BoundingBox box = {Point(0,0,0), Point(0,0,0)};
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
        
        
        if (primList.size() < 6) { //took out depth check
            if (primList.size() != 0) {
                for (auto & primPtr : primList) {
                    shapes->push_back(primPtr);
//                    printf("Leaf node at: %d push primitive\n", depth);
                }
            }
            isLeaf = true;
            return;
        }
        
        findLongestAxisIndex();
        cout << "avgb4cmp: " << average << endl;
        average /= (float)primList.size();
        cout << "average: " << average << endl;
        cout << "axis: " << longestAxisIndex << endl;
        for (auto & primPtr : primList) {
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
        float zDis = bb.max(2) - bb.min(2);
        
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
    
    bool intersect(Ray& ray, float* thit, Intersection* in)  {
        bool foundIntersection = false;
        *thit = FLT_MAX;
        if (intersectP(ray)) {
            if (isLeaf) {
                for (auto &primPtr : *shapes) {
                    float newThit;
                    Intersection newIn;
                    if (primPtr->intersect(ray, &newThit, &newIn)) {
                        foundIntersection = true;
                        if (newThit < *thit) {
                            *thit = newThit;
                            *in = newIn;
                        }
                    }
                }
                return foundIntersection;
            }else{
                float newThitLeft = FLT_MAX;
                float newThitRight = FLT_MAX;
                Intersection newInLeft;
                Intersection newInRight;
                bool left_intercept = left->intersectP(ray);
                bool right_intercept = right->intersectP(ray);
                if (left_intercept) {
                    if (left->intersect(ray, &newThitLeft, &newInLeft)) {
                        foundIntersection = true;
                    }
                }
                if (right_intercept){
                    if (right->intersect(ray, &newThitRight, &newInRight)) {
                        foundIntersection = true;
                    }
                }
                if (left_intercept || right_intercept){
                    if (newThitLeft < newThitRight) {
                        *thit = newThitLeft;
                        *in = newInLeft;
                    }else{
                        *thit = newThitRight;
                        *in = newInRight;
                    }
                }
            }
        }
        return foundIntersection;
    }
    
    //checks if you hit BOUNDING BOX of tree
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
        
        if ((txmin > tymax) || (tymin > txmax)) {
            return false;
        }
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
        if ((txmin > tzmax) || (tzmin > txmax)) {
            return false;
        }
        if (tzmin > txmin)
            txmin = tzmin;
        if (tzmax < txmax)
            txmax = tzmax;
        if ((txmin > ray.t_max) || (txmax < ray.t_min)) {
            return false;
        }
        return true;
        //if (ray.t_min < txmin) ray.t_min = txmin;
        //if (ray.t_max > txmax) ray.t_max = txmax;
        //Potentially unnecessary ray reassignment values.
        //return (left->intersectP(ray) || right->intersectP(ray));
    }
    
    //checks if you hit a PRIMITVE in AABB tree
    bool intersectP2(Ray &ray)
    {
        if (isLeaf) {
            for (auto &primitive: *shapes)
            {
                if (primitive->intersectP(ray))
                {
                    return true;
                }
                return false;
            }
        }
        
        else{
            bool right_intercept = false;
            bool left_intercept = false;
            if (right->intersectP(ray)) {
                right_intercept = right->intersectP2(ray);
            }
            if (left->intersectP(ray)) {
                left_intercept = left->intersectP2(ray);
            }
            return right_intercept || left_intercept;
        }
    }
};

#endif /* defined(__Assignment2__AABBNode__) */