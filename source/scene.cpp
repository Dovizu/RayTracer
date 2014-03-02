#include "Scene.h"
#include "test.cpp"

int main(int argc, char *argv[]) {
    
    vector<CmdLineOptResult> *results;
    string options = "-t(0)";
    getCmdLineOptions(argc, argv, options, &results);
    for (auto & result : *results) {
        if (result.optName.compare("-t") == 0) {
            flag_testing = true;
        }
    }

    if (flag_testing) {
        testAll();
        flag_testing = false;
    }

    return 0;
}