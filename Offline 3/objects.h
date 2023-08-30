#define _USE_MATH_DEFINES
#include <vector>
#include <iostream>
#include <cmath>
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include "Matrix.h"
#include <cstdint>

#ifndef OBJECTS_H
#define OBJECTS_H

class Vector {
public:
    double x, y, z, w;
    Vector() {x = 0; y = 0; z = 0; w = 0;}
    Vector(double a, double b, double c) {x = a; y = b; z = c; w = 0;}
    Vector(double a, double b, double c, double d) {x = a; y = b; z = c; w = d;}
    Vector operator=(Vector v) {x = v.x; y = v.y; z = v.z; w = v.w; return *this;}
    Vector operator*(double d) {return Vector(x*d, y*d, z*d);}
    Vector operator/(double d) {return Vector(x/d, y/d, z/d);}
    Vector operator+(Vector v) {return Vector(x+v.x, y+v.y, z+v.z);}
    Vector operator-(Vector v) {return Vector(x-v.x, y-v.y, z-v.z);}
    Vector operator^(Vector v) {return Vector(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);}
    double operator*(Vector v) {return x*v.x + y*v.y + z*v.z;}
    Vector operator-() {return Vector(-x, -y, -z);}
    double length() {return sqrt(x*x + y*y + z*z);}
    Vector normalize() {double l = length(); x /= l; y /= l; z /= l;return *this;}
    friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Vector& v) {
        is >> v.x >> v.y >> v.z;
        return is;
    }
};

class Point {
public:
    double x, y, z, w;
    Point() {x = 0; y = 0; z = 0; w = 1.0;}
    Point(double a, double b, double c) {x = a; y = b; z = c; w = 1.0;}
    Point(double a, double b, double c, double d) {x = a; y = b; z = c; w = d;}
    Point operator+(Vector v) {return Point(x+v.x, y+v.y, z+v.z);}
    Point operator-(Vector v) {return Point(x-v.x, y-v.y, z-v.z);}
    Vector operator-(Point p) {return Vector(x-p.x, y-p.y, z-p.z);}
    Point operator=(Point p) {x = p.x; y = p.y; z = p.z; w = p.w; return *this;}
    Point operator*(double d) {return Point(x*d, y*d, z*d);}
    Point operator/(double d) {return Point(x/d, y/d, z/d);}
    Point operator-() {return Point(-x, -y, -z);}
    void normalize() {x /= w; y /= w; z /= w; w = 1.0;}
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Point& p) {
        is >> p.x >> p.z >> p.y;
        p.x = -p.x;
        return is;
    }
};

class Ray {
public:
    Point start;
    Vector dir;
    Ray() {}
    Ray(Point p, Vector v) {start = p; dir = v.normalize();}
    Ray(Point p1, Point p2) {start = p2; dir = (p2 - p1).normalize();}
    friend std::ostream& operator<<(std::ostream& os, const Ray& r) {
        os << r.start << " " << r.dir;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Ray& r) {
        is >> r.start >> r.dir;
        return is;
    }
};

class Color {
public:
    double r, g, b;
    Color() {r = 0; g = 0; b = 0;}
    Color(double a, double c, double d) {r = a; g = c; b = d;}
    Color operator=(Color c) {r = c.r; g = c.g; b = c.b; return *this;}
    Color operator*(double d) {return Color(r*d, g*d, b*d);}
    Color operator*(Color c) {return Color(r*c.r, g*c.g, b*c.b);}
    Color operator+(Color c) {return Color(r+c.r, g+c.g, b+c.b);}
    Color operator+(double d) {return Color(r+d, g+d, b+d);}
    friend std::ostream& operator<<(std::ostream& os, const Color& c) {
        os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Color& c) {
        is >> c.r >> c.g >> c.b;
        return is;
    }

};

