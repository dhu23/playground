// write a method to print the last K lines of an input file using C++

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

ostream& printLastKLines(ostream& os, const char* fileName, int k)
{
    ifstream f(fileName);
    string line;
    size_t lineCount = 0;
    while (getline(f, line))
    {
        ++lineCount;
    }

    size_t startLine = lineCount > k ? lineCount-k : 0;
    f.clear(); // clear eof flag since end of the file was reached once
    f.seekg(0, ios::beg);
    lineCount = 0;
    while (getline(f, line))
    {
        if (lineCount >= startLine)
        {
            os << line << endl;
        }
        ++lineCount;
    }
    return os;
}

// use circular array to achieve this without reading the file twice
ostream& printLastKLinesOneRead(ostream& os, const char* fileName, size_t k)
{
    vector<string> savedLines(k);
    size_t i = 0; // indicates where the next writes go

    size_t lineCount = 0;
    ifstream f(fileName);
    while (getline(f, savedLines[i]))
    {
        i = (i == k-1 ? 0 : (i+1));
        ++lineCount;
    }
    // get the front
    // from savedLines[start] read min(k, lineCount) lines
    i = lineCount >= k ? i : 0;
    for (size_t c = 0; c < min(k, lineCount); ++c)
    {
        os << savedLines[i] << endl;
        i = (i == k-1 ? 0 : (i+1));
    }
    return os;
}

int main(int argc, char* argv[])
{
    cout << "---------- printing last 100 lines -----------" << endl;
    printLastKLines(cout, "last_k_lines.m.cpp", 100); // print the whole file

    cout << "---------- printing last 5 lines -------------" << endl;
    printLastKLines(cout, "last_k_lines.m.cpp", 5); // print the last 5 lines

    cout << "---------- printing last 100 lines -----------" << endl;
    printLastKLinesOneRead(cout, "last_k_lines.m.cpp", 100); 
    
    cout << "---------- printing last 5 lines -------------" << endl;
    printLastKLinesOneRead(cout, "last_k_lines.m.cpp", 5); 
    return 0;
}
