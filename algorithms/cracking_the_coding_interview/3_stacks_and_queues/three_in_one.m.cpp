// use a single array to implement three stacks. In theory you can implement
// any number of stacks by using it

// there are multiple ways of doing it
// 1, by dividing stacks into multiple sections, each representing a stack
// 2, by interleaving stack elements together, 1, 2, 3, 1, 2, 3 etc

#include <vector>
#include <array>
#include <stdexcept>
#include <utility> // for swap
#include <iterator> // for begin and end
#include <iostream>

using std::vector;
using std::array;
using std::begin;
using std::endl;
using std::endl;
using std::cout;

template<typename T>
class ThreeStacks
{
    public:
        // use small init for easier testing
        ThreeStacks():_data(3), _sizes{0, 0, 0} {} 
        
        void push(size_t stackIdx, const T& v)
        {
            this->verifyStackIdx(stackIdx);
            if (_sizes[stackIdx] == _data.size()/3)
            {
                this->expand();
            }

            size_t nextPos = stackIdx*_data.size()/3+_sizes[stackIdx];
            _data[nextPos] = v;
            ++_sizes[stackIdx];
        }
        void pop(size_t stackIdx)
        {
            if (this->isEmpty(stackIdx))
            {
                throw std::runtime_error("Cannot pop from empty stack");
            }

            --_sizes[stackIdx];
        }
        const T& peek(size_t stackIdx) const
        {
            if (this->isEmpty(stackIdx))
            {
                throw std::runtime_error("Cannot peek at empty stack");
            }
            
            size_t topPos = stackIdx*_data.size()/3+_sizes[stackIdx]-1;
            return _data[topPos];
        }
        bool isEmpty(size_t stackIdx) const 
        {
            this->verifyStackIdx(stackIdx);
            return _sizes[stackIdx] == 0;
        }

    private:
        void verifyStackIdx(size_t stackIdx) const
        {
            if (stackIdx >= 3)
            {
                throw std::runtime_error("There are only 3 stacks");
            }
        }
        void expand()
        {
            vector<T> temp(_data.size()*2); // double the capacity
            temp.insert(
                    begin(temp), 
                    begin(_data), 
                    begin(_data)+_sizes[0]);
            temp.insert(
                    begin(temp)+_data.size()*2/3, 
                    begin(_data)+_data.size()/3,
                    begin(_data)+_data.size()/3+_sizes[1]);
            temp.insert(
                    begin(temp)+_data.size()*4/3, 
                    begin(_data)+_data.size()*2/3,
                    begin(_data)+_data.size()*2/3+_sizes[2]);
            std::swap(_data, temp);
        }

        vector<T> _data; // _data size is going to be a multiplier of 3
        array<size_t, 3> _sizes; // stack sizes, _sizes[i] <= _data.size()/3
};


int main(int argc, char* argv[])
{
    ThreeStacks<int> ts;
    ts.push(0, 3);
    ts.push(0, 4);
    ts.push(0, 5);
    ts.push(1, 33);
    ts.push(1, 44);
    ts.push(1, 55);
    ts.push(2, 333);
    ts.push(2, 444);
    ts.push(2, 555);
    
    cout << ts.peek(0) << endl; // 5
    cout << ts.peek(1) << endl; // 55
    cout << ts.peek(2) << endl; // 555

    ts.pop(0);
    ts.pop(1);
    ts.pop(2);

    cout << ts.peek(0) << endl; // 4
    cout << ts.peek(1) << endl; // 44
    cout << ts.peek(2) << endl; // 444

    ts.pop(0);
    ts.pop(1);
    ts.pop(2);
    ts.pop(0);
    ts.pop(1);
    ts.pop(2);

    return 0;
}
