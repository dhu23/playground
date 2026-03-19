#include "utils.h"

std::ostream& printVec(std::ostream& os, const std::vector<int>& data) {
    if (data.empty()) {
        os << "[]";
    } else {
        os << '[' << data[0];
        for (int i = 1; i < data.size(); ++i) {
            os << ',' << data[i];
        }
        os << ']';
    }
    return os;
}


void Printer::accept(const std::vector<int>& data) {
    printVec(std::cout, data) << std::endl;
}