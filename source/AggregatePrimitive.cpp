#ifndef AGGREGATEPRIMITIVE
#define AGGREGATEPRIMITIVE

#include "preHeader.h"
#include "Ray.cpp"
#include "Intersection.cpp"
#include "BRDF.cpp"
#include "float.h"

class AggregatePrimitive : public Primitive{
public:
    vector<Primitive*> primList;
    
public:
    AggregatePrimitive();
    AggregatePrimitive(vector<Primitive*> list);
    bool intersect(Ray& ray, float* thit, Intersection* in);
    bool intersectP(Ray& ray);
    void getBRDF(LocalGeo& local, BRDF* brdf){
        exit(1);
        //This should never get called, because in->primitve
        //will never be an aggregate primitive
    }
};

AggregatePrimitive::AggregatePrimitive()
{
}

AggregatePrimitive::AggregatePrimitive(vector<Primitive*> list)
{
    primList = list;
}

bool AggregatePrimitive::intersect(Ray& ray, float* thit, Intersection* in)
{
    bool foundIntersect = false;
    *thit = FLT_MAX;
    for (auto primitive : primList)
    {
        float newThit;
        Intersection newIn;
        if(primitive->intersect(ray, &newThit, &newIn))
        {
            foundIntersect = true;
            if (newThit < *thit)
            {
                *thit = newThit;
                *in = newIn;
            }
        }
    }
    return foundIntersect;
}

bool AggregatePrimitive::intersectP(Ray& ray)
{
    for (auto &primitive: primList)
    {
        if (primitive->intersectP(ray)) {
            return true;
        }
    }
    return false;
}

#endif