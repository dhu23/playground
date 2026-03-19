#ifndef BACKTRACKING_UTILS_H_
#define BACKTRACKING_UTILS_H_

#include <iostream>
#include <vector>

std::ostream& printVec(std::ostream& os, const std::vector<int>& data);

class Visitor {
  public:
    Visitor() {}
    virtual ~Visitor() {}

    virtual void accept(const std::vector<int>& data) = 0;
};

class Printer : public Visitor {
  public:
    Printer() {}
    ~Printer() {}

    void accept(const std::vector<int>& data);
};

#endif