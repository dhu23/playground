#define BOOST_TEST_MODULE multifills
#include <boost/test/unit_test.hpp>

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

// legs       | spread
// qty cum    | cum/qty
//   5   5    |
//  10  15    |
//   3  18    |
// ------------ 18/18
//  10  28    |
//  30  58    |
//   2  60    |
// ------------ 42/60

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<K, V>& p)
{
    os << "[first=" << p.first << ",second=" << p.second << "]";
    return os;
}

template<typename K, typename V, typename Comp>
std::ostream& operator<<(std::ostream& os, const std::map<K, V, Comp>& m)
{
    os << "[";
    size_t i = 0;
    for (const auto& kv : m)
    {
        if (i++ > 0) { os << ","; }
        os << kv;
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    size_t i = 0;
    for (const auto& x : v)
    {
        if (i++ > 0) { os << ","; }
        os << x;
    }
    os << "]";
    return os;
}

template<typename T>
class QtySequencer
{
private:
    unsigned int processed_;
    std::vector<unsigned int> cumSeq_;
    std::map<unsigned int, T> seq_;
    std::vector<T> merged_;
public:
    QtySequencer(): processed_(0), cumSeq_(), seq_(), merged_() {}

    const std::vector<T>& merged() const { return merged_; }
    unsigned int processed() const { return processed_; }
    unsigned int inSequenceUntil() const 
    { 
        return cumSeq_.empty() ? 0 : cumSeq_.back();
    }
    // unsigned int isInSequenceUntil(unsigned int target) const
    // {
    //     return target <= this->inSequenceUntil();
    // }
    bool collapsable(unsigned int target) const
    {
        if (target <= processed_) { return false; }
        auto found = std::find(cumSeq_.begin(), cumSeq_.end(), target);
        return found != cumSeq_.end();
    }

    // example: 
    // stared values are in the map
    //    8    8 (collapsed)
    //   11    3 (collapsed) 
    //   -------------------------------- processed = 11
    //  *16    5 
    //  *17    1
    //   ----------------------------- cumSeq = 8, 11, 16, 17
    //   21    4 (not seen)
    //  *27    6
    //   30    3 (not seen)
    //  *40   10
    bool add(const T& t) 
    {
        auto seqVal = this->inSequenceUntil();
        // cannot show up before the last cumqty that is already in sequence
        // this certain guarantees that it won't show less than processed val
        if (t.cumulative <= seqVal) { return false; }

        // this cumqty is already in the map, this violates uniqueness
        if (seq_.find(t.cumulative) != seq_.end()) { return false; }

        // this object would be added to the map. needs to make sure 
        // data structure is still coherent
        auto upbound = seq_.upper_bound(t.cumulative);
        if (upbound == seq_.begin()) 
        {
            // this new object should be at the front. 
            // this includes inserting into an empty map
            if (processed_ + t.last > t.cumulative) { return false; }
        }
        else
        {
            // this object would have something in front of it. check
            auto prev = std::prev(upbound);
            if (prev->first + t.last > t.cumulative) { return false; }

            // if it also has something behind it. check it too
            if (upbound != seq_.end() &&
                t.cumulative + upbound->second.last > upbound->first)
            {
                return false;
            }
        }
        auto ret = seq_.emplace(t.cumulative, t);

        // now see if we can update cumseq because the new node can be 
        // potentially added to the list, so are the objects after it
        auto it = ret.first;
        while (it != seq_.end() && it->first == seqVal + it->second.last)
        {
            cumSeq_.push_back(it->first);
            seqVal = it->first;
            ++it;
        }
        return true;
    }

    // collapsing until target is accumulated
    bool collapse(unsigned int target) 
    {
        if (!this->collapsable(target)) { return false; }

        merged_.emplace_back();
        T& ret = merged_.back();
        auto bit = seq_.begin();
        while (bit != seq_.end() && bit->first <= target) 
        {
            ret.merge(bit->second);
            bit = seq_.erase(bit);
        }
        return true;
    }

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "QtySequencer[processed=" << processed_
            << ",cumSeq=" << cumSeq_ << ",seq=" << seq_
            << ",merged=" << merged_ << ']';
        return os;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const QtySequencer<T>& qs)
{
    return qs.print(os);
}

// the following are for testing purposes so that
// we don't have create sophisticated concrete types
struct Spread {};
struct Leg {};

template<typename T>
struct Fill
{
    // double avgPx;
    unsigned int cumulative;
    unsigned int last;
    std::string sym;

    void merge(const Fill<T>& other)
    {
        if (sym != other.sym)
        {
            throw std::runtime_error("cannot merge different symbols");
        }
        // avgPx = other.cumulative > cumulative ? other.avgPx : avgPx;
        last += other.last;
        cumulative = std::max(other.cumulative, cumulative);
        if (sym.empty()) { sym = other.sym; }
    }
};

using SpreadFill = Fill<Spread>;
using LegFill = Fill<Leg>;

std::ostream& operator<<(std::ostream& os, const SpreadFill& f)
{
    os << "SpreadFill[last=" << f.last << ",cumulative=" << f.cumulative << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const LegFill& f)
{
    os << "LegFill[last=" << f.last << ",cumulative=" << f.cumulative << "]";
    return os;
}


namespace {

struct Dummy {};
using FD = Fill<Dummy>;

auto makeFD = [](unsigned int cumulative, unsigned int last)
{
    return FD{cumulative, last, std::string()};
};

std::ostream& operator<<(std::ostream& os, const FD& obj)
{
    os << "DummyFill[last:" << obj.last << ",cum:" << obj.cumulative << ",sym:" << obj.sym << "]";
    return os;
}

} // anonymous namespace

BOOST_AUTO_TEST_CASE(test_sequencer_add)
{
    QtySequencer<FD> qs;
    BOOST_TEST(qs.add(makeFD(5, 5)));
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.inSequenceUntil() == 5);

    BOOST_TEST(qs.add(makeFD(15, 10)));
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.inSequenceUntil() == 15);
    BOOST_TEST(qs.collapsable(5));
    BOOST_TEST(qs.collapsable(15));
    BOOST_TEST(!qs.collapsable(12));

    BOOST_TEST(qs.add(makeFD(18, 3)));
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.inSequenceUntil() == 18);

    BOOST_TEST(qs.add(makeFD(28, 10)));
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.inSequenceUntil() == 28);

    BOOST_TEST(qs.add(makeFD(58, 30)));
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.inSequenceUntil() == 58);

    BOOST_TEST(qs.add(makeFD(60, 2)));
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.inSequenceUntil() == 60);
    BOOST_TEST(qs.collapsable(5));
    BOOST_TEST(qs.collapsable(15));
    BOOST_TEST(!qs.collapsable(12));

    // std::cout << qs << std::endl;
}

