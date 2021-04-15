#include "data_structure.h"

// how would you design a stack, which in addition to push and pop, has a 
// function min that returns the min element. push, pop and min should all
// operate in O(1) time
using ds::Stack;
using std::endl;
using std::cout;

template<typename T>
class StackMin
{
    public:
        StackMin():_data(), _mins() {}
        void push(const T& v)
        {
            _data.push(v);
            if (_mins.isEmpty())
            {
                _mins.push(v);
            }
            else if (_mins.peek() > v)
            {
                _mins.push(v);
            }
            else
            {
                _mins.push(_mins.peek());
            }
        }
        void pop()
        {
            _data.pop();
            _mins.pop();
        }
        const T& min() const {return _mins.peek();}

    private:
        Stack<T> _data;
        Stack<T> _mins;
};

template<typename T>
void pushAndPrint(StackMin<T>& sm, const T& v)
{
    sm.push(v);
    cout << sm.min() << endl;
}

template<typename T>
void popAndPrint(StackMin<T>& sm)
{
    sm.pop();
    cout << sm.min() << endl;
}

int main(int argc, char* argv[])
{
    // add numbers 7, 5, 9, 3, 4, 1, 6, 8
    // mins        7, 5, 5, 3, 3, 1, 1, 1

    StackMin<int> sm;
    pushAndPrint(sm, 7);
    pushAndPrint(sm, 5);
    pushAndPrint(sm, 9);
    pushAndPrint(sm, 3);
    pushAndPrint(sm, 4);
    pushAndPrint(sm, 1);
    pushAndPrint(sm, 6);
    pushAndPrint(sm, 8);

    popAndPrint(sm);
    popAndPrint(sm);
    popAndPrint(sm);
    popAndPrint(sm);
    popAndPrint(sm);
    popAndPrint(sm);
    popAndPrint(sm);
    popAndPrint(sm);

    return 0;
}
