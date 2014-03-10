#include "preHeader.h"
#include "Ray.cpp"
#include "Intersection.cpp"
#include "BRDF.cpp"

class AggregatePrimitive : Primitive{
public:
    vector<Primitive*> primList;
    
public:
    AggregatePrimitive(vector<Primitive*> list);
    bool intersect(Ray& ray, float* thit, Intersection* in);
    bool intersectP(Ray& ray);
    void getBRDF(LocalGeo& local, BRDF* brdf){
        exit(1);
        //This should never get called, because in->primitve
        //will never be an aggregate primitive
    }
};

AggregatePrimitive::AggregatePrimitive(vector<Primitive*> list)
{
    primList = list;
}

bool AggregatePrimitive::intersect(Ray& ray, float* thit, Intersection* in)
{
    bool foundIntersect = false;
    for (auto &primitive : primList)
    {
        float newThit;
        Intersection newIn;
        if(primitive->intersect(ray, &newThit, &newIn))
        {
            if(!foundIntersect)
            {
                foundIntersect = true;
                *thit = newThit;
                *in = newIn;
            }
            else
            {
                if (newThit < *thit)
                {
                    *thit = newThit;
                    *in = newIn;
                }
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