BOOST_AUTO_TEST_CASE(test_sequencer_collapse)
{
    QtySequencer<FD> qs;
    BOOST_TEST(qs.add(makeFD(5, 5)));
    BOOST_TEST(!qs.add(makeFD(5, 5))); // duplicate
    BOOST_TEST(qs.add(makeFD(18, 3)));
    BOOST_TEST(!qs.add(makeFD(16, 10))); // 16+3 > 18. incoherent
    BOOST_TEST(qs.add(makeFD(58, 30)));

    BOOST_TEST(qs.inSequenceUntil() == 5);
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.merged().size() == 0);

    BOOST_TEST(qs.collapsable(5));
    BOOST_TEST(!qs.collapsable(15));
    BOOST_TEST(!qs.collapsable(18));

    BOOST_TEST(qs.add(makeFD(15, 10))); // connecting 5 and 18
    BOOST_TEST(qs.collapsable(18)); 
    BOOST_TEST(qs.inSequenceUntil() == 18);

    BOOST_TEST(qs.collapse(qs.inSequenceUntil())); 

    BOOST_TEST(qs.add(makeFD(28, 10)));
    BOOST_TEST(qs.inSequenceUntil() == 58);
    BOOST_TEST(qs.add(makeFD(60, 2)));
    BOOST_TEST(qs.inSequenceUntil() == 60);
    
    BOOST_TEST(qs.merged().size() == 1);

    auto& firstMerge = qs.merged().back();
    BOOST_TEST(firstMerge.last == 18);
    BOOST_TEST(firstMerge.cumulative == 18);
    BOOST_TEST(firstMerge.sym.empty());

    BOOST_TEST(qs.collapse(qs.inSequenceUntil()));

    auto& secondMerge = qs.merged().back();
    BOOST_TEST(secondMerge.last == 42);
    BOOST_TEST(secondMerge.cumulative == 60);
    BOOST_TEST(secondMerge.sym.empty());

    // std::cout << qs << std::endl;
}
