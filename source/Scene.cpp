#ifndef MAIN_CPP
#define MAIN_CPP
#include "preHeader.h"
#include "test.cpp"
#include "AggregatePrimitive.cpp"
#include "Parser.cpp"

extern bool verbose;

class Scene {
public:
    float resolution;
    int maxDepth;
    int sqrt_rpp = 1;
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
    string outputFileName;
    
    Scene() {
        float defaultResolution = 320;
        UL = Point(-1,  1, -1);
        UR = Point(1,  1, -1);
        LR = Point(1, -1, -1);
        LL = Point(-1, -1, -1);
        eye = Point(0,0,0);
        maxDepth = 2;
        resolution = defaultResolution;
        world = new AggregatePrimitive();
        sampler = new Sampler(defaultResolution, defaultResolution);
        camera = new Camera(UL, UR, LR, LL, eye, defaultResolution, defaultResolution);
        film = new Film(defaultResolution, defaultResolution);
        outputFileName = "output.png";
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
    
    Scene(Point UL,
          Point UR,
          Point LR,
          Point LL,
          Point eye,
          float resolution,
          int maxDepth,
          int sqrt_rpp){
        this->UL = UL;
        this->UR = UR;
        this->LR = LR;
        this->LL = LL;
        this->eye = eye;
        this->resolution = resolution;
        this->maxDepth = maxDepth;
        world = new AggregatePrimitive();
        sampler = new Sampler(resolution, resolution, sqrt_rpp);
        camera = new Camera(UL, UR, LR, LL, eye, resolution, resolution);
        film = new Film(resolution, resolution);
    }
    
    void changeResolution(int resolution) {
        this->resolution = resolution;
        delete sampler;
        sampler = new Sampler(resolution, resolution);
        delete camera;
        camera = new Camera(UL, UR, LR, LL, eye, resolution, resolution);
        delete film;
        film = new Film(resolution, resolution);
    }
    
    void render() {
        raytracer = new Raytracer(*world, lights, maxDepth);
        raytracer->eye = eye;
        raytracer->generateTree();
        Sample sample;
        Ray ray;
        int currentPercentage = 0;
        while (sampler->getSample(&sample)) {
            Color color = {0,0,0};
            camera->generateRay(sample, &ray);
            if (sample(0) == 122.5 && sample(1) == 8.5) {
                //break
            }
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
        film->writeImage(outputFileName.c_str());
    }
};

int main(int argc, char *argv[]) {
    
    Scene scene;
    vector<CmdLineOptResult> *results;
    bool doesRender;
    string basePath;
    /*
     //testing
     -t         (comprehensive testing)
     -tobj      (test tinyObjLoader)
     -tsampler  (test Sampler)
     -tcam      (test Camera)
     -tintersect   (test for Intersection)
     -ttrans    (test Transformation)
     -tparser basePath   (test parser)
     
     //rendering
     -render path/to/object/
     -resolution pixels
     */
    string options = "-t(0)-tobj(0)-tsampler(0)-tcam(0)-tintersect(0)-ttrans(0)-tparser(1)-render(1)-resolution(1)-out(1)-verbose(0)-rpp(1)";
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
        if (result.optName.compare("-tintersect") == 0) {
            testSphereIntersection();
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
            basePath = result.args->at(0);
        }
        if (result.optName.compare("-resolution")==0) {
            int pix = stoi(result.args->at(0));
            scene.changeResolution(pix);
        }
        if (result.optName.compare("-out")==0) {
            scene.outputFileName = result.args->at(0);
        }
        if (result.optName.compare("-rpp")==0) {
            float sqrt_rpp = stoi(result.args->at(0));
            (scene.sampler)->sqrt_rpp = sqrt_rpp;
            (scene.sampler)->unit = 1/sqrt_rpp;
            (scene.sampler)->offset = 1/sqrt_rpp/2;
            (scene.sampler)->antialias = true;
            (scene.film)->rpp = pow(stoi(result.args->at(0)),2);
        }
    }
    
    if (doesRender) {
        vector<Point> plane;
        parseObjectFiles(*(scene.world), scene.lights, basePath, &(scene.eye), plane);
        if (plane.size() == 4) {
            scene.UL = plane[0];
            scene.UR = plane[1];
            scene.LR = plane[2];
            scene.LL = plane[3];
        }
        scene.render();
    }
    return 0;
}
#endif