class Triangle {
public:
    Point a, b, c;
    Triangle() {}
    Triangle(Point p, Point q, Point r) {a = p; b = q; c = r;}
    friend std::ostream& operator<<(std::ostream& os, const Triangle& t) {
        os << t.a << " " << t.b << " " << t.c;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Triangle& t) {
        is >> t.a >> t.b >> t.c;
        return is;
    }
    void draw(){
        glBegin(GL_TRIANGLES);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glEnd();
    }

    double intersect(Ray ray, double &u, double &v){
        Vector edge1 = b - a;
        Vector edge2 = c - a;
        Vector h = ray.dir ^ edge2;
        double det = edge1 * h;
        if (fabs(det) < 1e-5) return -1;
        double f = 1 / det;
        Vector s = ray.start - a;
        u = f * (s * h);
        if (u < 0.0 || u > 1.0) return -1;
        Vector q = s ^ edge1;
        v = f * (ray.dir * q);
        if (v < 0.0 || u + v > 1.0) return -1;
        double t = f * (edge2 * q);
        if (t > 0) return t;
        return -1;
    }

    Vector getNormal(Point p){
        Vector edge1 = b - a;
        Vector edge2 = c - a;
        Vector normal = edge1 ^ edge2;
        normal.normalize();
        return normal;
    }
};

class Light {
public:
    Point pos;
    Color color;
    double falloff;
    Light() {}
    Light(Point p, Color c) {pos = p; color = c;}
    Light(Point p, Color c, double f) {pos = p; color = c; falloff = f;}
    friend std::ostream& operator<<(std::ostream& os, const Light& l) {
        os << l.pos << " " << l.color << " " << l.falloff;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Light& l) {
        is >> l.pos >> l.falloff;
        l.color = Color(1, 1, 1);
        return is;
    }

    void draw(){
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        glTranslatef(pos.x, pos.y, pos.z);
        glutSolidSphere(5, 100, 100);
        glPopMatrix();
    }
};

class SpotLight : public Light{
public:
    Vector dir;
    double angle;
    SpotLight() {}
    SpotLight(Point p, Color c, double f, Vector d, double a) {pos = p; color = c; falloff = f; dir = d; angle = a;}
    friend std::ostream& operator<<(std::ostream& os, const SpotLight& l) {
        os << l.pos << " " << l.color << " " << l.falloff << " " << l.dir << " " << l.angle;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, SpotLight& l) {
        Point temp;
        is >> l.pos >> l.falloff >> temp >> l.angle;
        l.dir = (temp - l.pos).normalize();
        l.color = Color(1, 1, 1);
        return is;
    }

    void draw(){
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        glTranslatef(pos.x, pos.y, pos.z);
        //draw a cone facing the direction of the light
        double xRot = acos(dir.y) * 180 / 3.1416;
        double zRot = acos(dir.x / sqrt(dir.x * dir.x + dir.z * dir.z)) * 180 / M_PI;
        if (dir.z < 0) zRot = -zRot;
        glRotatef(xRot, 1, 0, 0);
        glRotatef(zRot, 0, 0, 1);

        glutSolidCone(5, 20, 100, 100);
        glPopMatrix();
    }
};

class Object{
public:
    double ambient, diffuse, specular, reflection;
    int shine;
    char type;
    virtual void draw() = 0;
    virtual double intersect(Ray ray, Color &c) = 0;
    virtual boolean intersectLight(Ray ray, double distance) {return -1;}
    virtual void draw(double draw_distance, Point pos){return;};
    virtual Vector getNormal(Point p){return Vector(0, 0, 0);};
};

class Sphere : public Object{
public:
    Point center;
    double radius;
    double radius2;
    Color color;
    Sphere() {}
    Sphere(Point p, double r) {center = p; radius = r;color = Color(1,1,1);radius2 = r*r;}
    Sphere(Point p, double r, Color c) {center = p; radius = r;color = c;radius2 = r*r;}
    friend std::ostream& operator<<(std::ostream& os, const Sphere& s) {
        os << s.center << " " << s.radius;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Sphere& s) {
        is >> s.center >> s.radius >> s.color;
        is >> s.ambient >> s.diffuse >> s.specular >> s.reflection;
        is >> s.shine;
        s.radius2 = s.radius * s.radius;
        s.type = 's';
        return is;
    }

