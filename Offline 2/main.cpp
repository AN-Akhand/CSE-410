#include "MatVec.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include "TransformationUtils.h"
#include "Triangle.h"
#include <vector>
#include "bitmap_image.hpp"

using namespace std;

void stage1();
void stage2(Matrix V);
void stage3(Matrix P);
void stage4();

int triangleCount = 0;
ifstream fin;
ofstream fout;
Matrix point(4, 1);
Matrix temp(3,1);

int main(int argc, char** argv){

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

    fin.open("stage3.txt");
    fout.open("z_buffer.txt");

    stage4();

}


void stage4(){
    Triangle triangles[triangleCount];

    for(int i = 0; i < triangleCount; i++){
        fin >> triangles[i];
    }

    fin.close();
    fin.open("config.txt");
    int screen_width, screen_height;
    fin >> screen_width >> screen_height;
    fin.close();

    double right_limit, left_limit, top_limit, bottom_limit;
    right_limit = 1.0;
    left_limit = -1.0;
    top_limit = 1.0;
    bottom_limit = -1.0;
    double z_max = 1.0, z_min = -1.0;

    double dx = (right_limit - left_limit) / screen_width;
    double dy = (top_limit - bottom_limit) / screen_height;

    double top_y = top_limit - dy / 2;
    double bottom_y = bottom_limit + dy / 2;
    double left_x = left_limit + dx / 2;
    double right_x = right_limit - dx / 2;

    vector <vector <double> > z_buffer(screen_height, vector <double> (screen_width, z_max));
    bitmap_image image(screen_width, screen_height);

    for(int i = 0; i < screen_width; i++){
        for(int j = 0; j < screen_height; j++){
            image.set_pixel(i, j, 0, 0, 0);
        }
    }

    for(Triangle triangle : triangles){

        double max_y = max(triangle.p1.y, max(triangle.p2.y, triangle.p3.y));
        double min_y = min(triangle.p1.y, min(triangle.p2.y, triangle.p3.y));
        double max_x = max(triangle.p1.x, max(triangle.p2.x, triangle.p3.x));
        double min_x = min(triangle.p1.x, min(triangle.p2.x, triangle.p3.x));

        Point top = triangle.p1.y == max_y ? triangle.p1 : triangle.p2.y == max_y ? triangle.p2 : triangle.p3;
        Point left = triangle.p1.x == min_x ? triangle.p1 : triangle.p2.x == min_x ? triangle.p2 : triangle.p3;
        Point right = triangle.p1.x == max_x ? triangle.p1 : triangle.p2.x == max_x ? triangle.p2 : triangle.p3;

        double AB_y = top.y - left.y;
        double AB_z = top.z - left.z;

        double AC_y = top.y - right.y;
        double AC_z = top.z - right.z;

        max_y = min(max_y, top_y);
        min_y = max(min_y, bottom_y);


        int top_scanline = int((top_y - max_y) / dy);
        int bottom_scanline = int((top_y - min_y) / dy);

        for(int i = top_scanline + 1; i <= bottom_scanline; i++){
            double y = top_y - i * dy;

            double x1, x2;
            if(y == top.y){
                x1 = top.x;
            }
            else{
                x1 = top.x + (y - top.y) / (left.y - top.y) * (left.x - top.x);
            }

            if(y == top.y){
                x2 = top.x;
            }
            else{
                x2 = top.x + (y - top.y) * (right.x - top.x) / (right.y - top.y);
            }

            x1 = max(x1, left_x);
            x2 = min(x2, right_x);

            int left_column = round((x1 - left_x) / dx);
            int right_column = round((x2 - left_x) / dx);

            for(int j = left_column; j <= right_column; j++){
                
                double x = left_x + j * dx;
                double t1 = (y - top.y) / AB_y;
                double z1 = top.z + t1 * AB_z;
                double t2 = (y - top.y) / AC_y;
                double z2 = top.z + t2 * AC_z;
                double z = z1 + (x - x1) * (z2 - z1) / (x2 -x1);

                if(z < z_buffer[i][j] && z > z_min){
                    z_buffer[i][j] = z;
                    image.set_pixel(j, i, triangle.r, triangle.g, triangle.b);
                }
            } 
        }
    }

    fout.precision(6);

    for(int i = 0; i < screen_width; i++){
        for(int j = 0; j < screen_height; j++){
            if(z_buffer[i][j] < z_max)
                fout << z_buffer[i][j] << "\t";
        }
        fout << endl;
    }

    image.save_image("out.bmp");

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
