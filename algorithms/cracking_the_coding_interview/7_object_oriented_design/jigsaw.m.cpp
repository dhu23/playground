// impolement an N by N jigsaw puzzle. Design the data structures and explain
// an algorithm to solve the puzzle. You can assume that you have a fitwidth
// method which whn passed two puzzle edges, returns true if the two edges
// belong together. 

#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <cassert>
// #include <boost/optional/optional.hpp>

// jigsaw piece side demo
//        1
//      _____
//     |     |
//   0 |     | 2
//     |_____|
//       
//        3

using namespace std;

struct Edge; 
struct Piece; // contains 4 edges
struct JigsawPuzzle; // a collection of pieces

bool fitFrame(const Edge& e);
bool fitWidth(const Edge& e1, const Edge& e2);

enum class Side { FLAT = 0, JIGSAW = 1 };

struct Edge
{
    Side side;
    int pixels; // placeholder for image data
};

// when position is 0, edges means left, top, right, bottom
// when position is 1, edges means top, right, bottom, left
// when position is 2, edges means right, bottom, left, top
// when position is 3, edges means bottom, left, top, right
// (it is a left rotation) 
// side --> 0 left 1 top 2 right 3 bottom
struct Piece
{
    Piece(const vector<Edge>& edges):edges(edges) {}
    // rotation goes from 0 to 3. size is 0 to 3
    const Edge& getEdge(size_t rotation, size_t side) const;

    vector<Edge> edges; 
};

struct JigsawSolver
{
    struct NoSolution : public runtime_error
    {
        NoSolution(const char* msg):runtime_error(msg) {}
    };
    
    bool fit(
            const pair<size_t, size_t>& pInfo1, size_t side1, 
            const pair<size_t, size_t>& pInfo2, size_t side2) const;
    bool fit(const pair<size_t, size_t>& pInfo, size_t side) const;

    void nextSolution()
    {
        if (!isFeasibleSolution())
        {
            // backtracking
            // start from adjusting the current position pos
            if (pos == markers.size()-1)
            {
                throw std::runtime_error("no solution");
            }

            if (solution[pos].second == 3)
            {
                // take this piece off the puzzle and replace with the next 
                // available one
                
                size_t nextAvailable = solution[pos].first;
                while (markers[nextAvailable] == 1 && 
                        nextAvailable < markers.size())
                {
                    ++nextAvailable; 
                }

                if (nextAvailable == markers.size())
                {

                    // TODO this is not yet done
                }

                markers[solution[pos].first] = 0; // take it off
                markers[nextAvailable] = 1; // take the next available
                solution[pos].first = nextAvailable;
                solution[pos].second = 0;
                // pos stays the same
            }
            else
            {
                // rotate the piece to the next position
                solution[pos].second += 1;
            }
        }
        else 
        {
            // this solution is feasible, move to next position
            // find the next available piece and use position 0
            if (pos == markers.size()-1)
            {
                // already at the end of solution
                // all pieces are used. since it was already a feasible
                // solution, we should have already encountered a complete
                // solution. 
                throw std::runtime_error("should have already stopped");
            }
            size_t m = 0;
            for (; m < markers.size(); ++m)
            {
                if (markers[m] == 0)
                {
                    break;
                }
                ++m;
            }
            assert(m < markers.size()); // we found an available piece
            ++pos;
            solution[pos] = make_pair(m, 0);
            markers[m] = 1;
        }
    }

    bool isCompleteSolution() const
    {
        return pos == height*length && isFeasibleSolution();
    }

    JigsawSolver(size_t h, size_t l, const vector<Piece>& jigsawPieces):
        height(h), length(l), pieces(), 
        pieceIndexDefault(h*l), pieceRotationDefault(4), 
        markers(h*l, 0), solution(), pos(0)
    {
        // verify size informatoin
        if (pieces.size() != height*length)
        {
            throw std::runtime_error("dimention not matching");
        }
        // populate pieces and soultion vectors
        pieces = jigsawPieces;
        solution.resize(h*l, make_pair(pieceIndexDefault, pieceRotationDefault));
    }

    bool isFeasibleSolution() const
    {
        size_t x = pos / length;
        size_t y = pos % length;
        // if it at the left of the row and edge 0 is not flat
        if (y == 0 && !fit(solution[pos], 0))
        {
            return false;
        }
        // if it is at the right of the row and edge 2 is not flat
        if (y == length-1 && !fit(solution[pos], 2))
        {
            return false;
        }
        // if it is at the first row, and edge 1 is not flat
        if (x == 0 && !fit(solution[pos], 1))
        {
            return false;
        }
        // if it is at the bottom row and edge 3 is not flat
        if (x == height-1 && !fit(solution[pos], 3))
        {
            return false;
        }
        // if it is in the middle of the row
        if (!fit(solution[pos-1], 2, solution[pos], 0))
        {
            return false;
        }
        if (!fit(solution[pos-length], 3, solution[pos], 1))
        {
            return false;
        }
        return true;
    }

    void solve()
    {
        // set init solution
        solution[0] = make_pair(0, 0); // use rotation 0 of piece 0
        pos = 0; // last piece in solution
        markers[0] = 1; // mark piece 0 used 

        try
        {
            while (!isCompleteSolution())
            {
                nextSolution(); // update itself
            }
        }
        catch(NoSolution& e)
        {
            cout << e.what() << endl;
        }
    }

    // jigsaw game information
    size_t height;
    size_t length;
    vector<Piece> pieces;
    
    size_t pieceIndexDefault;
    size_t pieceRotationDefault;

    // mark pieces that are used in the puzzle
    vector<char> markers;

    vector<pair<size_t, size_t>> solution;
    size_t pos;
};

int main(int argc, char* argv[])
{
    return 0;
}
