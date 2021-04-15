// write a function in C called my my2DAlloc which allocates a two-dimensional
// array. Minimize the number of calls to malloc and make sure that the memory
// is accessible by the notation arr[i][j]

#include <stdlib.h>
#include <iostream>
using namespace std;

// array[][] is a two-d array syntax. 
// this implies that we will have to store int*
template<typename T>
void** my2DAlloc(int rows, int cols)
{
    // it stores #rows (T*) and #rows by #cols (T) object 
    unsigned int total = rows*sizeof(T*) + rows*cols*sizeof(T);
    void* p = (void*)malloc(total);

    T** pIndex = (T**)p;
    T* pData = (T*)(pIndex+rows);
    for (int i = 0; i < rows; ++i)
    {
        pIndex[i] = pData+i*cols;
    }
    return (void**)pIndex;
}

int main(int argc, char* argv[])
{
    int val = 1;
    int** p = (int**)my2DAlloc<int>(5, 3);
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            p[i][j] = val++;
        }
    }
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (j == 0)
            {
                cout << (uintptr_t)&p[i][j] << endl;
            }
            cout << p[i][j] << endl;
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        cout << (uintptr_t)(*(p+i)) << endl;
    }
    

    return 0;
}
