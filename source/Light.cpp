#include "preHeader.h"
#include "LocalGeo.cpp"

class Light
{
public:
    Color color;
    Vector vector;
    Point location;
    LightSourceType lightType;

public:
    Light(Color clr, Vector vec, Point loc, LightSourceType type);
    void generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor);
};

Light::Light(Color clr, Vector vec, Point loc, LightSourceType type)
{
    color = clr;
    vector = vec;
    location = loc;
    lightType = type;
}

void Light::generateLightRay(LocalGeo& local, Ray* lray, Color* lcolor)
{
    *lcolor = color;
    if (lightType == LightSourcePoint) {
        Point localpos = local.position;
        Vector lvec = makeVec(localpos, location);
        lray->position = location;
        lray->direction = lvec;
        //Does t_max even matter?
    }
    if (lightType == LightSourceDirectional)
    {
        lray->position = local.position;
        lray->direction = vector;
        lray->t_max = FLT_MAX; //why is this infinite?
    }
}