#include "data_structure.h"
#include <string>
#include <iostream>
#include <map>

using std::string;

int main(int argc, char* argv[])
{
    Graph<BasicNode> g(7);

    g.addEdge(0, 4);
    g.addEdge(1, 4);
    g.addEdge(2, 0);
    g.addEdge(1, 0);
    g.addEdge(5, 0);
    g.addEdge(5, 1);
    g.addEdge(5, 2);
    
    g.addEdge(3, 6);

    for (size_t i = 0; i < 7; ++i)
    {
        std::cout << "from node=" << i << "----->" << std::endl;
        for (BasicNode* neighbor : g.getVertex(i).adjacency)
        {
            std::cout << neighbor->id << std::endl;
        }
    }

    std::cout << "========== bst test ===========" << std::endl;
    std::vector<int> vs{10, 5, 7, 30, 30, 15, 23, 24};
    BinarySearchTree<int> bst;
    for (int v : vs)
    {
        bst.insert(v);
    }
    std::cout << "root=" << bst.root()->value << std::endl;
    bst.inorder(std::cout);

    std::map<int, size_t> m;
    for (size_t i = 0; i < 30000; ++i)
    {
        int val = bst.random().value;
        if (m.find(val) == m.end())
        {
            m.emplace(val, 0);
        }
        m[val] += 1;
    }

    for (auto& kv : m)
    {
        std::cout << kv.first << "-->" << kv.second << std::endl;
    }

    return 0;
}
