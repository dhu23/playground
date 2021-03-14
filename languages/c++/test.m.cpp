#include "matrix.h"

#include <iostream>

using namespace dhm;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{   
    std::vector<int> elements;
    elements.push_back(1);
    elements.push_back(2);
    elements.push_back(3);
    elements.push_back(4);
    elements.push_back(5);
    elements.push_back(6);

    Matrix<int> m1(2, 3, elements);
    Matrix<int> m2 = m1;

    cout << m2 << endl;

    Matrix<int> v1 = m2.getRow(1); // [1, 2, 3]
    elements.clear();
    elements.push_back(3);
    elements.push_back(2);
    elements.push_back(1);
    elements.push_back(5);
    elements.push_back(-1);
    elements.push_back(0);
    Matrix<int> m3(3, 2, elements);

    cout << m3 << endl;

    cout << m2*m3 << endl;
    cout << m2.getTranspose() << endl;
    cout << m2*5 << endl;

    return 0;
}
