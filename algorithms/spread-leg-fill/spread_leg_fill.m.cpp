#define BOOST_TEST_MODULE multifills
#include <boost/test/unit_test.hpp>

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

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

struct Qty
{
    unsigned int cumulative;
    unsigned int last;
};

std::ostream& operator<<(std::ostream& os, const Qty& q)
{
    os << "Qty[cumulative=" << q.cumulative << ",last=" << q.last << "]";
    return os;
}

struct QtyCompare
{
    bool operator()(const Qty& lhs, const Qty& rhs) const 
    {
        return lhs.cumulative < rhs.cumulative;
    }
};

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
        seq_.emplace(t.cumulative, t);

        // check if it should be appended to the cum sequence
        if (t.cumulative == seqVal + t.last) { cumSeq_.push_back(t.cumulative); }
        return true;
    }

    // collapsing until target is accumulated
    bool collapse(unsigned int target) 
    {
        if (target <= processed_) { return false; }
        auto found = std::find(cumSeq_.begin(), cumSeq_.end(), target);
        if (found == cumSeq_.end()) { return false; }

        auto start = std::upper_bound(cumSeq_.begin(), cumSeq_.end(), processed_);
        merged_.emplace_back();
        T& ret = merged_.back();
        while (start <= found)
        {
            auto bit = seq_.begin(); // begin iterator
            ret.merge(bit->second);
            seq_.erase(bit);
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

// just tags
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
/*
class MultiFillCheck
{
private:
    struct MultiFill
    {
        SpreadFill sFill;
        std::unordered_map<std::string, LegFill> lFills;
    };

    std::string spreadSym_;
    QtySequencer<SpreadFill> spreads_;
    // mapping leg symbols to leg fill sequence structure
    std::unordered_map<std::string, QtySequencer<LegFill>> legs_;
    std::vector<MultiFill> order_;

public:
    MultiFillCheck(
        const std::string& spreadSym,
        const std::unordered_set<std::string>& legSyms):
        spreadSym_(spreadSym),
        spreads_(),
        legs_(),
        order_()
    {
        for (const auto& legSym : legSyms)
        {
            legs_.emplace(legSym, QtySequencer<LegFill>());
        }
    }

    void add(const SpreadFill& sf)
    {
        if (sf.sym != spreadSym_)
        {
            throw std::runtime_error("cannot process spread fill symbol");
        }

        spreads_.addQty(sf);

        while (true)
        {
            auto cumval = spreads_.isHeadInSequence();
            if (cumval <= 0) { return; }

            // try to tether spread and leg fills together
            for (const auto& kv : legs_)
            {
                auto& legSequencer = kv.second;
                if (!legSequencer.isInSequenceUntil(cumval)) { return; }
            }
            
            for (auto& kv : legs_)
            {
                auto& legSequencer = kv.second;
                legSequencer.collapseUntil(cumval);
            }
        }
    }

    void add(const LegFill& lf)
    {
        m_legs.addQty(lf);
    }
};

void testSequence()
{
    QtySequencer<SpreadFill> spreads;
    
    spreads.addQty(SpreadFill{35, 75});
    std::cout << spreads << std::endl << std::endl;

    spreads.addQty(SpreadFill{20, 20});
    std::cout << spreads << std::endl << std::endl;

    spreads.addQty(SpreadFill{20, 40});
    std::cout << spreads << std::endl << std::endl;
}
*/
//  8  10  2  |  12  8  |  30  2  3  leg last
//  8  18 20  |  32 40  |  70 72 75  leg cumulative
//        20  |     20  |        35  spread last 
//        20  |     40  |        75  spread cumulative
/*
void testInOrder()
{
    PartialFillCheck pfc;
    pfc.add(LegFill{8, 8});
    pfc.add(LegFill{10, 18});
    pfc.add(LegFill{2, 20});
    pfc.add(SpreadFill{20, 20});
    pfc.add(LegFill{12, 32});
    pfc.add(LegFill{8, 40});
    pfc.add(SpreadFill{20, 40});
    pfc.add(LegFill{30, 70});
    pfc.add(LegFill{2, 72});
    pfc.add(LegFill{3, 75});
    pfc.add(SpreadFill{35, 75});
} 

void testOutOfOrder()
{
    PartialFillCheck pfc;
    pfc.add(LegFill{8, 40});
    pfc.add(SpreadFill{35, 75});
    pfc.add(LegFill{2, 72});
    pfc.add(LegFill{10, 18});
    pfc.add(LegFill{12, 32});
    pfc.add(LegFill{30, 70});
    pfc.add(LegFill{8, 8});
    pfc.add(SpreadFill{20, 40});
    pfc.add(LegFill{3, 75});
    pfc.add(SpreadFill{20, 20});
    pfc.add(LegFill{2, 20});
}
*/

namespace {

    struct Dummy {};
    using FD = Fill<Dummy>;

    auto makeFD = [](unsigned int cumulative, unsigned int last)
    {
        return FD{cumulative, last, ""};
    };

} // anonymous namespace

BOOST_AUTO_TEST_CASE(test_sequencer)
{
    
    QtySequencer<FD> qs;
    BOOST_TEST(qs.add(makeFD(5, 5)));
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.inSequenceUntil() == 5);

    BOOST_TEST(qs.add(makeFD(15, 10)));
    BOOST_TEST(qs.processed() == 0);
    BOOST_TEST(qs.inSequenceUntil() == 15);

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
}
