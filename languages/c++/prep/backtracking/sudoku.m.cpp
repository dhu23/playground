// solving sudoku with backtracking

// it is a 9 by 9 grid, which can represented by using an array of size 81
#include <array>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <vector>
#include <iostream>


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
    std::vector<int> blankToFill_;
    std::vector<std::array<int, 81>> solutions_;
    int indexBeingWorked_;
private:
    std::ostream& printRow(int index, std::ostream& os) const {
        int lastIndexInRow = index + 8;
        if (lastIndexInRow < 81) {
            for (int i = 0; i < 8; ++i) {
                if (i == 3 || i == 6) {
                    os << "| ";
                }
                os << grid_[i + index] << ' ';
            }
            os << grid_[lastIndexInRow];
        }
        return os;
    }

    std::ostream& printDivider(std::ostream& os) const {
        os << "------|-------|------";
        return os;
    }

    bool hasCollision(int position, int value) const {
        if (hasRowCollision(position, value)) {
            return true;
        }
        if (hasColumnCollision(position, value)) {
            return true;
        }
        if (hasBoxCollision(position, value)) {
            return true;
        }
        return false;
    }

    bool hasRowCollision(int position, int value) const {
        int rowIndex = position / 9;
        for (int i = 0; i < 9; ++i) {
            int candidateIndex = rowIndex * 9 + i;
            if (candidateIndex == position) {
                continue;
            }
            if (grid_[candidateIndex] == value) {
                return true;
            }
        }
        return false;
    }

    bool hasColumnCollision(int position, int value) const {
        int columnIndex = position % 9;
        for (int i = 0; i < 9; ++i) {
            int candidateIndex = i * 9 + columnIndex;
            if (candidateIndex == position) {
                continue;
            }
            if (grid_[candidateIndex] == value) {
                return true;
            }
        }
        return false;
    }

    bool hasBoxCollision(int position, int value) const {
        int rowIndex = position / 9;
        int columnIndex = position % 9;

        // get top-left box index
        int boxCornerRowIndex = (rowIndex / 3) * 3;
        int boxCornerColumnIndex = (columnIndex / 3) * 3;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                int candidateIndex = (boxCornerRowIndex + i) * 9 + boxCornerColumnIndex + j;
                if (candidateIndex == position) {
                    continue;
                }
                if (grid_[candidateIndex] == value) {
                    return false;
                }
            }
        }
        return false;
    }

public:
    SudokuState(const std::array<int, 81>& puzzle)
    : grid_(puzzle)
    , blankToFill_()
    , solutions_() {
        // TODO calculate the blank vector
        for (int i = 0; i < 81; ++i) {
            if (grid_[i] <= 0) {
                blankToFill_.push_back(i);
            }
        }
        if (blankToFill_.empty()) {
            throw std::runtime_error("the puzzle has no blanks to fill");
        }
        indexBeingWorked_ = blankToFill_.front();
    }

    std::optional<int> getNextCellPosition() const {
        if (indexBeingWorked_ > 0) {
            return std::optional<int>(indexBeingWorked_);
        }
        return std::optional<int>();
    }

    std::vector<int> getPossibleChoices(int position) const {
        std::vector<int> choices{};
        for (int i = 1; i <= 9; ++i) {
            if (hasCollision(position, i)) {
                continue;
            }
            choices.push_back(i);
        }
        return choices;
    }

    void fill(int position, int value) {
        grid_[position] = value;
    }

    // print every row like this 
    // 1 2 3 | 4 5 6 | 7 8 9 
    // print every divider like this 
    // ------|-------|------    (- x 6, |, - x 7 |, - x 6)
    std::ostream& print(std::ostream& os) const {
        int index = 0;
        while (index + 8 < 81) {
            if (index == 27 || index == 54) {
                printDivider(os) << std::endl;
            }
            printRow(index, os) << std::endl;
            index += 9;
        }
        return os;
    }
};

std::ostream& operator<<(std::ostream& os, const SudokuState& state) {
    return state.print(os);
}


void solveSudoku_(SudokuState& state) {
    std::optional<int> nextPosition = state.getNextCellPosition();
    if (!nextPosition) {
        std::cout << state << std::endl;
    } else {
        for (int choice : state.getPossibleChoices(nextPosition.value())) {
            state.fill(nextPosition.value(), choice);
            solveSudoku_(state);
        }
    }
}


void solveSudoku(const std::array<int, 81>& puzzle) {
    SudokuState state(puzzle);
    solveSudoku_(state);
}


int main(int argc, char* argv[]) {
    std::array puzzle {
        1, 5, 0, 0, 8, 2, 0, 0, 0,
        3, 0, 0, 0, 7, 0, 0, 1, 0, 
        0, 0, 0, 0, 0, 0, 7, 5, 3,
        0, 0, 0, 5, 2, 7, 6, 0, 9, 
        0, 0, 0, 0, 0, 0, 5, 0, 0,
        0, 4, 0, 0, 6, 3, 8, 0, 7,
        4, 0, 0, 0, 0, 8, 0, 0, 0,
        7, 0, 3, 0, 4, 0, 1, 0, 0, 
        0, 0, 8, 6, 0, 0, 3, 0, 0
    };

    std::cout << puzzle << std::endl;
    solveSudoku(puzzle);

    return 0;
}