#include <vector>
#include <iostream>
#include <cmath>
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include "Matrix.h"

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
        return is;
    }
};

Vector vectorFromPoints(Point p1, Point p2) {
    return Vector(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
}

class Ray {
public:
    Point start;
    Vector dir;
    Ray() {}
    Ray(Point p, Vector v) {start = p; dir = v.normalize();}
    Ray(Point p1, Point p2) {start = p2; dir = vectorFromPoints(p1, p2).normalize();}
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
        Vector edge1 = vectorFromPoints(a, b);
        Vector edge2 = vectorFromPoints(a, c);
        Vector h = ray.dir ^ edge2;
        double det = edge1 * h;
        if (fabs(det) < 0.00001) return -1;
        double f = 1 / det;
        Vector s = vectorFromPoints(a, ray.start);
        u = f * (s * h);
        if (u < 0.0 || u > 1.0) return -1;
        Vector q = s ^ edge1;
        v = f * (ray.dir * q);
        if (v < 0.0 || u + v > 1.0) return -1;
        double t = f * (edge2 * q);
        if (t > 0) return t;
        return -1;
    }

    // double intersect(Ray ray){
    //     Matrix A(3, 3, new double*[3]{
    //         new double[3]{a.x-b.x, a.x-c.x, ray.dir.x},
    //         new double[3]{a.y-b.y, a.y-c.y, ray.dir.y},
    //         new double[3]{a.z-b.z, a.z-c.z, ray.dir.z}
    //     });
    //     Matrix B(3, 3, new double*[3]{
    //         new double[3]{a.x-ray.start.x, a.x-c.x, ray.dir.x},
    //         new double[3]{a.y-ray.start.y, a.y-c.y, ray.dir.y},
    //         new double[3]{a.z-ray.start.z, a.z-c.z, ray.dir.z}
    //     });
    //     Matrix C(3, 3, new double*[3]{
    //         new double[3]{a.x-b.x, a.x-ray.start.x, ray.dir.x},
    //         new double[3]{a.y-b.y, a.y-ray.start.y, ray.dir.y},
    //         new double[3]{a.z-b.z, a.z-ray.start.z, ray.dir.z}
    //     });
    //     Matrix T(3, 3, new double*[3]{
    //         new double[3]{a.x-b.x, a.x-c.x, a.x-ray.start.x},
    //         new double[3]{a.y-b.y, a.y-c.y, a.y-ray.start.y},
    //         new double[3]{a.z-b.z, a.z-c.z, a.z-ray.start.z}
    //     });

    //     double detA = A.determinant();
    //     double detB = B.determinant();
    //     double detC = C.determinant();
    //     double detT = T.determinant();

    //     double beta = detB / detA;
    //     double gamma = detC / detA;
    //     double t = detT / detA;

    //     if (beta >= 0 && gamma >= 0 && beta + gamma <= 1 && t > 0) {
    //         return t;
    //     }
    //     return -1;
    // }
};

class Sphere {
public:
    Point center;
    double radius;
    double radius2;
    Color color;
    double ambient, diffuse, specular, reflection;
    int shine;
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
        Vector L = vectorFromPoints(ray.start, center);
        double t_ca = L * ray.dir;
        if (t_ca < 0) return -1;
        double d2 = L * L - t_ca * t_ca;
        if (d2 > radius2) return -1;
        double t_hc = sqrt(radius2 - d2);
        double t1 = t_ca - t_hc;
        double t2 = t_ca + t_hc;
        c = color;

        if (t1 < 0 && t2 < 0) return -1;
        if (t1 < 0) return t1;
        if (t2 < 0)  return t2;
        return (t1 < t2) ? t2 : t1;
    }
};

class Cube {
public:
    Triangle faces[12];
    Point lowerLeft;
    double sideLength;
    Color color;
    double ambient, diffuse, specular, reflection;
    int shine;

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

    double intersect(Color &c, Ray ray){
        double min_t = 1000;
        double t;
        double u, v;
        for (int i = 0; i < 12; i++) {
            t = faces[i].intersect(ray, u, v);
            if (t > 0 && t < min_t) {
                min_t = t;
                c = color;
            }
        }
        if (min_t == 1000) return -1;
        return min_t;
    }
};

class Pyramid {
public:
    Point lowestPoint;
    double width, height;
    Color color;
    double ambient, diffuse, specular, reflection;
    int shine;

    Triangle sides[4];

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
        sides[0] = t1;
        sides[1] = t2;
        sides[2] = t3;
        sides[3] = t4;
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
        p.generateFaces();
        return is;
    }

    void draw(){
        glPushMatrix();
        glColor3f(color.r, color.g, color.b);
        for (int i = 0; i < 4; i++) {
            sides[i].draw();
        }
        glPopMatrix();
    }

    double intersect(Color &c, Ray ray){
        double min_t = 1000;
        double t;
        double u, v;
        for (int i = 0; i < 4; i++) {
            t = sides[i].intersect(ray, u, v);
            if (t > 0 && t < min_t) {
                min_t = t;
                c = color;
            }
        }
        if (min_t == 1000) return -1;
        return min_t;
    }
};

class CheckerBoard {
public:
    double width;
    double ambient, diffuse, reflection;

    CheckerBoard() {}
    CheckerBoard(double w) {width = w;}
    CheckerBoard(double w, double a, double d, double r) {width = w; ambient = a; diffuse = d; reflection = r;}
    friend std::ostream& operator<<(std::ostream& os, const CheckerBoard& c) {
        os << c.width << " " << c.ambient << " " << c.diffuse << " " << c.reflection;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, CheckerBoard& c) {
        is >> c.width >> c.ambient >> c.diffuse >> c.reflection;
        return is;
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

    double intersect(Color &c, Ray ray){
        double t = -ray.start.y / ray.dir.y;
        if (t < 0) return -1;
        Point p = ray.start + ray.dir * t;
        int x = int(p.x / width);
        int z = int(p.z / width);
        if ((x + z) % 2 == 0) {
            c = Color(0, 0, 0);
        } else {
            c = Color(1, 1, 1);
        }
        return t;
    }
};

Color trace(Ray ray, vector<Sphere> spheres, vector<Cube> cubes, vector<Pyramid> pyramids, CheckerBoard chk, int recLevel){
    Color c(0, 0, 0);
    if (recLevel == 0) return c;
    double min_t = 1000;
    double t;
    Color tempColor;
    for (int i = 0; i < spheres.size(); i++) {
        t = spheres[i].intersect(ray, tempColor);
        if (t > 0 && t < min_t) {
            min_t = t;
            c = tempColor;
        }
    }
    for (int i = 0; i < cubes.size(); i++) {
        t = cubes[i].intersect(tempColor, ray);
        if (t > 0 && t < min_t) {
            min_t = t;
            c = tempColor;
        }
    }
    for (int i = 0; i < pyramids.size(); i++) {
        t = pyramids[i].intersect(tempColor, ray);
        if (t > 0 && t < min_t) {
            min_t = t;
            c = tempColor;
        }
    }
    t = chk.intersect(tempColor, ray);
    if (t > 0 && t < min_t) {
        min_t = t;
        c = tempColor;
    }
    if (min_t == 1000) return c * 255;
    return c * 255;
}

#endif