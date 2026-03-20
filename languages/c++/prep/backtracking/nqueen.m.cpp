// put N queens on the chess board so that none of them attack each other

#include <algorithm>
#include <iostream>
#include <optional>
#include <vector>

struct Position {
    int rank; // from 1 to 8
    char file; // from a to h

    std::ostream& print(std::ostream& os) const {
        os << rank << file;
        return os;
    }

    bool operator<(const Position& other) const {
        if (rank != other.rank) {
            return rank < other.rank;
        }
        return file < other.file;
    }

    static const Position& getFirstPosition() {
        static const Position firstPosition(1, 'a');
        return firstPosition;
    }

    static const Position& getLastPosition() {
        static const Position lastPosition(8, 'h');
        return lastPosition;
    }
};

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    return pos.print(os);
}

bool operator==(const Position& lhs, const Position& rhs) {
    return !(lhs < rhs) && !(rhs < lhs); // there could be better implementation
}

std::optional<Position> getNext(const Position& pos) {
    if (pos == Position::getLastPosition()) {
        return std::optional<Position>();
    } else {
        // get the next one
        if (pos.file == 'h') {
            return std::optional<Position>(Position{pos.rank + 1, 'a'});
        } else {
            return std::optional<Position>(
                Position{pos.rank, static_cast<char>(pos.file + 1)});
        }
    }
}

// test if placed and other would attack each other
bool isAttacking(const Position& placed, const Position& other) {
    if (placed.file == other.file || placed.rank == other.rank) {
        return true;
    }
    int fileDist = static_cast<int>(placed.file - other.file);
    int rankDist = placed.rank - other.rank;
    if (fileDist == rankDist || fileDist + rankDist == 0) {
        return true;
    }
    return false;
}

// the order of attempt is 1a -> 1b -> 1c ... -> 2a -> 2b -> ... -> 8h
// this prevents generating duplicated results
class NQueenState {
    std::vector<Position> placement_;
private:

    std::optional<Position> getNextAvailable() const {
        if (placement_.empty()) {
            return std::optional<Position>(Position::getFirstPosition());
        } else {
            return ::getNext(placement_.back());
        }
    }

public:
    NQueenState(): placement_{} {}
    
    int count() const {
        return this->placement_.size();
    }

    bool isAllowed(const Position& pos) const {
        for (const Position& placed : this->placement_) {
            if (::isAttacking(placed, pos)) {
                return false;
            }
        }
        return true;
    }

    std::vector<Position> permittedPositions() const {
        std::vector<Position> permitted{};

        // start from the one with largest value
        std::optional<Position> nextAvailable = this->getNextAvailable();
        while (nextAvailable) {
            Position next = nextAvailable.value();
            if (this->isAllowed(next)) {
                permitted.push_back(next);
            }
            nextAvailable = ::getNext(next);
        }
        return permitted;
    }

    bool place(const Position& pos) {
        auto found = std::find(placement_.begin(), placement_.end(), pos);
        if (placement_.end() != found) {
            return false;
        }
        this->placement_.push_back(pos);
        return true;
    }

    bool removeLast() {
        if (placement_.empty()) {
            return false;
        }
        placement_.pop_back();
        return true;
    }

    std::ostream& print(std::ostream& os) {
        os << "Placement: "; 
        for (const Position& placed : placement_) {
            os << placed << ' ';
        }
        return os;
    }
};

class NQueenVisitor {
public:
    void accept(NQueenState& state) {
        state.print(std::cout) << std::endl;
    }
};

void runNQueen_(int n, NQueenState& state, NQueenVisitor& visitor) {
    if (state.count() == n) {
        visitor.accept(state);
    } else {
        for (const Position& candidate : state.permittedPositions()) {
            state.place(candidate);
            runNQueen_(n, state, visitor);
            state.removeLast();
        }
    }
}

void runNQueen(int n, NQueenVisitor& visitor) {
    NQueenState state{};
    runNQueen_(n, state, visitor);
}

int main(int argc, char* argv[]) {
    NQueenVisitor printer{};
    runNQueen(8, printer);
    return 0;
}