#include "preHeader.h"

class Raytracer {
    AggregatePrimitive primitives;
    
public:
    void trace(Ray& ray, int depth, Color* color);
};