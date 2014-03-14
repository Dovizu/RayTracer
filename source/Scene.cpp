#include "preHeader.h"
#include "test.cpp"
#include "AggregatePrimitive.cpp"

extern bool verbose;

class Scene {
public:
    float resolution;
    int maxDepth;
    Point UL;
    Point UR;
    Point LR;
    Point LL;
    Point eye;
    
    vector<Light> lights;
    AggregatePrimitive *world;
    Sampler *sampler;
    Camera *camera;
    Raytracer *raytracer;
    Film *film;
    
    Scene() {
        float defaultResolution = 160;
        UL = Point(-1,  1, -1);
        UR = Point(1,  1, -1);
        LR = Point(1, -1, -1);
        LL = Point(-1, -1, -1);
        eye = Point(0,0,0);
        maxDepth = 1;
        resolution = defaultResolution;
        world = new AggregatePrimitive();
        sampler = new Sampler(defaultResolution, defaultResolution);
        camera = new Camera(UL, UR, LR, LL, eye, defaultResolution, defaultResolution);
        film = new Film(defaultResolution, defaultResolution);
    }
    
    Scene(Point UL,
          Point UR,
          Point LR,
          Point LL,
          Point eye,
          float resolution,
          int maxDepth){
        this->UL = UL;
        this->UR = UR;
        this->LR = LR;
        this->LL = LL;
        this->eye = eye;
        this->resolution = resolution;
        this->maxDepth = maxDepth;
        world = new AggregatePrimitive();
        sampler = new Sampler(resolution, resolution);
        camera = new Camera(UL, UR, LR, LL, eye, resolution, resolution);
        film = new Film(resolution, resolution);
    }
    
    void render() {
        raytracer = new Raytracer(*world, lights, maxDepth);
        raytracer->eye = eye;
        Sample sample;
        Ray ray;
        int currentPercentage = 0;
        while (sampler->getSample(&sample)) {
            Color color = {0,0,0};
            camera->generateRay(sample, &ray);
            raytracer->trace(ray, 0, &color);
            if (verbose) {
                cout << "Color at: x:" << sample(0) << " y:" << sample(1) << " ";
                printf("%f %f %f \n", color(0), color(1), color(2));
            }
            film->commit(sample, color);
            //Report progress
            int percentage = (int)((sample(0)+sample(1)*resolution)/sqr(resolution)*100.0);
            if (percentage != currentPercentage) {
                currentPercentage = percentage;
                printf("Progress: %d%%\n", (currentPercentage));
            }
        }
        film->writeImage("output.png");
    }
};

int main(int argc, char *argv[]) {
    
    Scene scene;
    vector<CmdLineOptResult> *results;
    bool doesRender;
    bool automatic;
    string basePath;
    /*
     //testing
     -t         (comprehensive testing)
     -tobj      (test tinyObjLoader)
     -tsampler  (test Sampler)
     -tcam      (test Camera)
     -tsintersect   (test Sphere Intersection)
     -ttintersect   (test Triangle Intersection)
     -ttrans    (test Transformation)
     -tparser basePath   (test parser)
     
     //rendering
     -sphere radius x y z ka kd kr ks sp
     -triangle x1 y1 z1 x2 y2 z2 x3 y3 z3 x y z ka kd kr ks sp
     -pl r g b x y z
     -dl r g b x y z
     -render manual | auto
     -path path/to/object/
     */
    string options = "-t(0)-tobj(0)-tsampler(0)-tcam(0)-tsintersect(0)-ttintersect(0)-ttrans(0)-tparser(1)-render(1)-sphere(9)-triangle(17)-pl(6)-dl(6)-path(1)-verbose(0)";
    getCmdLineOptions(argc, argv, options, &results);
    for (auto & result : *results) {
        if (result.optName.compare("-verbose")==0) {
            verbose = true;
        }
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
            automatic = result.args->at(0).compare("auto") == 0;
        }
        if (result.optName.compare("-path")==0) {
            basePath = result.args->at(0);
        }
        if (result.optName.compare("-triangle")==0) {
            Triangle *tri = new Triangle(Point(stof(result.args->at(0)),
                                               stof(result.args->at(1)),
                                               stof(result.args->at(2))),
                                         Point(stof(result.args->at(3)),
                                               stof(result.args->at(4)),
                                               stof(result.args->at(5))),
                                         Point(stof(result.args->at(6)),
                                               stof(result.args->at(7)),
                                               stof(result.args->at(8))));
            Translation3f translate(stof(result.args->at(9)),
                                    stof(result.args->at(10)),
                                    stof(result.args->at(11)));
            float ka = stof(result.args->at(12));
            float kd = stof(result.args->at(13));
            float kr = stof(result.args->at(14));
            float ks = stof(result.args->at(15));
            int sp = stoi(result.args->at(16));
            Material *mat = new Material(BRDF(Color(kd, kd, kd),
                                              Color(ks, ks, ks),
                                              Color(ka, ka, ka),
                                              Color(kr, kr, kr),
                                              sp));
            GeometricPrimitive *prim = new GeometricPrimitive(new Transformation(translate),
                                                              tri,
                                                              mat);
            scene.world->primList.push_back(prim);
            
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
            GeometricPrimitive *prim = new GeometricPrimitive(new Transformation(transform), sphere, mat);
            scene.world->primList.push_back(prim);
        }
        if (result.optName.compare("-pl") == 0) {
            Color liteColor(stof(result.args->at(0)),
                            stof(result.args->at(1)),
                            stof(result.args->at(2)));
            Point location(stof(result.args->at(3)),
                           stof(result.args->at(4)),
                           stof(result.args->at(5)));
            scene.lights.push_back(Light(liteColor, Vector(), location, LightSourcePoint));
        }
        if (result.optName.compare("-dl") == 0) {
            Color liteColor(stof(result.args->at(0)),
                            stof(result.args->at(1)),
                            stof(result.args->at(2)));
            Vector direction(stof(result.args->at(3)),
                           stof(result.args->at(4)),
                           stof(result.args->at(5)));
            scene.lights.push_back(Light(liteColor, direction, Point(), LightSourceDirectional));
        }
    }
    
    if (doesRender) {
        if (automatic) {
            println("Automatic Mode");
            parseObjectFiles(*(scene.world), basePath);
        }
        scene.render();
    }
    return 0;
}