    void draw(){
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        glTranslatef(center.x, center.y, center.z);
        glutSolidSphere(radius, 100, 100);
        glPopMatrix();
    }

    double intersect(Ray ray, Color &c){
        Vector L = (center - ray.start);
        double t_ca = L * ray.dir;
        if (t_ca < 0) return -1;
        double d2 = L * L - t_ca * t_ca;
        if (d2 > radius2) return -1;
        double t_hc = sqrt(radius2 - d2);
        double t1 = t_ca - t_hc;
        double t2 = t_ca + t_hc;
        c = color;

        if (t1 < 0 && t2 < 0) return -1;
        if (t1 < 0) return t2;
        if (t2 < 0)  return t1;
        return (t1 < t2) ? t1 : t2;
    }

    boolean intersectLight(Ray ray, double distance){
        Vector L = (center - ray.start);
        double t_ca = L * ray.dir;
        if (t_ca < 0) return false;
        double d2 = L * L - t_ca * t_ca;
        if (d2 > radius2) return false;
        double t_hc = sqrt(radius2 - d2);
        double t1 = t_ca - t_hc;
        double t2 = t_ca + t_hc;
        if (t1 < 1e-5 && t2 < 1e-5) return false;
        if (t1 < 1e-5) return t2 < distance;
        if (t2 < 1e-5)  return t1 < distance;
        return (t1 < t2) ? t1 < distance : t2 < distance;
    }

    Vector getNormal(Point p){
        Vector v = (center - p);
        v.normalize();
        return v;
    }
};

class Cube : public Object{
public:
    Triangle faces[12];
    Point lowerLeft;
    double sideLength;
    Color color;
    Vector normal;

    Cube() {}
    Cube(Triangle t[]) {
        for (int i = 0; i < 12; i++) {
            faces[i] = t[i];
        }
        color = Color(1, 1, 1);
    }

    void generateFaces(){
        Point lowerRight = lowerLeft + Vector(sideLength, 0, 0);
        Point upperLeft = lowerLeft + Vector(0, sideLength, 0);
        Point upperRight = lowerLeft + Vector(sideLength, sideLength, 0);
        Point lowerLeftBack = lowerLeft + Vector(0, 0, sideLength);
        Point lowerRightBack = lowerLeft + Vector(sideLength, 0, sideLength);
        Point upperLeftBack = lowerLeft + Vector(0, sideLength, sideLength);
        Point upperRightBack = lowerLeft + Vector(sideLength, sideLength, sideLength);
        Triangle t1(lowerLeft, lowerRight, upperRight);
        Triangle t2(lowerLeft, upperRight, upperLeft);
        Triangle t3(lowerLeft, lowerLeftBack, lowerRightBack);
        Triangle t4(lowerLeft, lowerRightBack, lowerRight);
        Triangle t5(lowerLeft, upperLeft, upperLeftBack);
        Triangle t6(lowerLeft, upperLeftBack, lowerLeftBack);
        Triangle t7(upperLeft, upperRight, upperRightBack);
        Triangle t8(upperLeft, upperRightBack, upperLeftBack);
        Triangle t9(upperRight, lowerRight, lowerRightBack);
        Triangle t10(upperRight, lowerRightBack, upperRightBack);
        Triangle t11(lowerRightBack, lowerLeftBack, upperLeftBack);
        Triangle t12(lowerRightBack, upperLeftBack, upperRightBack);
        faces[0] = t1;
        faces[1] = t2;
        faces[2] = t3;
        faces[3] = t4;
        faces[4] = t5;
        faces[5] = t6;
        faces[6] = t7;
        faces[7] = t8;
        faces[8] = t9;
        faces[9] = t10;
        faces[10] = t11;
        faces[11] = t12;
    }

    Cube (Point ll, double l){
        lowerLeft = ll;
        sideLength = l;
        generateFaces();
        color = Color(1, 1, 1);
    }

