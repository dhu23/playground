// a monochrome screen is stored as a single array of bytes, allowing eight
// consecutive pixels to be stored in one byte. 

// The screen has width w where w is divisible by 8 that is no byte will
// be split across rows. 
// The height of the screen of course can be derived from the length of the 
// array and the width . Implement a funcion that draws a horizontal line
// from (x1, y) to (x2, y).


// suggested signature:
// drawLine(byte[] screen, int width, int x1, int x2, int y)

// w/8 * h = len(screen). 1 = point 1, 2 = point 2
// ........ ........ ........ ........ ........
// ........ ........ ........ ........ ........
// ........ ........ ........ ........ ........
// ........ ........ ........ ........ ........
// ........ 1....... ........ .....2.. ........
// ........ ........ ........ ........ ........
// ........ ........ ........ ........ ........
// ........ ........ ........ ........ ........

#include <vector>
#include <iostream>
#include <bitset>
#include <stdexcept>

struct Screen
{
    Screen(): numWidth(verifyWidth(width)), pixels(numWidth*height, 0) {}
    Screen(size_t w, size_t h): 
        width(w), height(h), numWidth(verifyWidth(width)), 
        pixels(numWidth*h, 0) {}
    const size_t width = 64;
    const size_t height = 5;
    const size_t numWidth;
    std::vector<int> pixels; 

    std::ostream& display(std::ostream& os) const
    {
        os 
            << "width=" << width << ",height=" << height
            << ",num of int per row=" << numWidth << std::endl;
        for (size_t h = 0; h < height; ++h)
        {
            for (size_t w = 0; w < numWidth; ++w)
            {
                // std::cout << h*width+w << std::endl;
                os << std::bitset<sizeof(int)*8>(pixels.at(h*numWidth+w));
            }
            os << std::endl;
        }
        return os;
    }

    static size_t verifyWidth(size_t width)
    {
        if (width%(sizeof(int)*8) != 0)
        {
            throw std::runtime_error("width is not a multiplier of 8");
        }
        return width/8/sizeof(int);
    }

    void verifyX(size_t x) const
    {
        if (x >= width)
        {
            throw std::runtime_error("x position out of boundary");
        }
    }
    void verifyY(size_t y) const
    {
        if (y >= height)
        {
            throw std::runtime_error("y position out of boundary");
        }
    }

    // use convention from STL, inclusive x1 and exclusive x2
    void drawHorizontalLine(size_t x1, size_t x2, size_t y)
    {
        size_t intBits = 8*sizeof(int);

        this->verifyX(x1);
        this->verifyX(x2);
        this->verifyY(y);

        size_t p1 = x1/intBits;
        size_t b1 = x1%intBits;
        size_t p2 = (x2-1)/intBits;
        size_t b2 = (x2-1)%intBits;

        if (p1 == p2)
        {
            
            int& n = pixels.at(p1+y*numWidth);
            // mark bits at position 31-b1 to 31-b2(inclusive) as 1
            int numOfOnes = b2-b1+1;
            int mask = (1 << numOfOnes) - 1;
            std::cout << std::bitset<32>(mask) << std::endl;
            mask <<= (intBits-1-b2);
            std::cout << std::bitset<32>(mask) << std::endl;
            n |= mask;
        }
        else
        {
            int& start = pixels.at(p1+y*numWidth);
            // mark bis at position 31-b1 to 0 as 1
            int mask = (1 << intBits-b1) - 1;
            start |= mask;

            int& end = pixels.at(p2+y*numWidth);
            // mark bits from the left to 31-b2 as 1
            mask = (1 << intBits-b2-1) - 1;
            end |= (~mask);

            // set anything in between as 1
            for (size_t i = p1+1; i < p2; ++i)
            {
                pixels[i+y*numWidth] |= -1;
            }
        }
    }
};


int main(int argc, char* argv[])
{
    Screen s1;
    s1.display(std::cout);
    s1.drawHorizontalLine(10, 25, 1);
    s1.display(std::cout);
    s1.drawHorizontalLine(10, 60, 2);
    s1.display(std::cout);
    Screen s2(128, 8);
    s2.drawHorizontalLine(10, 110, 2);
    s2.display(std::cout);
    return 0;
}
