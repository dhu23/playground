#include <stdio.h>

void printChar1(char* p, int row, int column)
{
    int i = 0;
    for(; i < row; ++i)
    {
        int j = 0;
        for(; j < column; ++j)
        {
            int offset = column * i + j;
            char* pOffset = p+sizeof(char)*offset;
            printf("i=%d,j=%d,pos=%p,data=%c,offset=%d\n", 
                    i, j, pOffset, *pOffset, offset);
        }
    }
}

void printChar2(char p[][3], int row, int column)
{
    int i = 0;
    for(; i < row; ++i)
    {
        int j = 0;
        for(; j < column; ++j)
        {
            printf("i=%d,j=%d,pos=%p,data=%c\n", 
                    i, j, &p[i][j], p[i][j]);
        }
    }
}

void printDouble1(double* p, int row, int column)
{
    int i = 0; 
    for(; i < row; ++i)
    {
        int j = 0;
        for(; j < column; ++j)
        {
            int offset = column * i + j;
            // double* pOffset = p+sizeof(double)*offset;
            double* pOffset = p+offset;
            printf("i=%d,j=%d,pos=%p,data=%f,offset=%d,p[offset]=%f\n",
                    i, j, pOffset, *pOffset, offset, p[offset]);
        }
    }
}

int main(int argc, char* argv[])
{
    char data1[4][3] = {
        {'a', 'b', 'c'}, 
        {'i', 'j', 'k'}, 
        {'x', 'y', 'z'}, 
        {'u', 'v', 'w'}};
    printChar1((char*)data1, 4, 3);
    printChar2(data1, 4, 3);
    
    double data2[3][5] = {
        {1.0, 2.0, 3.0, 4.0, 5.0}, 
        {21.5, 31.5, 41.5, 51.5, 61.5},
        {11.1, 22.2, 33.3, 44.4, 55.5}};

    printDouble1((double*)data2, 3, 5);

    return 0;
}

