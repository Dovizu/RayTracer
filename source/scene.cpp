#include "Scene.h"
#include "test.cpp"

int main(int argc, char *argv[]) {
    
    vector<CmdLineOptResult> *results;
    /*
     -t         comprehensive testing
     -tobj      test tinyObjLoader
     -tsampler  test Sampler
     -tcam      test Camera
     -tsintersect   test Sphere Intersection
     -ttintersect   test Triangle Intersection
     -ttrans    test Transformation
     -tparser basePath   test parser
     */
    string options = "-t(0)-tobj(0)-tsampler(0)-tcam(0)-tsintersect(0)-ttintersect(0)-ttrans(0)-tparser(1)";
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
    }
    return 0;
}