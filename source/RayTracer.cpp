#include "preHeader.h"

class Raytracer {
    AggregatePrimitive primitives;
    vector<Light> lights;
    int maxDepth;
    
public:
    Raytracer(AggregatePrimitive list, <vector<Light> lights);
    void trace(Ray& ray, int depth, Color* color);
    void setToBlack(Color* c);
};

Raytracer::Raytracer(AggregatePrimitive list, <vector<Light> lights, int maxDepth)
{
    primitives = list;
    this.lights = lights;
    this.maxDepth = maxDepth;
}

void Raytracer::trace(Ray& ray, int depth, Color* c)
{
    if (depth > maxDepth) {
        setToBlack(c);
        return;
    }
    float thit;
    Intersection in = Intersection();
    if(!primitives.intersect(ray, &thit, &in))
    {
        setToBlack(c);
        return;
    }
    BRDF brdf = 
    in.primitive->getBRDF(in.local, &brdf);
}

void setToBlack(Color* c)
{
    c(0) = 0.0;
    c(1) = 0.0;
    c(2) = 0.0;
}