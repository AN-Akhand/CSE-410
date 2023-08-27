#include <vector>
#include <iostream>
#include <cmath>
#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#ifndef OBJECTS_H


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
    void normalize() {double l = length(); x /= l; y /= l; z /= l;}
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
        is >> p.x >> p.y >> p.z;
        return is;
    }
};

class Color {
public:
    double r, g, b;
    Color() {r = 0; g = 0; b = 0;}
    Color(double a, double c, double d) {r = a; g = c; b = d;}
    Color operator=(Color c) {r = c.r; g = c.g; b = c.b; return *this;}
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
};

class Sphere {
public:
    Point center;
    double radius;
    Color color;
    double ambient, diffuse, specular, reflection;
    int shine;
    Sphere() {}
    Sphere(Point p, double r) {center = p; radius = r;color = Color(1,1,1);}
    Sphere(Point p, double r, Color c) {center = p; radius = r;color = c;}
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
};

class Pyramid {
public:
    Point lowestPoint;
    double width, height;
    Color color;
    double ambient, diffuse, specular, reflection;
    int shine;

    Triangle sides[4];
    Triangle base[2];

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
        base[0] = t5;
        base[1] = t6;
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
        for (int i = 0; i < 2; i++) {
            base[i].draw();
        }
        glPopMatrix();
    }
};

#define OBJECTS_H
#endif