// design and impolement a text-based minesweeper game. 
// Minesweeper is the classic single player computer game where an N by N
// grid as B mines(or bombs) hidden across the grid. The remaining cells are
// either blank or have a number behind them. The numbers reflect the number
// of bombs in the surrounding eight cells. The user then uncovers a cell. If
// it is a bomb, the player loses. 
// If it is a number the number is exposed. If it is a blank cell, this cell 
// and all adjacent blank cell(up to and including the surrounding numeric
// cells) are exposed. 

// The player wins when all non-bomb cells are exposed.The player can also
// flag certain places as potential bombs. This doesn'taffect game play, other
// than to block he user from accidentally clicking a cell that is thought to
// have a bomb. 

#include <cassert>
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <boost/optional/optional.hpp>

using namespace std;
using namespace boost;

// these two classes are responsible for board which is constant through game
struct Cell;
struct board; // board mechanism
struct MineField;

struct Mark;
struct Game; // responsible for user marks and game dynamics

template<typename T>
struct Board
{
    Board(size_t h, size_t l):height(h), length(l), grid(h*l, T())
    {
    }
    
    size_t getIndex(size_t x, size_t y) const
    {
        if (!withinBoard(x, y))
        {
            throw runtime_error("bad position");
        }
        return x*length+y;
    }

    bool withinBoard(size_t x, size_t y) const
    {
        return x < height && y < length;
    }

    T& get(size_t x, size_t y)
    {
        assert(withinBoard(x, y));
        return grid[this->getIndex(x, y)];
    }
    const T& get(size_t x, size_t y) const
    {
        assert(withinBoard(x, y));
        return grid[this->getIndex(x, y)];
    }

    size_t height;
    size_t length;
    vector<T> grid;
};

struct Cell
{
    Cell(): isMine(false) {}
    bool isMine;
};


struct BetterCell
{
    BetterCell();

    ostream& display(ostream& os) const;

    optional<size_t> mineCount; // null = mine, otherwise there is a number
    // 
    //        isExposed          T                             F
    // isFlag 
    //    T                  won't happen                     show flag
    //    F                  exposed can't be flagged         show ?
    bool isFlaged; 
    bool isExposed; 
};


struct MineField : public Board<Cell>
{
    MineField(size_t h, size_t l, const vector<pair<size_t, size_t>>& mines):
        Board<Cell>(h, l)
    {
        for (auto& m : mines)
        {
            this->get(m.first, m.second).isMine = true;
        }
    }
    size_t countSurroundingMines(size_t x, size_t y) const
    {
        if (this->get(x, y).isMine)
        {
            throw runtime_error("cannot count mines at mine!");
        }
        size_t count = 0;
        count += this->countMine(x-1, y-1);
        count += this->countMine(x-1, y);
        count += this->countMine(x-1, y+1);
        count += this->countMine(x, y-1);
        count += this->countMine(x, y+1);
        count += this->countMine(x+1, y-1);
        count += this->countMine(x+1, y);
        count += this->countMine(x+1, y+1);

        return count;
    }

    size_t countMine(size_t x, size_t y) const
    {
        if (this->withinBoard(x, y) && this->get(x, y).isMine)
        {
            return 1;
        }
        return 0;
    }
    bool isMine(size_t x, size_t y) const
    {
        assert(withinBoard(x, y));
        return this->get(x, y).isMine;
    }
};

// the algebraic data type would be
// data Mark = Uncovered Int | Flag 
struct Mark
{
    Mark():mineCount(), showAsMine(false) {}

    optional<size_t> mineCount;
    bool showAsMine;

    ostream& display(ostream& os) const
    {
        if (showAsMine)
        {
            os << 'M';
        }
        else
        {
            if (!mineCount)
            {
                // show as a potential mine
                os << 'F';
            }
            else
            {
                if (*mineCount > 0)
                {    
                    os << *mineCount;
                }
                else
                {
                    os << ' ';
                }
            }
        }
        return os;
    }
};


struct Game : public Board<optional<Mark>>
{
    Game(size_t h, size_t l, const vector<pair<size_t, size_t>>& mines):
        Board(h, l), 
        mineField(h, l, mines)
    {
        cout << "in game ctor" << endl;
    }


    ostream& display(ostream& os) const
    {
        os << "displaying game:" << endl;
        for (size_t i = 0; i < this->height; ++i)
        {
            for (size_t j = 0; j < this->length; ++j)
            {
                const optional<Mark>& om = this->get(i, j);
                if (!om)
                {
                    os << '?';
                }
                else
                {
                    om->display(os);
                }
                os << ' ';
            }
            os << endl;
        }
    }

    void flagMine(size_t x, size_t y);
    void flagUnknown(size_t x, size_t y);
    void clearMark(size_t x, size_t y);

    
    void addZeroCell(vector<pair<size_t, size_t>>& zeros, size_t x, size_t y)
    {
        if (mineField.withinBoard(x, y))
        {
            if (!mineField.get(x, y).isMine)
            {
                if (this->get(x, y) && this->get(x, y)->mineCount)
                {
                    return;
                }
                zeros.push_back(make_pair(x, y));
            }
        }
    }

    bool uncover(size_t x, size_t y)
    {
        if (mineField.get(x, y).isMine)
        {
            return false; // uncover failed. HIT a bomb    
        }
        
        // check the current state of the marks
        // if it is already uncovered, do nothing
        optional<Mark>& om = this->get(x, y);
        if (!om) // hasn't been flaged yet
        {
            size_t mineCount = mineField.countSurroundingMines(x, y);
            if (mineCount == 0)
            {
                // open all connected 0s, aka mark 0 on each of them
                vector<pair<size_t, size_t>> toOpen(1, make_pair(x, y));
                while (!toOpen.empty())
                {
                    pair<size_t, size_t> p = toOpen.back();
                    
                    mineCount = mineField.countSurroundingMines(p.first, p.second);

                    this->get(p.first, p.second) = Mark();
                    this->get(p.first, p.second)->mineCount = mineCount;
                    toOpen.pop_back();
                    if (mineCount != 0)
                    {
                        continue;
                    }

                    // add all surrounding to the process list
                    addZeroCell(toOpen, p.first-1, p.second-1);
                    addZeroCell(toOpen, p.first-1, p.second);
                    addZeroCell(toOpen, p.first-1, p.second+1);
                    addZeroCell(toOpen, p.first, p.second-1);
                    addZeroCell(toOpen, p.first, p.second+1);
                    addZeroCell(toOpen, p.first+1, p.second-1);
                    addZeroCell(toOpen, p.first+1, p.second);
                    addZeroCell(toOpen, p.first+1, p.second+1);
                }
            }
            else
            {
                om = Mark();
                om->mineCount = mineCount;
            }
        }
        else
        {
            if (!om->mineCount) // no mine count. flagged as mine
            {
                // do nothing. (as required by the rule)
                // this might be a potential mine. 
            }
            else
            {
                // already uncovered safely. do NOTHING.
            }
        }
        return true;
    }

    MineField mineField;
};

int main(int argc, char* argv[])
{
    Game g(7, 7, {make_pair(1, 2), make_pair(3, 2), make_pair(6, 4)});   

    g.display(cout);

    /*
    g.uncover(0, 1);
    g.uncover(1, 1);
    g.uncover(2, 1);
    g.uncover(3, 1);
    g.uncover(4, 1); 
    g.display(cout);
    */
    g.uncover(0, 0);
    g.uncover(1, 6);
    g.display(cout);

    return 0;
}
