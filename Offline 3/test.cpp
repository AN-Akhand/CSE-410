#include <iostream>
#include <cmath>
#include "Matrix.h"

using namespace std;

int main(){
    Matrix test(3, 3, new double*[3]{
        new double[3]{1, 2, 3},
        new double[3]{4, 5, 6},
        new double[3]{7, 8, 9}
    });

    cout << test.determinant() << endl;
}