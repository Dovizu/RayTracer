#ifndef __Assignment2__Parser__
#define __Assignment2__Parser__

#include "preHeader.h"
#include "AggregatePrimitive.cpp"
#include "Triangle.cpp"
#include "GeometricPrimitive.cpp"
#include "BRDF.cpp"

typedef map<string, string> TransformMap;
void getFileNamesOfDirectory(const string& basePath, vector<string>& filenames, TransformMap& tMap) {
    dirent *de;
    DIR *dirp = opendir(basePath.c_str());
    if (dirp) {
        while (true) {
            errno = 0;
            de = readdir(dirp);
            if (de == NULL) break;
            string name = string(de->d_name);
            if (name.find(".obj") != string::npos) {
                tMap[fileNameWithoutExt(basePath+name)] = "";
                filenames.push_back(basePath+name);
            }
            if (name.find(".transform") != string::npos) {
                tMap[fileNameWithoutExt(basePath+name)] = basePath+name;
            }
        }
        closedir(dirp);
    }
}

Color getKr(string& args) {
    vector<float> kr;
    string number;
    for (size_t i = 0; i < args.length(); ++i) {
        if (args.at(i) == ' '){
            kr.push_back(stof(number));
            number.clear();
        }else{
            number += args.at(i);
        }
    }
    kr.push_back(stof(number));
    return Color(kr.at(0), kr.at(1), kr.at(2));
}

void parseTransformFile(const string& fileName, Transformation** transPtr2Ptr) {
    const int MAX_CHARS_PER_LINE = 512;
    const int MAX_TOKENS_PER_LINE = 100;
    const char* const DELIMITER = " ";
    // create a file-reading object
    ifstream fin;
    fin.open(fileName); // open a file
    if (!fileName.find(".transform") || !fin.good()) {
        *transPtr2Ptr = new Transformation(); // set good-for-nothing transformation
        return;
    }
    // read each line of the file
    while (!fin.eof()) {
        // read an entire line into memory
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        // parse the line into blank-delimited tokens
        int n = 0; // a for-loop index
        // array to store memory addresses of the tokens in buf
        const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
        // parse the line
        token[0] = strtok(buf, DELIMITER); // first token
        if (token[0]) { // zero if line is blank
            for (n = 1; n < MAX_TOKENS_PER_LINE; n++) {
                token[n] = strtok(NULL, DELIMITER); // subsequent tokens
                if (!token[n]) break; // no more tokens
            }
        }
        // process the tokens
        for (int i = 0; i < n; i++) // n = #of tokens
            cout << "Token[" << i << "] = " << token[i] << endl;
        cout << endl;
        
        /**
         *  Do my transformation creation here
         */
    }
}

void parseObjectFiles(AggregatePrimitive& aggregate, const string& basePath) {
    vector<shape_t> shapes;
    //get all the file names
    vector<string> filenames;
    TransformMap tMap;
    getFileNamesOfDirectory(basePath, filenames, tMap);
    for (auto & file : filenames) {
        shapes.clear();
        string err = LoadObj(shapes, file.c_str(), basePath.c_str());
        if (!err.empty()){
            cerr << err << endl;
            return;
        }
        Transformation *transformation;
        parseTransformFile(tMap[fileNameWithoutExt(file)], &transformation);
        
        for (auto & shape : shapes) {
            assert(shape.mesh.indices.size() % 3 == 0);
            //mat is same for all primitives of this shape
            Material *material = new Material();
            BRDF *brdf = &(material->brdf);
            
            brdf->ka = Color(shape.material.ambient[0],
                             shape.material.ambient[1],
                             shape.material.ambient[2]);
            brdf->kd = Color(shape.material.diffuse[0],
                             shape.material.diffuse[1],
                             shape.material.diffuse[2]);
            brdf->ks = Color(shape.material.specular[0],
                             shape.material.specular[1],
                             shape.material.specular[2]);
            //for extra parameters that standard MTL format doesn't support
            map<string, string>::iterator it(shape.material.unknown_parameter.begin());
            map<string, string>::iterator itEnd(shape.material.unknown_parameter.end());
            for (; it != itEnd; it++) {
                printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
                if (it->first.compare("kr")==0) {
                    //find all three floats, R G B
                    brdf->kr = getKr(it->second);
                }
                else if (it->first.compare("sp")==0) {
                    brdf->sp = atoi(it->second.c_str());
                }
            }
            //convertedShape is an aggregate of triangles
            AggregatePrimitive *convertedShape = new AggregatePrimitive();
            aggregate.primList.push_back(convertedShape);
            vector<Point> threePoints; //placeholder
            for (size_t v = 0; v < shape.mesh.positions.size() / 3; v++) {
                float p1 = shape.mesh.positions[3*v+0]; //x
                float p2 = shape.mesh.positions[3*v+1]; //y
                float p3 = shape.mesh.positions[3*v+2]; //z
                if (threePoints.size() != 3) {
                    threePoints.push_back(Point(p1, p2, p3));
                }else{
                    Triangle *tri = new Triangle();
                    tri->A = threePoints.at(0);
                    tri->B = threePoints.at(1);
                    tri->C = threePoints.at(2);
                    threePoints.clear();
                    GeometricPrimitive *geoPrim = new GeometricPrimitive();
                    geoPrim->shape = tri;
                    geoPrim->mat = material;
                    geoPrim->objToWorld = transformation;
                    geoPrim->completeTransformationData();
                    //do transformation here
                    convertedShape->primList.push_back(geoPrim);
                }
            }
        }
    }
}


#endif /* defined(__Assignment2__Parser__) */