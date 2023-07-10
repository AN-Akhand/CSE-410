#include "MatVec.cpp"
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
using namespace std;

void printPoint(Matrix p, ofstream &os);

Matrix getPointMatrix(double x, double y, double z);
Matrix getVectorMatrix(double x, double y, double z);
Matrix getTranslationMatrix(double x, double y, double z);
Matrix getScaleMatrix(double x, double y, double z);
Matrix getRotationMatrix(double angle, double x, double y, double z);
Matrix applyRodriguesFormula(Matrix x, Matrix a, double angle);

int main(){
    stack<Matrix> matStack = stack<Matrix>();
    Matrix curr = Matrix::getIdentityMatrix(4);
    matStack.push(curr);

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

    while(true){
        string command;
        fin >> command;
        if(command == "triangle"){
            for(int i = 0; i < 3; i++){
                double x, y, z;
                fin >> x >> y >> z;
                Matrix point = getPointMatrix(x, y, z);
                Matrix t = curr * point;
                printPoint(t, fout);
            }
            fout << endl;
        }
        else if(command == "translate"){
            double x, y, z;
            fin >> x >> y >> z;
            Matrix translation = getTranslationMatrix(x, y, z);
            curr = curr * translation;
        }
        else if(command == "scale"){
            double x, y, z;
            fin >> x >> y >> z;
            Matrix scale = getScaleMatrix(x, y, z);
            curr = curr * scale;
        }
        else if(command == "rotate"){
            double angle, x, y, z;
            fin >> angle >> x >> y >> z;
            Matrix rotation = getRotationMatrix(angle, x, y, z);
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

Matrix getVectorMatrix(double x, double y, double z){
    Matrix vector = Matrix(3, 1);
    vector.data[0][0] = x;
    vector.data[1][0] = y;
    vector.data[2][0] = z;
    return vector;
}

Matrix getTranslationMatrix(double x, double y, double z){
    Matrix translation = Matrix::getIdentityMatrix(4);
    translation.data[0][3] = x;
    translation.data[1][3] = y;
    translation.data[2][3] = z;
    return translation;
}

Matrix getScaleMatrix(double x, double y, double z){
    Matrix scale = Matrix::getIdentityMatrix(4);
    scale.data[0][0] = x;
    scale.data[1][1] = y;
    scale.data[2][2] = z;
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

Matrix applyRodriguesFormula(Matrix x, Matrix a, double angle){
    double rad = angle * M_PI / 180;
    double c = cos(rad);
    double s = sin(rad);
    a = a.normalize();
    return x * c + a * (1 - c) * a.getDotProduct(x) + a.getCrossProduct(x) * s;
}
