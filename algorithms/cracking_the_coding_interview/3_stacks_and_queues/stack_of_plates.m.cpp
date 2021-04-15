// imagein a literal stak of plates, if the stack gests too high
// it might topple. Therefore in real life we would likely start a new stack 
// when the previous stack exceeds some threshold. Implement a data structure
// SetOfStacks that mimics this. SetOfStacks should be composed of several
// stacks and should create a new stack once the previous one exceeds capacity. 
// SetOfStacks.push() and SetOfStacks.pop() should behave identically to a 
// single stack, that is pop() should return the same values as it would if 
// there were only just a single stack. 


// follow up
// implement a function popAt(int index) which performs a pop operation on 
// a speicifc sub stack

// answer: replace Stack<Stack<T>> by vector<Stack<T>>

#include "data_structure.h"
#include <iostream>
using ds::Stack;
using std::cout;
using std::endl;
template<typename T>
class SetOfStacks
{
    public:
        SetOfStacks(size_t capacity):
            _collections(), _capacities(), _capacity(capacity) {}
        void push(const T& v)
        {
            if (this->isEmpty() || _capacities.peek() == _capacity)
            {
                _collections.push(Stack<T>());
                _capacities.push(0);
            }
            _collections.peek().push(v);
            ++_capacities.peek();
        }
        void pop()
        {
            if (this->isEmpty())
            {
                throw std::runtime_error("Cannot pop from empty stack set");
            }
           
            Stack<T>& topStack = _collections.peek(); 
            size_t numElements = _capacities.peek();
            if (topStack.isEmpty() || numElements == 0)
            {
                // for code completeness. in practice this won't 
                // happen due to other guarantees in this class
                throw std::runtime_error("Bad! class invariant broken");
            }
            topStack.pop();
            _capacities.pop();
            _capacities.push(numElements-1);
            if (topStack.isEmpty())
            {
                _collections.pop();
                _capacities.pop();
            }
        }
        const T& peek() const
        {
            if (this->isEmpty())
            {
                throw std::runtime_error("Cannot peek at empty stack set");
            }
            return _collections.peek().peek();    
        }
        bool isEmpty() const 
        {
            return _capacities.isEmpty();
        }
    private:
        Stack<Stack<T>> _collections;
        Stack<size_t> _capacities;
        size_t _capacity;
};

int main(int argc, char* argv[])
{
    SetOfStacks<int> sos(2);

    sos.push(1);
    sos.push(2);
    sos.push(3);
    sos.push(4);
    sos.push(5);
    sos.push(6);
    sos.push(7);
    sos.push(8);
    sos.push(9);

    cout << sos.peek() << endl;
    sos.pop();
    cout << sos.peek() << endl;
    sos.pop();
    cout << sos.peek() << endl;
    sos.pop();
    cout << sos.peek() << endl;
    sos.pop();
    cout << sos.peek() << endl;
    sos.pop();
    cout << sos.peek() << endl;
    sos.pop();
    cout << sos.peek() << endl;
    sos.pop();
    cout << sos.peek() << endl;
    sos.pop();
    cout << sos.peek() << endl;
    sos.pop();

    return 0;
}
