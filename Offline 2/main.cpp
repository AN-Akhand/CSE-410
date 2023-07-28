#include "MatVec.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include "TransformationUtils.h"
using namespace std;

void stage1();
void stage2(Matrix V);
void stage3(Matrix P);

int triangleCount = 0;
ifstream fin;
ofstream fout;
Matrix point(4, 1);
Matrix temp(3,1);

int main(int argc, char** argv){

    cout.precision(7);
    fout.precision(7);

    string path = string("scene.txt");

    fin.open(path);
    fout.open("stage1.txt");


    double eyex, eyey, eyez, lookx, looky, lookz, upx, upy, upz, fovy, aspect, near, far;
    fin >> eyex >> eyey >> eyez;
    fin >> lookx >> looky >> lookz;
    fin >> upx >> upy >> upz;
    fin >> fovy >> aspect >> near >> far;

    Matrix eye = getPointMatrix(eyex, eyey, eyez);
    Matrix look = getPointMatrix(lookx, looky, lookz);

    Vector l = Vector::getVectorFromPoints(eye, look);
    l.normalize();
    Vector up = Vector(upx, upy, upz);
    Vector r = l.cross(up);
    r.normalize();
    Vector u = r.cross(l);

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

    double fovx = fovy * aspect;
    double t = near * tan(fovy * M_PI / 360);
    double rr = near * tan(fovx * M_PI / 360);

    Matrix P = Matrix::getIdentityMatrix(4);
    P.data[0][0] = near / rr;
    P.data[1][1] = near / t;
    P.data[2][2] = -(far + near) / (far - near);
    P.data[3][3] = 0;
    P.data[2][3] = -(2 * far * near) / (far - near);
    P.data[3][2] = -1;


    stage1();

    fin.open("stage1.txt");
    fout.open("stage2.txt");

    stage2(V);

    fin.open("stage2.txt");
    fout.open("stage3.txt");

    stage3(P);

}

void stage1(){
    stack<Matrix> matStack = stack<Matrix>();
    Matrix curr = Matrix::getIdentityMatrix(4);
    matStack.push(curr);

    while(true){
        string command;
        fin >> command;
        if(command == "triangle"){
            triangleCount++;
            for(int i = 0; i < 3; i++){
                fin >> temp;
                Matrix point = getPointMatrix(temp);
                printPoint(transform(point, curr), fout);
            }
            fout << endl;
        }
        else if(command == "translate"){
            fin >> temp;
            Matrix translation = getTranslationMatrix(temp);
            curr = curr * translation;
        }
        else if(command == "scale"){
            fin >> temp;
            Matrix scale = getScaleMatrix(temp);
            curr = curr * scale;
        }
        else if(command == "rotate"){
            double angle;
            fin >> angle;
            fin >> temp;
            Matrix rotation = getRotationMatrix(angle, temp);
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
}

void stage2(Matrix V){
    for(int i = 0; i < triangleCount; i++){
        for(int i = 0; i < 3; i++){
            fin >> temp;
            point = getPointMatrix(temp);
            printPoint(transform(point, V), fout);
        }
        fout << endl;
    }
    fin.close();
    fout.close();
}

void stage3(Matrix P){
    for(int i = 0; i < triangleCount; i++){
        for(int i = 0; i < 3; i++){
            fin >> temp;
            point = getPointMatrix(temp);
            printPoint(transform(point, P), fout);
        }
        fout << endl;
    }
    fin.close();
    fout.close();
}
