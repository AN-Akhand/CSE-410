#include "MatVec.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>

#ifndef TRANSFORMATIONUTILS_H
#define TRANSFORMATIONUTILS_H

void printPoint(Matrix p, ofstream &os){
    os << fixed << p.data[0][0] << " " << p.data[1][0] << " " << p.data[2][0] << endl;
}

Matrix getPointMatrix(double x, double y, double z){
    Matrix point = Matrix(4, 1);
    point.data[0][0] = x;
    point.data[1][0] = y;
    point.data[2][0] = z;
    point.data[3][0] = 1;
    return point;
}

Matrix getPointMatrix(Matrix p){
    Matrix point = Matrix(4, 1);
    point.data[0][0] = p.data[0][0];
    point.data[1][0] = p.data[1][0];
    point.data[2][0] = p.data[2][0];
    point.data[3][0] = 1;
    return point;
}

Matrix getTranslationMatrix(double x, double y, double z){
    Matrix translation = Matrix::getIdentityMatrix(4);
    translation.data[0][3] = x;
    translation.data[1][3] = y;
    translation.data[2][3] = z;
    return translation;
}

Matrix getTranslationMatrix(Matrix p){
    Matrix translation = Matrix::getIdentityMatrix(4);
    translation.data[0][3] = p.data[0][0];
    translation.data[1][3] = p.data[1][0];
    translation.data[2][3] = p.data[2][0];
    return translation;
}

Matrix getScaleMatrix(double x, double y, double z){
    Matrix scale = Matrix::getIdentityMatrix(4);
    scale.data[0][0] = x;
    scale.data[1][1] = y;
    scale.data[2][2] = z;
    return scale;
}

Matrix getScaleMatrix(Matrix p){
    Matrix scale = Matrix::getIdentityMatrix(4);
    scale.data[0][0] = p.data[0][0];
    scale.data[1][1] = p.data[1][0];
    scale.data[2][2] = p.data[2][0];
    return scale;
}

Matrix applyRodriguesFormula(Vector x, Vector a, double angle){
    double rad = angle * M_PI / 180;
    double c = cos(rad);
    double s = sin(rad);
    a.normalize();
    return x * c + a * (1 - c) * a.dot(x) + a.cross(x) * s;
}

Matrix getRotationMatrix(double angle, double x, double y, double z){
    Matrix rotation = Matrix::getIdentityMatrix(4);

    Vector a = Vector(x, y, z);
    Vector i = Vector(1, 0, 0);
    Vector j = Vector(0, 1, 0);
    Vector k = Vector(0, 0, 1);

    Vector c1 = applyRodriguesFormula(i, a, angle);
    Vector c2 = applyRodriguesFormula(j, a, angle);
    Vector c3 = applyRodriguesFormula(k, a, angle);

    for(int i = 0; i < 3; i++){
        rotation.data[i][0] = c1.data[i][0];
        rotation.data[i][1] = c2.data[i][0];
        rotation.data[i][2] = c3.data[i][0];
    }
    return rotation;
}

Matrix getRotationMatrix(double angle, Matrix p){
    Matrix rotation = Matrix::getIdentityMatrix(4);

    Vector a = Vector(p);
    Vector i = Vector(1, 0, 0);
    Vector j = Vector(0, 1, 0);
    Vector k = Vector(0, 0, 1);

    Vector c1 = applyRodriguesFormula(i, a, angle);
    Vector c2 = applyRodriguesFormula(j, a, angle);
    Vector c3 = applyRodriguesFormula(k, a, angle);

    for(int i = 0; i < 3; i++){
        rotation.data[i][0] = c1.data[i][0];
        rotation.data[i][1] = c2.data[i][0];
        rotation.data[i][2] = c3.data[i][0];
    }
    return rotation;
}

Matrix transform(Matrix point, Matrix transformation){
    Matrix result = transformation * point;
    result = result / result.data[3][0];
    return result;
}  

#endif // TRANSFORMATIONUTILS_H