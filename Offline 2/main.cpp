#include "MatVec.cpp"
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
using namespace std;

void printPoint(Matrix p, ofstream &os);

Matrix getPointMatrix(double x, double y, double z);
Matrix getPointMatrix(Matrix p);
Matrix getVectorMatrix(double x, double y, double z);
Matrix getVectorMatrix(Matrix p);
Matrix getTranslationMatrix(double x, double y, double z);
Matrix getTranslationMatrix(Matrix p);
Matrix getScaleMatrix(double x, double y, double z);
Matrix getScaleMatrix(Matrix p);
Matrix getRotationMatrix(double angle, double x, double y, double z);
Matrix getRotationMatrix(double angle, Matrix p);
Matrix applyRodriguesFormula(Matrix x, Matrix a, double angle);

int main(){
    stack<Matrix> matStack = stack<Matrix>();
    Matrix curr = Matrix::getIdentityMatrix(4);
    matStack.push(curr);

    int triangleCount = 0;

    ifstream fin;
    fin.open("scene.txt");

    ofstream fout;
    fout.open("stage1.txt");

    cout.precision(7);
    fout.precision(7);

    double eyex, eyey, eyez, lookx, looky, lookz, upx, upy, upz, fovy, aspect, near, far;
    fin >> eyex >> eyey >> eyez;
    fin >> lookx >> looky >> lookz;
    fin >> upx >> upy >> upz;
    fin >> fovy >> aspect >> near >> far;

    Matrix eye = getPointMatrix(eyex, eyey, eyez);
    Matrix look = getPointMatrix(lookx, looky, lookz);

    Matrix l = Matrix::getVectFromPoints(eye, look);
    l.normalize();
    Matrix up = getVectorMatrix(upx, upy, upz);
    Matrix r = Matrix::getCrossProduct(l, up);
    r.normalize();
    Matrix u = Matrix::getCrossProduct(r, l);

    Matrix T = Matrix::getIdentityMatrix(4);
    T.data[0][3] = -eyex;
    T.data[1][3] = -eyey;
    T.data[2][3] = -eyez;

    Matrix R = Matrix::getIdentityMatrix(4);
    for(int i = 0; i < 3; i++){
        R.data[0][i] = r.data[i][0];
        R.data[1][i] = u.data[i][0];
        R.data[2][i] = -l.data[i][0];
    }

    Matrix V = R * T;

    Matrix point(4, 1);
    Matrix p(3,1);

    while(true){
        string command;
        fin >> command;
        if(command == "triangle"){
            triangleCount++;
            for(int i = 0; i < 3; i++){
                fin >> p;
                Matrix point = getPointMatrix(p);
                Matrix t = curr * point;
                printPoint(t, fout);
            }
            fout << endl;
        }
        else if(command == "translate"){
            fin >> p;
            Matrix translation = getTranslationMatrix(p);
            curr = curr * translation;
        }
        else if(command == "scale"){
            fin >> p;
            Matrix scale = getScaleMatrix(p);
            curr = curr * scale;
        }
        else if(command == "rotate"){
            double angle;
            fin >> angle;
            fin >> p;
            Matrix rotation = getRotationMatrix(angle, p);
            curr = curr * rotation;
        }
        else if(command == "push"){
            matStack.push(curr);
        }
        else if(command == "pop"){
            curr = matStack.top();
            matStack.pop();
        }
        else if(command == "end"){
            break;
        }
    }

    fin.close();
    fout.close();

    fin.open("stage1.txt");
    fout.open("stage2.txt");

    for(int i = 0; i < triangleCount; i++){
        for(int i = 0; i < 3; i++){
            fin >> p;
            point = getPointMatrix(p);
            printPoint(V * point, fout);
        }
        fout << endl;
    }
}

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

Matrix applyRodriguesFormula(Matrix x, Matrix a, double angle){
    double rad = angle * M_PI / 180;
    double c = cos(rad);
    double s = sin(rad);
    a.normalize();
    return x * c + a * (1 - c) * Matrix::getDotProduct(a, x) + Matrix::getCrossProduct(a, x) * s;
}