    Cube (Point ll, double l, Color c){
        lowerLeft = ll;
        sideLength = l;
        generateFaces();
        color = c;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cube& c) {
        for (int i = 0; i < 12; i++) {
            os << c.faces[i] << std::endl;
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Cube& c) {
        is >> c.lowerLeft >> c.sideLength >> c.color;
        is >> c.ambient >> c.diffuse >> c.specular >> c.reflection;
        is >> c.shine;
        c.type = 'c';
        c.generateFaces();
        return is;
    }
    
    void draw(){
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        for (int i = 0; i < 12; i++) {
            faces[i].draw();
        }
        glPopMatrix();
    }

    double intersect(Ray ray, Color &c){
        double min_t = 1000;
        double t;
        double u, v;
        for (int i = 0; i < 12; i++) {
            t = faces[i].intersect(ray, u, v);
            if (t > 0 && t < min_t) {
                min_t = t;
                c = color;
                normal = faces[i].getNormal(ray.start + ray.dir * t);
            }
        }
        if (min_t == 1000) return -1;
        return min_t;
    }

    boolean intersectLight(Ray ray, double distance){
        double min_t = 1000;
        double t;
        double u, v;
        for (int i = 0; i < 12; i++) {
            t = faces[i].intersect(ray, u, v);
            if (t > 1e-5 && t < min_t) {
                min_t = t;
                if(min_t < distance) return true;
            }
        }
        if (min_t == 1000) return false;
        return min_t < distance;
    }

    Vector getNormal(Point p){
        return normal;
    }
};

class Pyramid : public Object{
public:
    Point lowestPoint;
    double width, height;
    Color color;
    Vector normal;

    Triangle sides[6];

    void generateFaces(){
        Point top = lowestPoint + Vector(0, height, 0);
        Point lowerLeft = lowestPoint + Vector(-width/2, 0, -width/2);
        Point lowerRight = lowestPoint + Vector(width/2, 0, -width/2);
        Point upperLeft = lowestPoint + Vector(-width/2, 0, width/2);
        Point upperRight = lowestPoint + Vector(width/2, 0, width/2);
        Triangle t1(top, lowerLeft, lowerRight);
        Triangle t2(top, lowerRight, upperRight);
        Triangle t3(top, upperRight, upperLeft);
        Triangle t4(top, upperLeft, lowerLeft);
        Triangle t5(lowerLeft, lowerRight, upperRight);
        Triangle t6(lowerLeft, upperRight, upperLeft);
        sides[0] = t1;
        sides[1] = t2;
        sides[2] = t3;
        sides[3] = t4;
        sides[4] = t5;
        sides[5] = t6;
    }

    Pyramid() {}
    Pyramid(Point p, double w, double h) {
        lowestPoint = p;
        width = w;
        height = h;
        color = Color(1, 1, 1);
        generateFaces();
    }

    Pyramid(Point p, double w, double h, Color c) {
        lowestPoint = p;
        width = w;
        height = h;
        color = c;
        generateFaces();
    }

    friend std::ostream& operator<<(std::ostream& os, const Pyramid& p) {
        os << p.lowestPoint << " " << p.width << " " << p.height;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Pyramid& p) {
        is >> p.lowestPoint >> p.width >> p.height >> p.color;
        is >> p.ambient >> p.diffuse >> p.specular >> p.reflection;
        is >> p.shine;
        p.lowestPoint = p.lowestPoint + Vector(p.width/2, 0, p.width/2);
        p.type = 'p';
        p.generateFaces();
        return is;
    }

    void draw(){
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        for (int i = 0; i < 6; i++) {
            sides[i].draw();
        }
        glPopMatrix();
    }

    double intersect(Ray ray, Color &c){
        double min_t = 1000;
        double t;
        double u, v;
        for (int i = 0; i < 6; i++) {
            t = sides[i].intersect(ray, u, v);
            if (t > 0 && t < min_t) {
                min_t = t;
                c = color;
                normal = sides[i].getNormal(ray.start + ray.dir * t);
            }
        }
        if (min_t == 1000) return -1;
        return min_t;
    }

