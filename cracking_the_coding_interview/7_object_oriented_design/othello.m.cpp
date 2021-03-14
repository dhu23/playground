// Othello is played as follows: each Othello piece is white on one side and 
// black on the other. When a piece is surrounded by its opponents on both
// the left and the right, or the top and the bottom, it is said to be 
// captured and its color is flipped. On your turn, you must capture at least
// one of your opponent's pieces. The game ends when either user has no more
// valid moves. The win is assigned to the person with the most pieces. 

// implement the object oriented design for Othello

#include <array>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include <boost/optional/optional.hpp>


using namespace std;
using namespace boost;

struct Piece;
struct Board;
struct Game;


enum class Color { BLACK = 'X', WHITE = 'O' };
Color flipColor(Color c)
{
    switch(c)
    {
        case Color::BLACK:
            return Color::WHITE;
        case Color::WHITE:
            return Color::BLACK;
        default:
            throw runtime_error("bad color");
    }
}

char toChar(Color c)
{
    return static_cast<char>(c);
}

struct Piece
{
    Piece(Color c): color(c) {}

    Color color;
    void flip() 
    {
        color = flipColor(color);
    }

    ostream& print(ostream& os) const
    {
        os << toChar(color);
        return os;
    }

    operator char() const
    {
        return toChar(color);
    }
};




struct Board
{
    static const size_t dim = 8;
    struct BadPlacement : public runtime_error
    {
        BadPlacement(const char* msg):runtime_error(msg) {}
    };

    Board():cells()
    {
        fill(cells.begin(), cells.end(), optional<Piece>());
    }

    bool withinBoard(size_t x, size_t y) const
    {
        return x < dim && y < dim;
    }

    void place(Color c, size_t x, size_t y, bool legal=false)
    {
        if (!withinBoard(x, y))
        {
            throw BadPlacement("outside of the board");
        }
        size_t pos = x*dim+y;
        if (cells[pos])
        {
            throw BadPlacement("there is a piece there!!");
        }
        // if there is anything that this placement can flip, it is legal
        // otherwise it is not legal

        legal = (capture(c, x, y, -1, -1) > 0) || legal;
        legal = (capture(c, x, y, 1, 1) > 0) || legal;
        legal = (capture(c, x, y, 1, 1) > 0) || legal;
        legal = (capture(c, x, y, 1, -1) > 0) || legal;
        legal = (capture(c, x, y, 1, 0) > 0) || legal;
        legal = (capture(c, x, y, -1, 0) > 0) || legal;
        legal = (capture(c, x, y, 0, 1) > 0) || legal;
        legal = (capture(c, x, y, 0, -1) > 0) || legal;
        
        if (legal)
        {
            cells[pos] = Piece(c);
        }
    }

    // capture 0 means the move is illegal
    size_t capture(Color c, size_t x, size_t y, size_t xdir, size_t ydir)
    {
        size_t x0 = x+xdir;
        size_t y0 = y+ydir;
        size_t oppositeCount = 0;
        bool canCapture = false;
        while (withinBoard(x0, y0))
        {
            const optional<Piece>& p = cells[x0*dim+y0];
            if (!p) // encounters empty before making a capture
            {
                return 0;
            }
            if (p->color != c)
            {
                ++oppositeCount;
            }
            if (p->color == c && oppositeCount > 0)
            {
                canCapture = true;
                break;
            }
            x0 += xdir;
            y0 += ydir;
        }
        if (!canCapture)
        {
            return 0;
        }

        x0 = x+xdir;
        y0 = y+ydir;
        while (oppositeCount-- > 0)
        {
            optional<Piece>& p = cells[x0*dim+y0];
            p->color = c;
        }
        return oppositeCount;
    }

    array<optional<Piece>, dim*dim> cells;
    size_t blackScore;
    size_t whiteScore;

    ostream& print(ostream& os) const
    {
        os << "Board display:" << endl;
        for (size_t i = 0; i < dim; ++i)
        {
            for (size_t j = 0; j < dim; ++j)
            {
                const optional<Piece>& piece = cells[i*dim+j];
                if (!piece)
                {
                    os << '*'; 
                }
                else
                {
                    os << static_cast<char>(*piece);
                }
                os << ' ';
            }
            os << endl;
        }
        return os;
    }
};


struct Game
{
    Game():b()
    {
        // game starts with 2 pieces for each color
        b.place(Color::BLACK, 3, 3, true);
        b.place(Color::WHITE, 3, 4, true);
        b.place(Color::WHITE, 4, 3, true);
        b.place(Color::BLACK, 4, 4, true);
    }

    bool hasMove(Color color) const;
    size_t score(Color color) const;
    bool place(Color color, size_t x, size_t y)
    {
        b.place(color, x, y, false);
    }

    Board b;
    Color nextMove;
};


int main(int argc, char* argv[])
{
    Game g;
    g.b.print(cout);

    g.place(Color::BLACK, 5, 3);
    g.b.print(cout);

    g.place(Color::WHITE, 3, 2);
    g.b.print(cout);

    cout << g.place(Color::BLACK, 2, 3) << endl;
    g.b.print(cout);

    g.place(Color::WHITE, 5, 4);
    g.b.print(cout);
   
    /*
    Color turn = Color::BLACK;
    while (g.hasMove(color))
    {
        size_t x; 
        size_t y;
        do
        {
            cin >> x; 
            cin >> y;
        }
        while (!g.place(color, x, y));

        if (color == Color::WHITE)
        {
            color = Color::BLACK;
        }
        else
        {
            color = Color::WHITE;
        }
    }

    cout << g.score(Color::Board) << endl;
    cout << g.score(Color::WHITE) << endl;
    */
    return 0;
}
