// write a method that takes a pointer to a Node structure as a parameter and
// returns a complete copy of the passed in data structure. The Node data 
// structure contains two pointers to other Nodes

#include <unordered_map>
#include <stdexcept>
#include <cassert>
#include <iostream>

using std::cout;
using std::endl;
using std::ostream;
using std::unordered_map;
using std::runtime_error;

template<typename T>
struct Node
{
    Node(T t):data(t), left(nullptr), right(nullptr) {}
    T data;
    Node<T>* left;
    Node<T>* right;

    ostream& print(ostream& os) const
    {
        os 
            << "Node(val=" << data << ",this=" << this 
            << ",left=" << left << ",right=" << right; 
        return os;
    }
};

// if it forms a tree that would be easy
// however nodes can be shared. for example:
// Node1 --> (Node2, Node3)
// Node2 --> (Node4, Node5)
// Node3 --> (Node4, Node6)

template<typename T>
void traverse(Node<T>* p, unordered_map<Node<T>*, Node<T>*>& markers)
{
    if (!p) return;

    auto it = markers.find(p);
    if (it == markers.end())
    {
        markers.emplace(p, nullptr);
        traverse(p->left, markers);
        traverse(p->right, markers);
    }
}

template<typename T>
void show(const unordered_map<Node<T>*, Node<T>*>& mapper)
{
    cout << "mapper size=" << mapper.size() << endl;
    for (auto& kv : mapper)
    {
        cout << "key=";
        if (kv.first) kv.first->print(cout);
        else cout << "null";
        cout << "\n  ====> mappedto=";
        if (kv.second) kv.second->print(cout); 
        else cout << "null";
        cout << endl;
    }
}

template<typename T>
Node<T>* copy(Node<T>* pSource)
{
    if (!pSource) return nullptr;

    unordered_map<Node<T>*, Node<T>*> mapper;
    traverse(pSource, mapper);
    //show(mapper);
    for (auto& kv : mapper)
    {
        Node<T>* pOriginal = kv.first;
        Node<T>* pCopy = new Node<T>(pOriginal->data);
        kv.second = pCopy;
    }
    show(mapper);
    // made the mapping from original structure to the copy
    for (auto kv : mapper) // it is okay to leave & out
    {
        Node<T>* pOriginal = kv.first;
        Node<T>* pCopy = kv.second;
 
        // replicate pOriginal->left if not null
        if (pOriginal->left)
        {
            auto itLeft = mapper.find(pOriginal->left);
            assert(itLeft != mapper.end() && itLeft->second);
            pCopy->left = itLeft->second;
        }
        if (pOriginal->right)
        {
            auto itRight = mapper.find(pOriginal->right);
            assert(itRight != mapper.end() && itRight->second);
            pCopy->right = itRight->second;
        }
    }
    auto itRoot = mapper.find(pSource);
    assert(itRoot != mapper.end() && itRoot->second);
    show(mapper);
    return itRoot->second;
}

// a different method that combines the DFS of the structure and building
// edges together.
template<typename T>
Node<T>* copyRecursive(
        Node<T>* curr, unordered_map<Node<T>*, Node<T>*>& nodeMap)
{
    if (!curr) return nullptr;
    auto it = nodeMap.find(curr);
    if (it == nodeMap.end()) // the node has not been copied
    {
        Node<T>* copy = new Node<T>(curr->data);
        // has to mark it in the map before visiting the children. 
        // otherwise if there is a loop formed by having an edge pointing 
        // back from the children to the parent(the copy node), this will 
        // end up with an infinite loop
        nodeMap.emplace(curr, copy);
        copy->left = copyRecursive(curr->left, nodeMap);
        copy->right = copyRecursive(curr->right, nodeMap);
        return copy;
    }
    return it->second;
}


int main(int argc, char* argv[])
{
    Node<int>* n1 = new Node<int>(1);
    Node<int>* n2 = new Node<int>(2);
    Node<int>* n3 = new Node<int>(3);
    Node<int>* n4 = new Node<int>(4);
    Node<int>* n5 = new Node<int>(5);
    Node<int>* n6 = new Node<int>(6);

    n1->left = n2;
    n1->right = n3;

    n2->left = n4;
    n2->right = n5;

    n3->left = n4;
    n3->right = n6;

    Node<int>* copyN1 = copy(n1);

    // alternative method
    cout << "================ Another test ================" << endl;
    Node<int>* anotherN1 = new Node<int>(1);
    Node<int>* anotherN2 = new Node<int>(2);
    Node<int>* anotherN3 = new Node<int>(3);

    anotherN1->left = anotherN2;
    anotherN1->right = anotherN3;

    anotherN2->left = anotherN1;
    anotherN2->right = anotherN3;

    anotherN3->left = anotherN1;
    anotherN3->right = anotherN2;

    unordered_map<Node<int>*, Node<int>*> nodeMap;
    Node<int>* anotherCopyN1 = copyRecursive(anotherN1, nodeMap);

    return 0;
}
