// design and implement a hash table which uses chaining(linked lists)
// to handle collisions

#include <utility>
#include <vector>
#include <cstddef>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iostream>

// string1 -- 5 ==> hashed to position 1213
// string2 -- 5 ==> hashed to position 1213
// string3 -- 4 ==> hashed to position 1234
// string4 -- 4 ==> hashed to position 1213

// at position 1213     (string1, 5) --> (string2, 5) --> (string4, 4) --> null
// at position 1234     (string3, 4) --> null


template<typename Key, typename Value>
struct LinkedListNode
{
    LinkedListNode(const Key& k, const Value& v): k(k), v(v), pNext(nullptr) {}

    Key k;
    Value v;
    LinkedListNode<Key, Value>* pNext;
};

// is responsible for all the memeory. There are actually many more things to
// worry about, for example copy constructor and assignment operators as 
// LinkedList is used as vector element and STL assignment will happen often
template<typename Key, typename Value>
struct LinkedList
{
    LinkedList():pHead(nullptr) {}
    ~LinkedList()
    {
        if (pHead)
        {
            LinkedListNode<Key, Value>* p = pHead;
            while (p)
            {
                LinkedListNode<Key, Value>* tmp = p->pNext;
                delete p;
                p = tmp;
            }
        }
    }

    LinkedListNode<Key, Value>* find(const Key& k) const
    {
        if (!pHead)
        {
            return nullptr;
        }

        LinkedListNode<Key, Value>* p = pHead;
        while (p)
        {
            if (p->k == k)
            {
                return p;
            }
            p = p->pNext;
        }
        return nullptr;
    }

    LinkedListNode<Key, Value>& insert(const Key& k, const Value& v)
    {
        LinkedListNode<Key, Value>* pFound = this->find(k);
        if (pFound)
        {
            // overwrite value
            pFound->v = v;
            return *pFound;
        }
        // not found. insert at head
        LinkedListNode<Key, Value>* pInserted = 
            new LinkedListNode<Key, Value>(k, v);
        pInserted->pNext = pHead;
        pHead = pInserted;

        return *pHead;
    }
    void remove(const Key& k)
    {
        LinkedListNode<Key, Value>* p = pHead;
        if (p->k == k)
        {
            pHead = p->pNext;
            delete p;
            return;
        }
        // if the node is not the first one
        while (p->pNext)
        {
            LinkedListNode<Key, Value>* pN = p->pNext;
            if (pN->k == k)
            {
                // remove the node from the chain and delete memory
                p->pNext = pN->pNext;
                delete pN;
                return;
            }
        }
        // nothing is deleted because the node is not found
    }

    LinkedListNode<Key, Value>* pHead;
};

// a map from type Key to type Value 
template<typename Key, typename Value>
struct HashTable
{
    static const size_t N = 255; // should be more sophisticated
    HashTable():data(N, LinkedList<Key, Value>()) {}
    size_t hash(const Key& k) {return std::hash<Key>()(k) % N;}
    
    Value& find(const Key& k)
    {
        LinkedList<Key, Value>& ll = data[hash(k)];
        LinkedListNode<Key, Value>* pFound = ll.find(k);
        if (!pFound)
        {
            std::stringstream ss;
            ss << k << " not found";
            throw std::runtime_error(ss.str());
        }
        return pFound->v;
    }

    const Value& find(const Key& k) const
    {
        LinkedList<Key, Value>& ll = data[hash(k)];
        LinkedListNode<Key, Value>* pFound = ll.find(k);
        if (!pFound)
        {
            std::stringstream ss;
            ss << k << " not found";
            throw std::runtime_error(ss.str());
        }
        return pFound->v;
    }


    Value& insert(const Key& k, const Value& v)
    {
        LinkedList<Key, Value>& ll = data[hash(k)];
        return ll.insert(k, v).v;
    }
    void remove(Key k)
    {
        LinkedList<Key, Value>& ll = data[hash(k)];
        ll.remove(k);
    }

    std::vector<LinkedList<Key, Value>> data;
};

int main(int argc, char* argv[])
{
    HashTable<std::string, int> ht;
    std::cout << "inserting (string1, 5)" << std::endl;
    ht.insert("string1", 5);
    std::cout << "finding (string1)" << std::endl;
    std::cout << ht.find("string1") << std::endl;

    ht.insert("string1", 6);
    std::cout << ht.find("string1") << std::endl;


    return 0;
}
