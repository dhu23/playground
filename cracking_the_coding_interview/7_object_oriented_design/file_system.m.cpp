// explain the data structure and algorithms that you would use to design an
// in-memory file system. Illustrate with an example in code where possible. 

// file attribute:
// file content, timestamp, privilege, owner, name

// directory attribute:
// timestamp, privilege, owner, name

// in a file system, you can do
// mv, cp, cd, rm/rmdir, mkdir, ls, pwd

#include <string>
#include <chrono>
#include <vector>

using namespace std;

// Node could be designed as a base class which is later subclassed into 
// File and Directory. However that would require dynamic cast and is ugly
// use an indicator instead

enum class NodeType { DIRECTORY_NODE = 0, FILE_NODE = 1 };
struct Node
{
    Node(const string& name, chrono::system_clock::time_point& createTime, NodeType type):
        name(name), 
        createTime(createTime), 
        modifiedTime(createTime),
        type(type),
        parent(nullptr), 
        children()
    {
    }

    string name; // the unique identifier
    chrono::system_clock::time_point createTime;
    chrono::system_clock::time_point modifiedTime;
    // omitting owner or permission attributes 

    NodeType type;
    Node* parent;
    vector<Node*> children;
};

// Idea:
// I think this can be done more efficiently by using a modified trie. 
// meaning each root node contains a trie including both its subdirectories
// and contained files. Each subdirectory itself starts another individual 
// trie. The benefit is faster "tab"

struct FileSystem
{
    FileSystem(chrono::system_clock::time_point& createTime):
        root(new Node("/", createTime, NodeType::DIRECTORY_NODE)),
        currentDir(root)
    {
    }

    // not the best names but it mimics a Unix/Linux system
    string pwd() const;

    Node& getFile(const string& relativePath);
    Node& createFile(
            const string& relativePath, 
            chrono::system_clock::time_point& createTime);

    vector<string> ls() const;
    vector<string> ls(const string& relativePath) const;

    void mkdir(const string& relativePath);

    void cp(const string& relativeSourcePath);
    void cp(const string& relativeSourcePath, const string& relativeTargetPath);

    void mv(const string& relativeSourcePath);
    void mv(const string& relativeSourcePath, const string& relativeTargetPath);

    void cd(const string& relativePath);
    void rm(const string& relativePath);
    
    Node* root;
    Node* currentDir;
};

int main(int argc, char* argv[])
{
    return 0;
}
