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
    Transform3fAffine composedTransform = IdentityTransform();
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
//        for (int i = 0; i < n; i++) // n = #of tokens
//            cout << "Token[" << i << "] = " << token[i] << endl;
//        cout << endl;
        
        /**
         *  Construct Transformation
         */
        if (token[0]) {
            if (strcmp(token[0], "scale")==0) {
                if (!token[1]) {
                    cerr << "syntax error for scale" << endl;
                    continue;
                }
                float factor = atof(token[1]);
                composedTransform *= Scalingf(factor);
            }else if (strcmp(token[0], "rotate")==0) {
                if (!(token[1] && token[2] && token[3] && token[4])) {
                    cerr << "syntax error for rotate" << endl;
                    continue;
                }
                Vector axis = {atof(token[1]), atof(token[2]), atof(token[3])};
                float angle = atof(token[4]);
                composedTransform *= AngleAxisf(angle, axis.normalized());
            }else if (strcmp(token[0], "translate")==0) {
                if (!(token[1] && token[2] && token[3])) {
                    cerr << "syntax error for translate" << endl;
                    continue;
                }
                composedTransform *= Translation3f(atof(token[1]),
                                                   atof(token[2]),
                                                   atof(token[3]));
            }
        }
    }
    *transPtr2Ptr = new Transformation(composedTransform);
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
            brdf->sp = shape.material.shininess;
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

            vector<float> normals = shape.mesh.normals;
            vector<unsigned int> indices = shape.mesh.indices;
            vector<float> positions = shape.mesh.positions;

            for (size_t v = 0; v < indices.size(); v+=3) {
                Point p1(positions[indices[v+0]*3+0],
                         positions[indices[v+0]*3+1],
                         positions[indices[v+0]*3+2]);
                Point p2(positions[indices[v+1]*3+0],
                         positions[indices[v+1]*3+1],
                         positions[indices[v+1]*3+2]);
                Point p3(positions[indices[v+2]*3+0],
                         positions[indices[v+2]*3+1],
                         positions[indices[v+2]*3+2]);
                Vector n1(normals[indices[v+0]*3+0],
                          normals[indices[v+0]*3+1],
                          normals[indices[v+0]*3+2]);
                Vector n2(normals[indices[v+1]*3+0],
                          normals[indices[v+1]*3+1],
                          normals[indices[v+1]*3+2]);
                Vector n3(normals[indices[v+2]*3+0],
                          normals[indices[v+2]*3+1],
                          normals[indices[v+2]*3+2]);
                Triangle *tri = new Triangle();
                tri->predefinedNormal = true;
                tri->A = p1;
                tri->B = p2;
                tri->C = p3;
                tri->Anorm = n1;
                tri->Bnorm = n2;
                tri->Cnorm = n3;
                GeometricPrimitive *geoPrim = new GeometricPrimitive();
                geoPrim->shape = tri;
                geoPrim->mat = material;
                geoPrim->objToWorld = transformation;
                geoPrim->completeTransformationData();
                //do transformation here
                convertedShape->primList.push_back(geoPrim);
                println("Obtained Triangle");
                printf("A: %f, %f, %f\n", tri->A(0), tri->A(1), tri->A(2));
                printf("B: %f, %f, %f\n", tri->B(0), tri->B(1), tri->B(2));
                printf("C: %f, %f, %f\n", tri->C(0), tri->C(1), tri->C(2));
            }
        }
    }
}


#endif /* defined(__Assignment2__Parser__) */