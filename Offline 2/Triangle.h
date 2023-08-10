#include "MatVec.h"
#include <iostream>
#include "TransformationUtils.h"

#ifndef TRIANGLE_H
#define TRIANGLE_H

using namespace std;

class Point{
public:
    double x, y, z;

    Point(){
        x = 0;
        y = 0;
        z = 0;
    }

    Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    friend istream& operator>>(istream& is, Point& p){
        is >> p.x >> p.y >> p.z;
        return is;
    }

    friend ostream& operator<<(ostream& os, Point& p){
        os << fixed << p.x << " " << p.y << " " << p.z << " " << endl;
        return os;
    }
};

class Triangle {
public:

    Point p1, p2, p3;

    int r, g, b;

    Triangle(){
        p1 = Point();
        p2 = Point();
        p3 = Point();
        r = random();
        g = random();
        b = random();
    }

    friend istream& operator>>(istream& is, Triangle& t){
        is >> t.p1 >> t.p2 >> t.p3;
        return is;
    }

    friend ostream& operator<<(ostream& os, Triangle& t){
        os << t.p1 << t.p2 << t.p3 << endl;
        return os;
    }
};

#endif //TRIANGLE_H