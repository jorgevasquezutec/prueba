#include <iostream>

#include "matrix.h"
// #include "tester/tester.h"
// #include "mocker/mocker.cpp"
// #include "tester/tester.cpp"

int main() {
    // Tester::execute();
    // return EXIT_SUCCESS;
    Matrix<int> prueba(2,3);
    Matrix<int> prueba2(2,3);
    prueba.set(0,0,2);
    prueba.set(1,0,1);

    prueba.print();

    prueba2.set(0,0,1000);
    prueba2.set(0,1,1);
    prueba2.set(0,2,100);
    prueba2.set(1,0,10);


    prueba2.print();

    Matrix<int> prueba3=prueba+prueba2;
    prueba3.print();
    prueba3.set(1,0,0);
    prueba3.print();
    // cout<<prueba3(1,0)<<endl;


}

