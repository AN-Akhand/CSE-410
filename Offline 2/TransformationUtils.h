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

Matrix getVectorMatrix(double x, double y, double z){
    Matrix vector = Matrix(4, 1);
    vector.data[0][0] = x;
    vector.data[1][0] = y;
    vector.data[2][0] = z;
    vector.data[3][0] = 0;
    return vector;
}

Matrix getVectorMatrix(Matrix p){
    Matrix vector = Matrix(4, 1);
    vector.data[0][0] = p.data[0][0];
    vector.data[1][0] = p.data[1][0];
    vector.data[2][0] = p.data[2][0];
    vector.data[3][0] = 0;
    return vector;
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

Matrix applyRodriguesFormula(Matrix x, Matrix a, double angle){
    double rad = angle * M_PI / 180;
    double c = cos(rad);
    double s = sin(rad);
    a.normalize();
    return x * c + a * (1 - c) * Matrix::getDotProduct(a, x) + Matrix::getCrossProduct(a, x) * s;
}

Matrix getRotationMatrix(double angle, double x, double y, double z){
    Matrix rotation = Matrix::getIdentityMatrix(4);

    Matrix a = getVectorMatrix(x, y, z);
    Matrix i = getVectorMatrix(1, 0, 0);
    Matrix j = getVectorMatrix(0, 1, 0);
    Matrix k = getVectorMatrix(0, 0, 1);

    Matrix c1 = applyRodriguesFormula(i, a, angle);
    Matrix c2 = applyRodriguesFormula(j, a, angle);
    Matrix c3 = applyRodriguesFormula(k, a, angle);

    for(int i = 0; i < 3; i++){
        rotation.data[i][0] = c1.data[i][0];
        rotation.data[i][1] = c2.data[i][0];
        rotation.data[i][2] = c3.data[i][0];
    }
    return rotation;
}

Matrix getRotationMatrix(double angle, Matrix p){
    Matrix rotation = Matrix::getIdentityMatrix(4);

    Matrix a = getVectorMatrix(p);
    Matrix i = getVectorMatrix(1, 0, 0);
    Matrix j = getVectorMatrix(0, 1, 0);
    Matrix k = getVectorMatrix(0, 0, 1);

    Matrix c1 = applyRodriguesFormula(i, a, angle);
    Matrix c2 = applyRodriguesFormula(j, a, angle);
    Matrix c3 = applyRodriguesFormula(k, a, angle);

    for(int i = 0; i < 3; i++){
        rotation.data[i][0] = c1.data[i][0];
        rotation.data[i][1] = c2.data[i][0];
        rotation.data[i][2] = c3.data[i][0];
    }
    return rotation;
}

Matrix transform(Matrix point, Matrix transformation){
    Matrix result = transformation * point;
    for(int i = 0; i < 4; i++){
        result.data[i][0] /= result.data[3][0];
    }
    return result;
}  

#endif // TRANSFORMATIONUTILS_H