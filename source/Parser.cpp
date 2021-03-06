#ifndef __Assignment2__Parser__
#define __Assignment2__Parser__

#include "preHeader.h"
#include "AggregatePrimitive.cpp"
#include "Triangle.cpp"
#include "GeometricPrimitive.cpp"
#include "BRDF.cpp"
extern bool verbose;
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
            if (name.find(".param") != string::npos) {
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

void parseTransformFile(AggregatePrimitive& aggregate,
                        vector<Light>& lights,
                        const string& fileName, 
                        Transformation** transPtr2Ptr, 
                        Point *eye,
                        vector<Point>& plane)
{
    const int MAX_CHARS_PER_LINE = 100;
    const int MAX_TOKENS_PER_LINE = 100;
    const char* const DELIMITER = " ";
    // create a file-reading object
    ifstream fin;
    fin.open(fileName); // open a file
    if (!fileName.find(".param") || !fin.good()) {
        *transPtr2Ptr = new Transformation(); // set good-for-nothing transformation
        return;
    }
    Transform3fAffine composedTransform = IdentityTransform(); //to combine all parsed transformations
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
        if (verbose) {
            for (int i = 0; i < n; i++) // n = #of tokens
                cout << "Token[" << i << "] = " << token[i] << endl;
            cout << endl;
        }
        /**
         *  Construct Transformation and other parameters
         */
        if (token[0]) {
            if (strcmp(token[0], "scale")==0) {
                if (!(token[1] && token[2] && token[3])) {
                    cerr << "syntax error for scale" << endl;
                    continue;
                }
                composedTransform = Scaling3f(atof(token[1]),
                                               atof(token[2]),
                                               atof(token[3])) * composedTransform;
            }else if (strcmp(token[0], "rotate")==0) {
                if (!(token[1] && token[2] && token[3] && token[4])) {
                    cerr << "syntax error for rotate" << endl;
                    continue;
                }
                Vector axis = {(float)atof(token[1]), (float)atof(token[2]), (float)atof(token[3])};
                float angle = atof(token[4]);
                composedTransform = AngleAxisf(angle, axis.normalized()) * composedTransform;
            }else if (strcmp(token[0], "translate")==0) {
                if (!(token[1] && token[2] && token[3])) {
                    cerr << "syntax error for translate" << endl;
                    continue;
                }
                composedTransform = Translation3f(atof(token[1]),
                                                   atof(token[2]),
                                                   atof(token[3])) * composedTransform;
            }else if (strcmp(token[0], "plane")==0) {
                if (!(token[1] && token[2] && token[3] &&
                      token[4] && token[5] && token[6] &&
                      token[7] && token[8] && token[9] &&
                      token[10] && token[11] && token[12])) {
                    cerr << "syntax error for plane" << endl;
                    continue;
                }
                plane.push_back(Point(atof(token[1]), atof(token[2]), atof(token[3])));
                plane.push_back(Point(atof(token[4]), atof(token[5]), atof(token[6])));
                plane.push_back(Point(atof(token[7]), atof(token[8]), atof(token[9])));
                plane.push_back(Point(atof(token[10]), atof(token[11]), atof(token[12])));
            }else if (strcmp(token[0], "eye")==0) {
                if (!(token[1] && token[2] && token[3])) {
                    cerr << "syntax error for eye" << endl;
                    continue;
                }
                *eye = Point(atof(token[1]),
                             atof(token[2]),
                             atof(token[3]));
            }else if (strcmp(token[0], "pl")==0) {
                if (!(token[1] && token[2] && token[3] &&
                      token[4] && token[5] && token[6])) {
                    cerr << "syntax error for point light" << endl;
                    continue;
                }
                Color liteColor(atof(token[1]),
                                atof(token[2]),
                                atof(token[3]));
                Point location(atof(token[4]),
                               atof(token[5]),
                               atof(token[6]));
                lights.push_back(Light(liteColor, Vector(), location, LightSourcePoint));
            }else if (strcmp(token[0], "dl")==0) {
                if (!(token[1] && token[2] && token[3] &&
                      token[4] && token[5] && token[6])) {
                    cerr << "syntax error for directional light" << endl;
                    continue;
                }
                Color liteColor(atof(token[1]),
                                atof(token[2]),
                                atof(token[3]));
                Point direction(atof(token[4]),
                                atof(token[5]),
                                atof(token[6]));
                lights.push_back(Light(liteColor, direction, Point(), LightSourceDirectional));
            }else if (strcmp(token[0], "sphere")==0) {
                if (!(token[1] && //radius
                      token[2] && token[3] && token[4] && //center x y z
                      token[5] && token[6] && token[7] && // ka
                      token[8] && token[9] && token[10] && // kd
                      token[11] && token[12] && token[13] && // kr
                      token[14] && token[15] && token[16] && // ks
                      token[17] && //sp
                      token[18] && token[19] && token[20] //scale x y z
                      )) {
                    cerr << "syntax error for sphere" << endl;
                    continue;
                }
                Sphere *sphere = new Sphere(atof(token[1]),0,0,0);
                Translation3f translate(atof(token[2]), atof(token[3]), atof(token[4]));
                Transform3fAffine transform(Scaling3f(atof(token[18]),atof(token[19]),atof(token[20])));
                Color ka(atof(token[5]), atof(token[6]), atof(token[7]));
                Color kd(atof(token[8]), atof(token[9]), atof(token[10]));
                Color kr(atof(token[11]), atof(token[12]), atof(token[13]));
                Color ks(atof(token[14]), atof(token[15]), atof(token[16]));
                int sp = atoi(token[17]);
                Material *mat = new Material(BRDF(kd, ks, ka, kr, sp));
                GeometricPrimitive *prim = new GeometricPrimitive(new Transformation(translate*transform),
                                                                  sphere,
                                                                  mat);
                aggregate.primList.push_back(prim);
            }else if (strcmp(token[0], "triangle")==0) {
                if (!(token[1] && token[2] && token[3] && // Point 1 x y z
                      token[4] && token[5] && token[6] && // Point 2 x y z
                      token[7] && token[8] && token[9] && // Point 3 x y z
                      token[10] && token[11] && token[12] && //center x y z
                      token[13] && token[14] && token[15] && // ka
                      token[16] && token[17] && token[18] && // kd
                      token[19] && token[20] && token[21] && // kr
                      token[22] && token[23] && token[24] && // ks
                      token[25] //sp
                      )) {
                    cerr << "syntax error for triangle" << endl;
                    continue;
                }
                Triangle *tri = new Triangle(Point(atof(token[1]),
                                                   atof(token[2]),
                                                   atof(token[3])),
                                             Point(atof(token[4]),
                                                   atof(token[5]),
                                                   atof(token[6])),
                                             Point(atof(token[7]),
                                                   atof(token[8]),
                                                   atof(token[9])));
                Translation3f translate(atof(token[10]),atof(token[11]),atof(token[12]));
                Color ka(atof(token[13]), atof(token[14]), atof(token[15]));
                Color kd(atof(token[16]), atof(token[17]), atof(token[18]));
                Color kr(atof(token[19]), atof(token[20]), atof(token[21]));
                Color ks(atof(token[22]), atof(token[23]), atof(token[24]));
                int sp = atoi(token[25]);
                Material *mat = new Material(BRDF(kd, ks, ka, kr, sp));
                GeometricPrimitive *prim = new GeometricPrimitive(new Transformation(translate), tri, mat);
                aggregate.primList.push_back(prim);
            }else if (strcmp(token[0], "rect")==0) {
                if (!(token[1] && token[2] && token[3] && // Point 1 x y z
                      token[4] && token[5] && token[6] && // Point 2 x y z
                      token[7] && token[8] && token[9] && // Point 3 x y z
                      token[10] && token[11] && token[12] && //Point 4 x y z
                      token[13] && token[14] && token[15] && // ka
                      token[16] && token[17] && token[18] && // kd
                      token[19] && token[20] && token[21] && // kr
                      token[22] && token[23] && token[24] && // ks
                      token[25] && //sp
                      token[26] && token[27] && token[28] //translate x y z
                      )) {
                    cerr << "syntax error for rectangle" << endl;
                    continue;
                }
                Triangle *tri1 = new Triangle(Point(atof(token[1]),
                                                    atof(token[2]),
                                                    atof(token[3])),
                                              Point(atof(token[4]),
                                                    atof(token[5]),
                                                    atof(token[6])),
                                              Point(atof(token[7]),
                                                    atof(token[8]),
                                                    atof(token[9])));
                Triangle *tri2 = new Triangle(Point(atof(token[1]),
                                                    atof(token[2]),
                                                    atof(token[3])),
                                              Point(atof(token[7]),
                                                    atof(token[8]),
                                                    atof(token[9])),
                                              Point(atof(token[10]),
                                                    atof(token[11]),
                                                    atof(token[12])));
                Translation3f translate(atof(token[26]),atof(token[27]),atof(token[28]));
                Color ka(atof(token[13]), atof(token[14]), atof(token[15]));
                Color kd(atof(token[16]), atof(token[17]), atof(token[18]));
                Color kr(atof(token[19]), atof(token[20]), atof(token[21]));
                Color ks(atof(token[22]), atof(token[23]), atof(token[24]));
                int sp = atoi(token[25]);
                Material *mat = new Material(BRDF(kd, ks, ka, kr, sp));
                Transformation *trans = new Transformation(translate);
                GeometricPrimitive *prim1 = new GeometricPrimitive(trans, tri1, mat);
                GeometricPrimitive *prim2 = new GeometricPrimitive(trans, tri2, mat);
                aggregate.primList.push_back(prim1);
                aggregate.primList.push_back(prim2);
            } //end if else
        } // end if (token[0])
    } //end while
    *transPtr2Ptr = new Transformation(composedTransform);
}

void parseObjectFiles(AggregatePrimitive& aggregate,
                      vector<Light>& lights,
                      const string& basePath,
                      Point *eye,
                      vector<Point>& plane) {
    vector<shape_t> shapes;
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
        parseTransformFile(aggregate, lights, tMap[fileNameWithoutExt(file)], &transformation, eye, plane);
        
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
                aggregate.primList.push_back(geoPrim);
                if (verbose) {
                    println("Obtained Triangle");
                    printf("A: %f, %f, %f\n", tri->A(0), tri->A(1), tri->A(2));
                    printf("B: %f, %f, %f\n", tri->B(0), tri->B(1), tri->B(2));
                    printf("C: %f, %f, %f\n", tri->C(0), tri->C(1), tri->C(2));
                }
            }
        }
    }
}


#endif /* defined(__Assignment2__Parser__) */