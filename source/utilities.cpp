#ifndef DVZUtil_H
#define DVZUtil_H
#include "preHeader.h"
#define RCh 0
#define GCh 1
#define BCh 2


#pragma mark - Math
float sqr(float x) { return x*x;}

#pragma mark - Data Structures
typedef enum {
    LightSourceDirectional,
    LightSourcePoint
} LightSourceType;
typedef struct {
    int w, h;
} Viewport;
typedef struct {
    string oBRDFptName;
    int numOfArgs;
    vector<string> *args;
} CmdLineOptResult;

typedef Array3f Color;


class Vector {
public:
    float x, y, z;
    Vector(float xa, float ya, float za){x=xa;y=ya;z=za;}
    Vector(){x=0.0;y=0.0;z=0.0;}
    //Summation
    Vector& operator+=(const Vector &rhs) {
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }
    const Vector operator+(const Vector &other) const {
        return Vector(*this) += other;
    }
    //Negation
    Vector operator-() const {
        return Vector(-x, -y, -z);
    }
    //Subtraction
    Vector& operator-=(const Vector &rhs) {
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }
    const Vector operator-(const Vector &other) const {
        return Vector(*this) -= other;
    }
    //Dot Product
    float operator*(const Vector &other) const {
        return other.x*x + other.y*y + other.z*z;
    }
    //Scalar Multiplication
    Vector& operator*=(const float &rhs) {
        x *= rhs; y *= rhs; z *= rhs;
        return *this;
    }
    const Vector operator*(const float &other) const {
        return Vector(*this) *= other;
    }
    //Magnitude/length
    float magnitude() {
        return (float)sqrt(sqr(x)+sqr(y)+sqr(z));
    }
    //Normalize
    void normalize() {
        float mag = magnitude();
        if (mag != 0.0) {
            x /= mag;
            y /= mag;
            z /= mag;
        }
    }
    //Normalized
    Vector normalized() {
        Vector normalizedVec = Vector(*this);
        normalizedVec.normalize();
        return normalizedVec;
    }
    //Angel between two vectors
    float angleBetween(Vector &other) {
        return acos(other.normalized() * normalized());
    }
    
};

class Coordiate {
public:
    float x, y, z;
    Coordiate(float xa, float ya, float za){x=xa;y=ya;z=za;}
    Coordiate() {x=0.0;y=0.0;z=0.0;}
    //Point minus Point to form vector
    const Vector operator-(const Coordiate &other) const {
        return Vector(x-other.x, y-other.y, z-other.z);
    }
    Coordiate scaledBy(float factor) {
        return Coordiate(x*factor, y*factor, z*factor);
    }
};
typedef Coordiate Location;
typedef Coordiate Point;

typedef struct {
    Color color;
    Vector vector;
    Location location;
    LightSourceType type;
} LightSource;

#pragma mark - Utilities

/**
 *  Parses command line options specified in "options"
 *
 *  @param argc    argc from main
 *  @param argv    argc from main
 *  @param options a specially formatted string specifying what you want to parse
 *  @param results a pointer to a pointer to the vector of parsed results
 *  @discussion
 *      The format of options: "argX(n)argY(m)", where argX argY are the name of the options,
 *      n and m are the number of arguments for each option
 *      for example, if my program takes "./program -file one two --copy bunny -k rgb"
 *      then my options string would be "-file(2)--copy(1)-k(1)"
 *
 *      The results pointer will be set to a vector, containing CmdLineOptResult's, which is typedef'd above.
 *      To-Do: catch incorrectly formatted arguments and inconsistent number of arguments error instead of crashing
 *  @return none
 *  @example
 *      vector<CmdLineOptResults> *results;
 *      getCmdLineOptions(argc, argv, "-ka(2)-kz(1)", &results);
 *      for (auto & result : results) {
 *          cout << result.optName << "\n";
 *          cout << result.args->at(0) <<" "<< result.args-at(1);
 *      }
 */
void getCmdLineOptions(int argc, char *argv[], string options, vector<CmdLineOptResult> **results)
{
    vector<CmdLineOptResult> *resultsArray = new vector<CmdLineOptResult>();
    for (int i=1; i < argc; ++i)
    {
        string arg = string(argv[i]);
        size_t index = options.find(arg);
        if (index!=string::npos)
        {
            char c = ' ';
            while (c!='\0' && (c=options[index++])!='('){};
            string num = string();
            while (c!='\0' && (c=options[index])!=')') {
                num.append(1, options[index]);
                ++index;
            }
            if (c=='\0') {
                throw "Options string is incorrectly formatted";
            }
            int numOfArgs = atoi(num.c_str());
            //build result for this option
            vector<string> *arguments = new vector<string>(numOfArgs);
            CmdLineOptResult result = {arg, numOfArgs, arguments};
            for (int arg_index=0; arg_index<numOfArgs; ++arg_index) {
                (*arguments)[arg_index] = argv[(++i)];
            }
            resultsArray->push_back(result);
        }
    }
    *results = resultsArray;
}

#pragma mark - Conversion
float floatFromString(string str)
{
    return (float)atof(str.c_str());
}
GLint GLintFromString(string str)
{
    return (GLint)atoi(str.c_str());
}

#endif
