#include <iostream>
#include <cmath>

using namespace std;

#ifndef MATVEC_H
#define MATVEC_H


class Matrix {

public:
    int rows;
    int cols;
    double **data;

    Matrix(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        data = new double *[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
        }
    }

    Matrix(int n) {
        rows = n;
        cols = n;
        data = new double *[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
        }
    }

    Matrix(const Matrix &m) {
        rows = m.rows;
        cols = m.cols;
        data = new double *[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
            for (int j = 0; j < cols; j++) {
                data[i][j] = m.data[i][j];
            }
        }
    }

    ~Matrix() {
        for (int i = 0; i < rows; i++) {
            delete[] data[i];
        }
        delete[] data;
    }

    Matrix &operator=(const Matrix &m) {
        if (this == &m) {
            return *this;
        }
        for (int i = 0; i < rows; i++) {
            delete[] data[i];
        }
        delete[] data;
        rows = m.rows;
        cols = m.cols;
        data = new double *[rows];
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols];
            for (int j = 0; j < cols; j++) {
                data[i][j] = m.data[i][j];
            }
        }
        return *this;
    }

    Matrix operator+(const Matrix &m) {
        Matrix result(rows, cols);
        if (rows != m.rows || cols != m.cols) {
            cout << "Error: Dimensions not equal." << endl;
            exit(-1);
            return result;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] + m.data[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix &m) {
        Matrix result(rows, cols);
        if (rows != m.rows || cols != m.cols) {
            cout << "Error: Dimensions not equal." << endl;
            return result;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] - m.data[i][j];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix &m) {
        Matrix result(rows, m.cols);
        if (cols != m.rows) {
            cout << "Error: Dimensions not right." << endl;
            return result;
        }
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                result.data[i][j] = 0;
                for (int k = 0; k < cols; k++) {
                    result.data[i][j] += data[i][k] * m.data[k][j];
                }
            }
        }
        return result;
    }

    Matrix operator*(const double &d) {
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] * d;
            }
        }
        return result;
    }

    Matrix operator/(const double &d) {
        Matrix result(rows, cols);
        if (d == 0) {
            cout << "Error: Division by zero." << endl;
            return result;
        }
        result = *this * (1 / d);
        return result;
    }

    Matrix getInverseMatrix() {
        Matrix result(rows, cols);
        if (rows != cols) {
            cout << "Error: Not a square matrix." << endl;
            return result;
        }
        Matrix temp(*this);
        result = Matrix::getIdentityMatrix(rows);
        for (int i = 0; i < rows; i++) {
            if (temp.data[i][i] == 0) {
                for (int j = i + 1; j < rows; j++) {
                    if (temp.data[j][i] != 0) {
                        for (int k = 0; k < rows; k++) {
                            temp.data[i][k] += temp.data[j][k];
                            result.data[i][k] += result.data[j][k];
                        }
                        break;
                    }
                }
            }
            if (temp.data[i][i] == 0) {
                cout << "Error: Not invertible." << endl;
                return result;
            }
            for (int j = 0; j < rows; j++) {
                if (j == i) {
                    continue;
                }
                double ratio = temp.data[j][i] / temp.data[i][i];
                for (int k = 0; k < rows; k++) {
                    temp.data[j][k] -= ratio * temp.data[i][k];
                    result.data[j][k] -= ratio * result.data[i][k];
                }
            }
        }
        for (int i = 0; i < rows; i++) {
            double ratio = temp.data[i][i];
            for (int j = 0; j < rows; j++) {
                temp.data[i][j] /= ratio;
                result.data[i][j] /= ratio;
            }
        }
        return result;
    }

    Matrix getTransposeMatrix() const{
        Matrix result(cols, rows);
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                result.data[i][j] = data[j][i];
            }
        }
        return result;
    }

    friend ostream &operator<<(ostream &os, const Matrix &m) {
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                os << m.data[i][j] << " ";
            }
            os << endl;
        }
        return os;
    }

    friend istream &operator>>(istream &is, Matrix &m) {
        for (int i = 0; i < m.rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                is >> m.data[i][j];
            }
        }
        return is;
    }

    static Matrix getIdentityMatrix(int n){
        Matrix result(n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                result.data[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }
        return result;
    }
};
class Vector : public Matrix{
public:
    Vector(int n) : Matrix(n, 1){}
    Vector(const Matrix &m) : Matrix(4, 1){
        data[0][0] = m.data[0][0];
        data[1][0] = m.data[1][0];
        data[2][0] = m.data[2][0];
        data[3][0] = 0;
    }
    Vector(const Vector &v) : Matrix(v){}
    Vector(double x, double y, double z) : Matrix(4, 1){
        data[0][0] = x;
        data[1][0] = y;
        data[2][0] = z;
        data[3][0] = 0;
    }


    double dot(const Vector &v){
        return (this->getTransposeMatrix() * v).data[0][0];
    }

    Vector cross(const Vector &v){
        Vector result(4);
        result.data[0][0] = data[1][0] * v.data[2][0] - data[2][0] * v.data[1][0];
        result.data[1][0] = data[2][0] * v.data[0][0] - data[0][0] * v.data[2][0];
        result.data[2][0] = data[0][0] * v.data[1][0] - data[1][0] * v.data[0][0];
        result.data[3][0] = 0;
        return result;
    }

    void normalize(){
        double length = sqrt(dot(*this));
        for(int i = 0; i < 3; i++){
            data[i][0] /= length;
        }
    }

    static Vector getVectorFromPoints(const Matrix &p1, const Matrix &p2){
        Vector result(4);
        for(int i = 0; i < 4; i++){
            result.data[i][0] = p2.data[i][0] - p1.data[i][0];
        }
        return result;
    }
};
#endif //MATVEC_H