// implement a queue with two stacks

#include "data_structure.h"


using ds::Stack;
using ds::Queue;

template<typename T>
class QueueViaStacks
{
    public:
        QueueViaStacks() {}
        void add(const T& v)
        {
            _in.push(v);
        }
        void remove()
        {
            if (_out.isEmpty())
            {
                this->emptyInAndFillOut();
            }
            if (this->isEmpty())
            {
                throw std::runtime_error("Cannot remove from empty queue");
            }
            _out.pop();
        }
        const T& peek()
        {
            if (_out.isEmpty())
            {
                this->emptyInAndFillOut();
            }
            if (this->isEmpty())
            {
                throw std::runtime_error("Cannot peek at empty queue");
            }
            return _out.peek();
        }
        bool isEmpty() const {return _in.isEmpty() && _out.isEmpty();}
    private:
        void emptyInAndFillOut()
        {
            if (!_out.isEmpty())
            {
                throw std::runtime_error(
                        "out stack can be filled only when empty");
            }
            while (!_in.isEmpty())
            {
                _out.push(_in.peek());
                _in.pop();
            }
        }

        Stack<T> _in; // add items to here
        Stack<T> _out; // remove items from here
};

template<typename T>
void verifyPush(QueueViaStacks<T>& qvs, Queue<T>& q, const T& v)
{
    qvs.add(v);
    q.add(v);
    if (qvs.peek() != q.peek())
    {
        throw std::runtime_error("qvs.peek() != q.peek() after push");
    }
}

template<typename T>
void verifyPop(QueueViaStacks<T>& qvs, Queue<T>& q)
{
    if (qvs.peek() != q.peek())
    {
        throw std::runtime_error("qvs.peek() != q.peek() before pop");
    }
    qvs.remove();
    q.remove();
}


int main(int argc, char* argv[])
{
    QueueViaStacks<int> qvs;
    Queue<int> q;

    verifyPush(qvs, q, 3);
    verifyPush(qvs, q, 4);
    verifyPop(qvs, q);
    verifyPop(qvs, q);
    verifyPush(qvs, q, 5);
    verifyPush(qvs, q, 6);
    verifyPush(qvs, q, 7);
    verifyPush(qvs, q, 8);
    verifyPop(qvs, q);
    verifyPush(qvs, q, 9);
    verifyPop(qvs, q);

    return 0;
}
