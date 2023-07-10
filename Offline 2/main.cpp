#include "MatVec.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include "TransformationUtils.h"
using namespace std;

void stage1();
void stage2(Matrix V);

int triangleCount = 0;
ifstream fin;
ofstream fout;
Matrix point(4, 1);
Matrix p(3,1);

int main(){

    cout.precision(7);
    fout.precision(7);

    fin.open("scene.txt");
    fout.open("stage1.txt");


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

    stage1();

    fin.open("stage1.txt");
    fout.open("stage2.txt");

    stage2(V);

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
}

void stage2(Matrix V){
    for(int i = 0; i < triangleCount; i++){
        for(int i = 0; i < 3; i++){
            fin >> p;
            point = getPointMatrix(p);
            printPoint(V * point, fout);
        }
        fout << endl;
    }
    fin.close();
    fout.close();
}

void stage3(){
    
}
