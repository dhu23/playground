#include <map>

struct FillQty
{
    int last;
    int cumulative;
};

class SortedFills
{

};

class PartialLegFill
{
private:
    std::map<int, int> _partials;
    std::map<int, int> _legs;
public:
    void addLeg(int last, int cumulative);
    void addPartial(int last, int cumulative);

};

void PartialLegFill::addPartial(int last, int cumulative)
{
}
