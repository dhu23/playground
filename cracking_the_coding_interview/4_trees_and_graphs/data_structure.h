#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <string>
#include <iostream>
#include <random>

// graph and its nodes only tracks topology of the structure. 
// it does not try to track actual data
struct BasicNode
{
    BasicNode(size_t id):id(id) {}
    size_t id;
    std::vector<BasicNode*> adjacency;
};


template<typename NodeType>
class Graph
{
    public:
        Graph(size_t numV);
        ~Graph();
        void addEdge(size_t fromId, size_t toId);
        NodeType& getVertex(size_t id);
        const NodeType& getVertex(size_t id) const;

        size_t numOfVertices() const {return _nodes.size();}

    private:
        void verifyId(size_t id) const;
        std::vector<NodeType*> _nodes; // every node in the graph 
};

template<typename NodeType>
Graph<NodeType>::Graph(size_t numV):_nodes(numV, nullptr)
{
    for (size_t i = 0; i < numV; ++i)
    {
        _nodes[i] = new NodeType(i);
    }
}

template<typename NodeType>
Graph<NodeType>::~Graph()
{
    for (NodeType* gn : _nodes)
    {
        if (gn)
        {
            delete gn;
        }
    }
}

template<typename NodeType>
NodeType& Graph<NodeType>::getVertex(size_t id)
{
    this->verifyId(id);
    return *_nodes[id];
}

template<typename NodeType>
const NodeType& Graph<NodeType>::getVertex(size_t id) const
{
    this->verifyId(id);
    return *_nodes[id];
}

template<typename NodeType>
void Graph<NodeType>::verifyId(size_t id) const
{
    if (id >= _nodes.size())
    {
        throw std::runtime_error("id is greater than graph capacity");
    }
    if (!_nodes[id])
    {
        throw std::runtime_error("node is null");
    }
}

template<typename NodeType>
void Graph<NodeType>::addEdge(size_t fromId, size_t toId)
{
    NodeType& fromNode = this->getVertex(fromId);
    NodeType& toNode = this->getVertex(toId);
    fromNode.adjacency.push_back(&toNode);
}


// binary tree node
template<typename T>
struct TreeNode
{
    TreeNode(T t):value(t), size(1), pLeft(nullptr), pRight(nullptr) {}
    T value;
    size_t size; // size of the tree including itself
    TreeNode* pLeft;
    TreeNode* pRight;
};

template<typename T>
TreeNode<T>* insertNode(TreeNode<T>* pRoot, T t)
{
    if (!pRoot)
    {
        pRoot = new TreeNode<T>(t);
    }
    else if (pRoot->value < t)
    {
        size_t rightSize = pRoot->pRight ? pRoot->pRight->size : 0;
        pRoot->pRight = insertNode(pRoot->pRight, t);
        pRoot->size += pRoot->pRight->size - rightSize;
    }
    else if (pRoot->value > t)
    {
        size_t leftSize = pRoot->pLeft ? pRoot->pLeft->size : 0;
        pRoot->pLeft = insertNode(pRoot->pLeft, t);
        pRoot->size += pRoot->pLeft->size - leftSize;
    }
    else // equal case
    {
    }
    return pRoot;
}

template<typename T>
TreeNode<T>* findNode(TreeNode<T>* pRoot, T t)
{
    if (!pRoot)
    {
        return nullptr;
    }
    else if (pRoot->value < t)
    {
        return findNode(pRoot->pRight, t);
    }
    else if (pRoot->value > t)
    {
        return findNode(pRoot->pLeft, t);
    }
    else // equal case
    {
        return pRoot;
    }
}

template<typename T>
std::ostream& inorder(TreeNode<T>* pRoot, std::ostream& os)
{
    if (!pRoot)
    {
        return os;
    }

    inorder(pRoot->pLeft, os);
    os 
        << std::string(" ") << pRoot->value << "(" << pRoot->size << ")"
        << std::string(" "); 
    inorder(pRoot->pRight, os);
    return os;
}

template<typename T, typename Gen>
TreeNode<T>& randomNode(TreeNode<T>& root, Gen& gen)
{
    std::uniform_int_distribution<size_t> dis(0, root.size-1);
    size_t r = dis(gen);
    size_t leftSize = root.pLeft ? root.pLeft->size : 0;
    if (r < leftSize)
    {
        return randomNode(*root.pLeft, gen);
    }
    else if (r == leftSize)
    {
        return root;
    }
    else
    {
        return randomNode(*root.pRight, gen);
    }
}

// binary search tree
template<typename T>
class BinarySearchTree
{
    public:
        BinarySearchTree():_pRoot(nullptr) {}
        ~BinarySearchTree();

        void insert(T t);
        TreeNode<T>* find(T t) const;
        void remove(T t);
        TreeNode<T>& random() const;

        std::ostream& inorder(std::ostream& os) const;

        TreeNode<T>* root() {return _pRoot;}

    private:

        void deleteTree(TreeNode<T>* pRoot);    

        TreeNode<T>* _pRoot;
};

template<typename T>
std::ostream& BinarySearchTree<T>::inorder(std::ostream& os) const
{
    ::inorder(_pRoot, os);
    os << std::endl;
    return os;
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
    deleteTree(_pRoot);
    _pRoot = nullptr;
}

template<typename T>
void BinarySearchTree<T>::deleteTree(TreeNode<T>* pRoot)
{
    if (!pRoot)
    {
        return;
    }
    deleteTree(pRoot->pLeft);
    deleteTree(pRoot->pRight);
    pRoot->pLeft = nullptr;
    pRoot->pRight = nullptr;
    std::cout << "deleting node " << pRoot->value << std::endl;
    delete pRoot;
}

template<typename T>
void BinarySearchTree<T>::insert(T t)
{
    _pRoot = insertNode(_pRoot, t);
}

template<typename T>
TreeNode<T>* BinarySearchTree<T>::find(T t) const
{
    return findNode(_pRoot, t);
}

template<typename T>
void BinarySearchTree<T>::remove(T t)
{
}

template<typename T>
TreeNode<T>& BinarySearchTree<T>::random() const
{
    if (!_pRoot)
    {
        throw std::runtime_error("cannot get random node from empty tree");
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    return randomNode(*_pRoot, gen);
}

