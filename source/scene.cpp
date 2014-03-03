#include "Scene.h"
#include "test.cpp"

int main(int argc, char *argv[]) {
    
    vector<CmdLineOptResult> *results;
    string options = "-t(0)-tobj(0)-tsampler(0)";
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
    }
    return 0;
}