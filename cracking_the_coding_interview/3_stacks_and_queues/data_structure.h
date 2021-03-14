#ifndef INCLUDED_DATA_STRUCTURE_H
#define INCLUDED_DATA_STRUCTURE_H

#include <iostream>
#include <stdexcept>

namespace ds 
{

template<typename T>
struct LinkedListNode
{
    LinkedListNode(const T& v):value(v), pNext(nullptr) {}

    T value;
    LinkedListNode* pNext;

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "value=" << value << ",id=" << this 
            << ",next=" << pNext;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const LinkedListNode<T>& n)
{
    return n.print(os);
}


template<typename T>
class Stack
{
    public:
        Stack():_pHead(nullptr) {}
        ~Stack();

        void pop();
        void push(const T& v);
        const T& peek() const;
        T& peek();
        bool isEmpty() const {return !_pHead;}

    private:
        LinkedListNode<T>* _pHead;
};

template<typename T>
Stack<T>::~Stack()
{
    while (!this->isEmpty())
    {
        this->pop();
    }
}

template<typename T>
void Stack<T>::pop()
{
    if (this->isEmpty())
    {
        throw std::runtime_error("Cannot pop from empty stack");
    }
    LinkedListNode<T>* pTmp = _pHead;
    _pHead = _pHead->pNext;
    delete pTmp;
}

template<typename T>
void Stack<T>::push(const T& v)
{
    LinkedListNode<T>* pCurrent = _pHead;
    _pHead = new LinkedListNode<T>(v);
    _pHead->pNext = pCurrent;
}

template<typename T>
const T& Stack<T>::peek() const
{
    if (this->isEmpty())
    {
        throw std::runtime_error("Cannot peek at empty stack");
    }
    return _pHead->value;
}

template<typename T>
T& Stack<T>::peek()
{
    if (this->isEmpty())
    {
        throw std::runtime_error("Cannot peek at empty stack");
    }
    return _pHead->value;
}

// Queue adds to the tail of the linked list and removes from the head
template<typename T>
class Queue
{
    public:
        Queue():_pTail(nullptr), _pHead(nullptr) {}
        ~Queue();

        void add(const T& v);
        void remove();
        const T& peek() const;
        T& peek();
        bool isEmpty() const {return !_pHead;}

    private:
        LinkedListNode<T>* _pTail;
        LinkedListNode<T>* _pHead;
};

template<typename T>
Queue<T>::~Queue()
{
    while (!this->isEmpty())
    {
        this->remove();
    }
}

template<typename T>
void Queue<T>::remove()
{
    if (this->isEmpty())
    {
        throw std::runtime_error("Cannot remove from empty queue");
    }
    LinkedListNode<T>* pTmp = _pHead;
    _pHead = _pHead->pNext;
    if (!_pHead)
    {
        _pTail = nullptr;
    }
    delete pTmp;
}

template<typename T>
void Queue<T>::add(const T& v)
{
    LinkedListNode<T>* pAdded = new LinkedListNode<T>(v);
    if (this->isEmpty())
    {
        _pHead = pAdded;
    }
    else
    {
        _pTail->pNext = pAdded;
    }
    _pTail = pAdded;
}


template<typename T>
const T& Queue<T>::peek() const
{
    if (this->isEmpty())
    {
        throw std::runtime_error("Cannot peek at empty queue");
    }
    return _pHead->value;
}


template<typename T>
T& Queue<T>::peek()
{
    if (this->isEmpty())
    {
        throw std::runtime_error("Cannot peek at empty queue");
    }
    return _pHead->value;
}

} // end of namespace ds
#endif
