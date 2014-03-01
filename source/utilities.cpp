#ifndef DVZUtil_H
#define DVZUtil_H

#include "preHeader.h"

#define RCh 0
#define GCh 1
#define BCh 2


#pragma mark - Math
GLfloat sqr(GLfloat x) { return x*x;}
#pragma mark - Data Structures
typedef enum {
    LightSourceDirectional,
    LightSourcePoint
} LightSourceType;
typedef struct {
    int w, h;
} Viewport;

typedef struct {
    string optName;
    int numOfArgs;
    vector<string> *args;
} CmdLineOptResult;

class Color {
public:
    GLfloat r, g, b;
    Color(GLfloat red, GLfloat green, GLfloat blue){r=red;g=green;b=blue;}
    Color(){r=0.0; g=0.0; b=0.0;}
    //Point wise multiplication
    Color ptWiseMul(Color &other) {
        return Color(r*other.r, g*other.g, b*other.b);
    }
    //Scalar Multiplication
    Color& operator*=(const GLfloat &rhs) {
        r *= rhs; g *= rhs; b *= rhs;
        return *this;
    }
    const Color operator*(const GLfloat &other) const {
        return Color(*this) *= other;
    }
    //Summation
    Color& operator+=(const Color &rhs) {
        r += rhs.r; g += rhs.g; b += rhs.b;
        return *this;
    }
    const Color operator+(const Color &other) const {
        return Color(*this) += other;
    }
    //Map from 1.0 scale to 255 scale
    
    //Make sure values are between 0 and 1 inclusive
    void standardize() {
        r = fmin(fmax(r, 0.0), 1.0);
        g = fmin(fmax(g, 0.0), 1.0);
        b = fmin(fmax(b, 0.0), 1.0);
    }
};

class Vector {
public:
    GLfloat x, y, z;
    Vector(GLfloat xa, GLfloat ya, GLfloat za){x=xa;y=ya;z=za;}
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
    GLfloat operator*(const Vector &other) const {
        return other.x*x + other.y*y + other.z*z;
    }
    //Scalar Multiplication
    Vector& operator*=(const GLfloat &rhs) {
        x *= rhs; y *= rhs; z *= rhs;
        return *this;
    }
    const Vector operator*(const GLfloat &other) const {
        return Vector(*this) *= other;
    }
    //Magnitude/length
    GLfloat magnitude() {
        return (GLfloat)sqrt(sqr(x)+sqr(y)+sqr(z));
    }
    //Normalize
    void normalize() {
        GLfloat mag = magnitude();
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
    GLfloat angleBetween(Vector &other) {
        return acos(other.normalized() * normalized());
    }
    
};

class Coordiate {
public:
    GLfloat x, y, z;
    Coordiate(GLfloat xa, GLfloat ya, GLfloat za){x=xa;y=ya;z=za;}
    Coordiate() {x=0.0;y=0.0;z=0.0;}
    //Point minus Point to form vector
    const Vector operator-(const Coordiate &other) const {
        return Vector(x-other.x, y-other.y, z-other.z);
    }
    Coordiate scaledBy(GLfloat factor) {
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
GLfloat GLfloatFromString(string str)
{
    return (GLfloat)atof(str.c_str());
}
GLint GLintFromString(string str)
{
    return (GLint)atoi(str.c_str());
}

#endif