    boolean intersectLight(Ray ray, double distance){
        double min_t = 1000;
        double t;
        double u, v;
        for (int i = 0; i < 6; i++) {
            t = sides[i].intersect(ray, u, v);
            if (t > 1e-5 && t < min_t) {
                min_t = t;
                if(min_t < distance) return true;
            }
        }
        if (min_t == 1000) return false;
        return min_t < distance;
    }

    Vector getNormal(Point p){
        return normal;
    }
};

#pragma pack(push, 1)
struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t dataSize;
    int32_t horizontalResolution;
    int32_t verticalResolution;
    uint32_t colors;
    uint32_t importantColors;
};
#pragma pack(pop)



class Texture{
public:
    vector<vector<uint8_t>> pixel_values;
    int width, height;
    Texture() {}
    void load(ifstream &fin){
        if (!fin) {
            cerr << "Failed to open file" << endl;
            return;
        }

        BMPHeader header;
        fin.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

        if (header.signature[0] != 'B' || header.signature[1] != 'M') {
            cerr << "Invalid BMP file" << endl;
            return;
        }

        if (header.bitsPerPixel != 24) {
            cerr << "Only 24-bit BMP files are supported" << endl;
            return;
        }

        width = header.width;
        height = header.height;
        int size = 3 * width * height;
        pixel_values = vector<vector<uint8_t>>(height, vector<uint8_t>(width * 3));

        // Seek to the beginning of the pixel data
        fin.seekg(header.dataOffset);

        for (int y = 0; y < height; ++y) {
            fin.read(reinterpret_cast<char*>(pixel_values[y].data()), width * 3);
        }

        fin.close();
    }

    Color getColor(double u, double v){
        int x = u * width;
        int y = v * height;
        return Color(pixel_values[y][x * 3] / 255.0, pixel_values[y][x * 3 + 1] / 255.0, pixel_values[y][x * 3 + 2] / 255.0);
    }
};

class CheckerBoard : public Object{
public:
    double width;
    Texture white;
    Texture black;
    boolean tex = false;

    void loadTexture(){
        ifstream fin;
        fin.open("texture_w.bmp", ios::binary);
        white.load(fin);
        fin.close();
        fin.open("texture_b.bmp", ios::binary);
        black.load(fin);
        fin.close();
    }

    CheckerBoard() {loadTexture();}
    CheckerBoard(double w) {width = w;}
    CheckerBoard(double w, double a, double d, double r) {width = w; ambient = a; diffuse = d; reflection = r;}
    friend std::ostream& operator<<(std::ostream& os, const CheckerBoard& c) {
        os << c.width << " " << c.ambient << " " << c.diffuse << " " << c.reflection;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, CheckerBoard& c) {
        is >> c.width >> c.ambient >> c.diffuse >> c.reflection;
        c.shine = 0;
        c.specular = 0;
        c.type = 'b';
        return is;
    }

    void draw(){
        return;
    }

    void draw(double draw_distance, Point pos){
        int noOfSquares = 2 * draw_distance / width;
        glPushMatrix();
        glTranslatef(-draw_distance + int(pos.x / width / 2) * width * 2, 0, -draw_distance + int(pos.z / width / 2) * width * 2);
        for (int i = 0; i < noOfSquares; i++) {
            for (int j = 0; j < noOfSquares; j++) {
                if ((i + j) % 2 == 0) {
                    glColor3f(0, 0, 0);
                } else {
                    glColor3f(1, 1, 1);
                }
                glBegin(GL_QUADS);
                glVertex3f(i * width, 0, j * width);
                glVertex3f(i * width, 0, (j + 1) * width);
                glVertex3f((i + 1) * width, 0, (j + 1) * width);
                glVertex3f((i + 1) * width, 0, j * width);
                glEnd();
            }
        }
        glPopMatrix();
    }

