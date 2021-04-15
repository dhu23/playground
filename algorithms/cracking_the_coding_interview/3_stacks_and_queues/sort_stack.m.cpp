// sort a stack such that the smallest items are on the top
// You can use one additional temporary stack but you cannot copy the data
// to other data structures such as array. 

#include "data_structure.h"

using std::cout;
using std::endl;
using ds::Stack;

// use another stack. so by emptying original stack and filling the aux stack
// the min element is found. Keep the min in the aux stack. so every run 
// the current min would be found and left in the aux stack and at the end
// fill the original stack then it should have the right order
template<typename T>
void sort(Stack<T>& s)
{
    if (s.isEmpty())
    {
        return;
    }

    Stack<T> aux;
    
    while (!s.isEmpty())
    {
        // fill aux stack and leave the min inside
        int count = 0;
        T currentMin = s.peek();
        while (!s.isEmpty())
        {
            currentMin = std::min(currentMin, s.peek());
            aux.push(s.peek());
            s.pop();
            ++count;
        }
        // all (count) elements from aux and push (count-1) elements to s
        // except for one currentMin(if there are duplicates).
        // there is an issue though: are two T object that are considered
        // equal identical? In many practical cases, no. 
        int minCount = 0;
        while (count-- > 0) // pop (count) elements,  it may be less than size
        {
            if (aux.peek() != currentMin)
            {
                s.push(aux.peek());
                aux.pop();
            }
            else // when peek == currentMin 
            {
                aux.pop(); // don't put currentMin back in s
                ++minCount;
            }
        }
        // push all the mins we found in this round back to aux
        while (minCount-- > 0)
        {
            aux.push(currentMin);
        }
    }

    // all elements now sorted from small to large(top) in aux
    while (!aux.isEmpty())
    {
        s.push(aux.peek());
        aux.pop();
    }
}


template<typename T>
void sort2(Stack<T>& s)
{
    if (s.isEmpty())
    {
        return;
    }

    Stack<T> aux;

    while (!s.isEmpty())
    {
        T tmp = s.peek();
        s.pop();

        while (!aux.isEmpty() && aux.peek() > tmp)
        {
            // move it to s. Say we moved 3 elements which should be already 
            // sorted to s, then later the outer while will execute another
            // 3 times to move them back to the next unsorted element in s
            s.push(aux.peek());
            aux.pop();
        }
        aux.push(tmp);
    }
    // put back to s
    while (!aux.isEmpty())
    {
        s.push(aux.peek());
        aux.pop();
    }
}


int main(int argc, char* argv[])
{
    // build a stack 5, 6, 1, 4, 2, 9, 7
    Stack<int> s;
    s.push(5);
    s.push(6);
    s.push(1);
    s.push(4);
    s.push(2);
    s.push(9);
    s.push(7);

    sort2(s);
    while (!s.isEmpty())
    {
        cout << s.peek() << endl;
        s.pop();
    }
    // print out 1, 2, 4, 5, 6, 7, 9

    return 0;
}
