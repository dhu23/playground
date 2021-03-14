// design a data structure for an online book reader system

// collection of books
// readers

// book table 
// book_name|authors|description|content

// reader table
// reader_name|book_name|

#include <string>
#include <set>
#include <map>
#include <vector>
#include <memory>

using namespace std;

struct Book; // tracks library shared by all readers
struct Library;
struct Reader; // owns read lists, history
struct ReaderPool;

struct Rating; // each book has a rating record by users

struct ReadList; // a book list that tracks reading progression
struct Recommendation; // based on reading hitory give out book recommendations

struct BookRating
{
    // map of reader id to rating value 
    std::map<int, int> rates;
};

struct Book
{
    int id;
    string name;
    vector<string> authors;
    string description;
    string text;

    shared_ptr<BookRating> rating;
};

typedef vector<shared_ptr<Book>> BookList;
struct Library
{
    shared_ptr<Book> get(int id) const;
    BookList get(const std::string& author) const;
};

struct ReadList
{
    BookList books;
    vector<int> progressions; // tracks which pages readers are at
};


struct Reader
{
    Reader(const string& name, Library& lib);
    // operates on the library
    void rate(shared_ptr<Book>& pBook, int rating) const
    {

    }
    void read(shared_ptr<Book>& pBook);
    void read(int bookId);
    void finish(int bookId);

    int id;
    Library& lib;
    string name;
    BookList history;
    ReadList currentReads;
};

struct ReaderPool
{
    shared_ptr<Reader> signUp(const string& name, const Library& lib);
};

struct Recommandation
{
    
};

int main(int argc, char* argv[])
{
    return 0;
}