    double intersect(Ray ray, Color &c){
        double t = -ray.start.y / ray.dir.y;
        if (t < 0) return -1;
        Point p = ray.start + ray.dir * t;
        int x = round(p.x / width);
        int z = round(p.z / width);
        double tex_x = fabs((p.x - x * width) / width);
        double tex_y = fabs((p.z - z * width) / width); 
        if ((x + z) % 2 == 0) {
            if(tex) c = black.getColor(tex_x, tex_y);
            else c = Color(0, 0, 0);
        } else {
            if(tex) c = white.getColor(tex_x, tex_y);
            else c = Color(1, 1, 1);
        }
        return t;
    }

    boolean intersectLight(Ray ray, double distance){
        double t = -ray.start.y / ray.dir.y;
        if (t < 1e-5) return false;
        return true;
    }

    Vector getNormal(Point p){
        return Vector(0, 1, 0);
    }
};

Color trace(Ray ray, vector<Object*> objects, vector<Light> lights, vector<SpotLight> spotLights, double draw_distance, int recLevel){
    Color c(0, 0, 0);
    if (recLevel == 0) return c;
    double min_t = draw_distance;
    double t;
    Color tempColor;
    Object *obj;
    for (int i = 0; i < objects.size(); i++) {
        t = objects[i]->intersect(ray, tempColor);
        if (t > 1e-5 && t < min_t) {
            min_t = t;
            c = tempColor;
            obj = objects[i];
        }
    }
    if (min_t == draw_distance) return c;
    Point p = ray.start + ray.dir * min_t;
    Vector normal = obj->getNormal(p);
    if(normal * ray.dir > 0) normal = -normal;
    double lambert = 0;
    double phong = 0;
    for (int i = 0; i < lights.size(); i++) {
        Vector lightDir = lights[i].pos - p;        
        double distance = lightDir.length();
        lightDir.normalize();
        Ray shadowRay(p, lightDir);
        bool inShadow = false;
        for (int j = 0; j < objects.size(); j++) {
            if (objects[j]->intersectLight(shadowRay, distance)) {
                inShadow = true;
                break;
            }
        }
        if (!inShadow) {
            double scaling_factor = exp(-(distance * distance) * lights[i].falloff);

            lambert += max((lightDir * normal) * scaling_factor, 0.0);
            
            Vector reflection = lightDir - normal * (2 * (lightDir * normal));
            reflection.normalize();
            phong += max(pow((reflection * ray.dir), obj->shine) * scaling_factor, 0.0);
        }
    }

    for (int i = 0; i < spotLights.size(); i++) {
        Vector lightDir = spotLights[i].pos - p;        
        double distance = lightDir.length();
        lightDir.normalize();
        Ray shadowRay(p, lightDir);
        bool inShadow = false;
        if(fabs(acos((-lightDir * spotLights[i].dir)))  > spotLights[i].angle * M_PI / 180) inShadow = true;
        if(!inShadow){
            for (int j = 0; j < objects.size(); j++) {
                if (objects[j]->intersectLight(shadowRay, distance)) {
                    inShadow = true;
                    break;
                }
            }
        }
        if (!inShadow) {
            double scaling_factor = exp(-distance * distance * lights[i].falloff);
            lambert += max((lightDir * normal) * scaling_factor, 0.0);

            Vector reflection = lightDir - normal * (2 * (lightDir * normal));
            reflection.normalize();
            phong += max(pow((reflection * ray.dir), obj->shine) * scaling_factor, 0.0);

        }
    }

    Vector reflected = ray.dir - normal * (2 * (ray.dir * normal));
    reflected.normalize();
    Ray reflectedRay(ray.start + ray.dir * min_t, reflected);
    Color reflectedColor = trace(reflectedRay, objects, lights, spotLights, draw_distance, recLevel - 1);
    c = c * (obj->diffuse * lambert + obj->specular * phong + obj->ambient) + reflectedColor * obj->reflection;
    return c;
}

#endif