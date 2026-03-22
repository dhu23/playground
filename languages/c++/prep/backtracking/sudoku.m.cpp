// solving sudoku with backtracking

// it is a 9 by 9 grid, which can represented by using an array of size 81
#include <array>
#include <vector>


// Sudoku position layout
//  0   1   2 |  3   4   5 |  6   7   8 
//  9  10  11 | 12  13  14 | 15  16  17
//  ...
//
// from a position i, 
// column(i) = i%9
// row(i) = i/9

class SudokuState {
    std::array<int, 81> grid_;
    std::vector<int> blank_;
public:
    SudokuState(const std::array<int, 81>& puzzle)
    : grid_(puzzle)
    , blank_() {
        // TODO calculate the blank vector
        for (int i = 0; i < 81; ++i) {
            if (grid_[i] <= 0) {
                blank_.push_back(i);
            }
        }
    }

    
};


void solveSudoku_(SudokuState& state) {

}


void solveSudoku(const std::array<int, 81>& puzzle) {
    SudokuState state(puzzle);
    solveSudoku_(state);
}


int main(int argc, char* argv[]) {
    return 0;
}