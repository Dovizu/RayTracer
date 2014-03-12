#include "preHeader.h"
#include "test.cpp"
#include "AggregatePrimitive.cpp"

bool doesRender;
Point eye;
vector<Light> lights;
AggregatePrimitive world;
Sampler sampler;
Camera camera(Point(-1,  1, -1),
              Point(1,  1, -1),
              Point(1, -1, -1),
              Point(-1, -1, -1),
              Point(0,0,0),
              320,
              320);
Raytracer raytracer(world, lights, 1);
Film film(320, 320);

void render() {
    raytracer.lights = lights;
    raytracer.primitives = world;
    Sample sample;
    Ray ray;
    while (sampler.getSample(&sample)) {
        Color color = {0,0,0};
        camera.generateRay(sample, &ray);
        if (sample(0)==160.5 && sample(1)==160.5) {
            //break
        }
        raytracer.trace(ray, 0, &color);
        cout << "Color at: x:" << sample(0) << " y:" << sample(1) << " ";
        printf("%f %f %f \n", color(0), color(1), color(2));
        film.commit(sample, color);
    }
    film.writeImage("testImage.bmp");
}

int main(int argc, char *argv[]) {
    
    vector<CmdLineOptResult> *results;
    /*
     -t         (comprehensive testing)
     -tobj      (test tinyObjLoader)
     -tsampler  (test Sampler)
     -tcam      (test Camera)
     -tsintersect   (test Sphere Intersection)
     -ttintersect   (test Triangle Intersection)
     -ttrans    (test Transformation)
     -tparser basePath   (test parser)
     -render obj  (render image using obj files)
     -render manual (render image using manual input)
     -sphere radius x y z ka kd kr ks sp
     -triangle x1 y1 z1 x2 y2 z2 x3 y3 z3 r g b
     -light r g b x y z
     -dl    r g b x y z
     */
    string options = "-t(0)-tobj(0)-tsampler(0)-tcam(0)-tsintersect(0)-ttintersect(0)-ttrans(0)-tparser(1)-render(1)-sphere(9)-triangle(12)-light(6)-dl(6)";
    getCmdLineOptions(argc, argv, options, &results);
    for (auto & result : *results) {
        if (result.optName.compare("-t") == 0) {
            testAll();
        }
        if (result.optName.compare("-tobj") == 0) {
            testTinyObjLoader();
        }
        if (result.optName.compare("-tsampler") == 0) {
            testSampler();
        }
        if(result.optName.compare("-tcam") == 0){
            testCamera();
        }
        if (result.optName.compare("-tsintersect") == 0) {
            testSphereIntersection();
        }
        if (result.optName.compare("-ttintersect") == 0) {
            testTriangleIntersection();
        }
        if (result.optName.compare("-ttrans") == 0) {
            testTransformation();
        }
        if (result.optName.compare("-tparser") == 0) {
            string basePath = result.args->at(0);
            testParser(basePath);
        }
        if (result.optName.compare("-render") == 0) {
            doesRender = true;
        }
        if (result.optName.compare("-sphere") == 0) {
            Sphere *sphere = new Sphere(stof(result.args->at(0)),0.0,0.0,0.0);
            Translation3f transform(stof(result.args->at(1)),
                                  stof(result.args->at(2)),
                                  stof(result.args->at(3)));
            float ka = stof(result.args->at(4));
            float kd = stof(result.args->at(5));
            float kr = stof(result.args->at(6));
            float ks = stof(result.args->at(7));
            int sp = stoi(result.args->at(8));
            Material *mat = new Material(BRDF(Color(kd, kd, kd),
                                              Color(ks, ks, ks),
                                              Color(ka, ka, ka),
                                              Color(kr, kr, kr),
                                              sp));
            GeometricPrimitive *prim = new GeometricPrimitive(Transformation(transform), sphere, mat);
            world.primList.push_back(prim);
        }
        if (result.optName.compare("-light") == 0) {
            Color liteColor(stof(result.args->at(0)),
                            stof(result.args->at(1)),
                            stof(result.args->at(2)));
            Point location(stof(result.args->at(3)),
                           stof(result.args->at(4)),
                           stof(result.args->at(5)));
            lights.push_back(Light(liteColor, Vector(), location, LightSourcePoint));
        }
        if (result.optName.compare("-dl") == 0) {
            Color liteColor(stof(result.args->at(0)),
                            stof(result.args->at(1)),
                            stof(result.args->at(2)));
            Vector direction(stof(result.args->at(3)),
                           stof(result.args->at(4)),
                           stof(result.args->at(5)));
            lights.push_back(Light(liteColor, direction, Point(), LightSourceDirectional));
        }
    }
    
    if (doesRender) {
        render();
    }
    return 0;
}