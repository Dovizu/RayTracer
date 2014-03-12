#include "preHeader.h"
#include "AggregatePrimitive.cpp"
#include "Light.cpp"
#include "Intersection.cpp"
#include "BRDF.cpp"
#include "LocalGeo.cpp"

extern Point eye;

class Raytracer {
public:
    AggregatePrimitive primitives;
    vector<Light> lights;
    int maxDepth;
    
    Raytracer (){};
    Raytracer(AggregatePrimitive list, vector<Light> lights, int maxDepth);
    void trace(Ray& ray, int depth, Color* color);
    void setToBlack(Color* c);
    Color shading(LocalGeo &local, BRDF &brdf, Ray &lray, Color &lcolor);
    Ray createReflectRay(LocalGeo &local, Ray &ray);
};

Raytracer::Raytracer(AggregatePrimitive list, vector<Light> lights, int maxDepth)
{
    primitives = list;
    this->lights = lights;
    this->maxDepth = maxDepth;
}

void Raytracer::trace(Ray& ray, int depth, Color* color)
{
    if (depth > maxDepth) {
        setToBlack(color);
        return;
    }
    float thit;
    Intersection in = Intersection();
    if(!primitives.intersect(ray, &thit, &in))
    {
        setToBlack(color);
        return;
    }
    
    //Obtain brdf at intersection point
    BRDF brdf;
    in.primitive->getBRDF(in.localGeo, &brdf);
    
    //There is an intersection; loop through all light sources
    for (int i = 0; i < lights.size(); i++) {
        Ray lray = Ray();
        Color lcolor = {0,0,0};
        lights[i].generateLightRay(in.localGeo, &lray, &lcolor);
        if (!primitives.intersectP(lray)) {
            *color += shading(in.localGeo,brdf,lray,lcolor);
        }
    }
    if(brdf.kr(0) + brdf.kr(1)+brdf.kr(2) > 0){
        Ray reflectRay = createReflectRay(in.localGeo, ray);
        
        //Make a recursive call to trace the reflected ray
        Color tempColor = {0,0,0};
        trace(reflectRay, depth+1, &tempColor);
        *color += brdf.kr*tempColor;
    }
}

void Raytracer::setToBlack(Color* c)
{
    (*c)(0) = 0;
    (*c)(1) = 0;
    (*c)(2) = 0;
}

Color Raytracer::shading(LocalGeo &local, BRDF &brdf, Ray &lray, Color &lcolor)
{
    Color colorValue;
    
    Vector n = local.surfaceNormal;
    Vector l = lray.direction.normalized(); //surface->light
    float NDotL = n.dot(l);
    Vector r = -l + 2*NDotL*n; //The reflection ray
    r = r.normalized();
    Vector v = makeVec(local.position,eye).normalized(); //The view vector
    Color diffuse_comp = brdf.kd*lcolor*max<float>(NDotL, 0.0f);
    Color spec_comp = brdf.ks*lcolor*pow(max<float>(r.dot(v),0.0),brdf.sp);
    Color ambient_comp = brdf.ka*lcolor;
    return diffuse_comp + spec_comp + ambient_comp;
}

Ray Raytracer::createReflectRay(LocalGeo &local, Ray &ray)
{
    Vector n = local.surfaceNormal;
    Vector l = -ray.direction.normalized();
    //What is the best way to make sure reflect ray doesn't think it intersects at object? Adjust tmin?
    return Ray(local.position, -l + 2*n.dot(l)*n, 0.01f, FLT_MAX);
}