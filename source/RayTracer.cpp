#include "preHeader.h"
#include "AggregatePrimitive.cpp"
#include "Light.cpp"
#include "Intersection.cpp"
#include "BRDF.cpp"
#include "LocalGeo.cpp"
#include "AABBNode.cpp"
#include "Primitive.cpp"
#include "time.h"
#include <sys/timeb.h>

class Raytracer {
public:
    AggregatePrimitive primitives;
    vector<Light> lights;
    int maxDepth;
    Point eye;
    AABBNode *tree;
    
    Raytracer (){};
    Raytracer(AggregatePrimitive &list, vector<Light> &lights, int maxDepth);
    void trace(Ray& ray, int depth, Color* color);
    Color shading(LocalGeo &local, BRDF &brdf, Ray &lray, Color &lcolor);
    Color shadeAmbient(BRDF &brdf, Color &lcolor);
    Ray createReflectRay(LocalGeo &local, Ray &ray);
    void generateTree();
};

Raytracer::Raytracer(AggregatePrimitive& list, vector<Light> &lights, int maxDepth)
{
    primitives = list;
    this->lights = lights;
    this->maxDepth = maxDepth;
}

void Raytracer::trace(Ray& ray, int depth, Color* color)
{
    //struct timeb start, end;
    //ftime(&start);
    if (depth > maxDepth) {
        *color = {0,0,0};
        return;
    }
    float thit;
    Intersection in = Intersection();
    if(!tree->intersect(ray, &thit, &in))
    {
        *color = {0,0,0};
        ftime(&end);
        double diff = 1000*(end.time - start.time)
        + (end.millitm - start.millitm);
        //printf ("Elasped MISS time is %.2lf milliseconds.\n", diff );
        return;
    }
    
    //Obtain brdf at intersection point
    BRDF brdf;
    in.primitive->getBRDF(in.localGeo, &brdf);
    
    //There is an intersection; loop through all light sources
    for (size_t i = 0; i < lights.size(); i++) {
        Ray lray = Ray();
        Color lcolor = {0,0,0};
        lights[i].generateLightRay(in.localGeo, &lray, &lcolor);
        if (!tree->intersectP2(lray)) {
            *color += shading(in.localGeo,brdf,lray,lcolor);
        }else{
            *color += shadeAmbient(brdf, lcolor);
        }
    }
    if(brdf.kr(0) + brdf.kr(1)+brdf.kr(2) > 0){
        Ray reflectRay = createReflectRay(in.localGeo, ray);
        
        //Make a recursive call to trace the reflected ray
        Color tempColor = {0,0,0};
        trace(reflectRay, depth+1, &tempColor);
        *color += brdf.kr*tempColor;
    }
    //ftime(&end);
    //double diff = 1000*(end.time - start.time)
                  + (end.millitm - start.millitm);
    //printf ("Elasped time is %.2lf milliseconds.\n", diff );

}

void Raytracer::generateTree() {
    tree = new AABBNode((primitives.primList), 0);
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

Color Raytracer::shadeAmbient(BRDF &brdf, Color &lcolor)
{
    return brdf.ka*lcolor;
}

Ray Raytracer::createReflectRay(LocalGeo &local, Ray &ray)
{
    Vector n = local.surfaceNormal;
    Vector l = -ray.direction.normalized();
    //What is the best way to make sure reflect ray doesn't think it intersects at object? Adjust tmin?
    return Ray(local.position, -l + 2*n.dot(l)*n, 0.01f, FLT_MAX);
}