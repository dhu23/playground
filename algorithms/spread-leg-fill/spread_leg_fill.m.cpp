#define BOOST_TEST_MODULE multifills
#include <boost/test/unit_test.hpp>

#include <unordered_set>
#include <unordered_map>
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

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m)
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
    unsigned int nextInSequence() const
    {
        auto upbound = std::upper_bound(cumSeq_.begin(), cumSeq_.end(), processed_);
        return upbound != cumSeq_.end() ? *upbound : 0;
    }
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
            processed_ = bit->first;
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

template<typename SpreadFill, typename LegFill>
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

    unsigned int tether() // return number of MultiFill tethered together
    {
        unsigned int count = 0;
        auto target = spreads_.nextInSequence();
        while (target > 0)
        {
            bool legTetherable = true;
            for (const auto& kv : legs_)
            {
                if (!kv.second.collapsable(target)) 
                {
                    legTetherable = false;
                    break;
                }
            }
            if (!legTetherable) { break; }

            order_.emplace_back();
            auto& multiFill = order_.back();
            // collapse the spread fill
            if (!spreads_.collapse(target)) 
            { 
                throw std::runtime_error("spread fatal error"); 
            }
            multiFill.sFill = spreads_.merged().back();

            // collapse all the leg fills
            for (auto& kv : legs_)
            {
                if (!kv.second.collapse(target)) 
                { 
                    throw std::runtime_error("leg fatal error"); 
                }
                multiFill.lFills.emplace(kv.first, kv.second.merged().back());
            }
            ++count;
            target = spreads_.nextInSequence();
        }
        return count;
    }
        
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

    const std::vector<MultiFill>& multiFills() const { return order_; }
    
    unsigned int add(const SpreadFill& sf)
    {
        if (sf.sym != spreadSym_) { throw std::runtime_error("bad spread sym"); }
        if (!spreads_.add(sf)) { throw std::runtime_error("cannot add spread"); }
        
        return this->tether();
    }
   
    unsigned int add(const LegFill& lf)
    {
        auto found = legs_.find(lf.sym);
        if (found == legs_.end()) { throw std::runtime_error("bad leg sym"); }
        if (!found->second.add(lf)) { throw std::runtime_error("cannot add leg"); }

        return this->tether();
    }

    std::ostream& print(std::ostream& os) const
    {
        os 
            << "spreadSym:" << spreadSym_
            << "\nspreads:" << spreads_
            << "\nlegs:" << legs_;
        return os;
    }
};

template<typename S, typename L>
inline std::ostream& operator<<(std::ostream& os, const MultiFillCheck<S, L>& obj)
{
    return obj.print(os);
}

namespace {

template<typename T>
struct Fill
{
    // double avgPx;
    unsigned int cumulative;
    unsigned int last;
    std::string sym;

    void merge(const Fill<T>& other)
    {
        if (!sym.empty() && sym != other.sym)
        {
            throw std::runtime_error("cannot merge different symbols");
        }
        // avgPx = other.cumulative > cumulative ? other.avgPx : avgPx;
        last += other.last;
        cumulative = std::max(other.cumulative, cumulative);
        if (sym.empty()) { sym = other.sym; }
    }
};

struct Dummy {};
using FD = Fill<Dummy>;

FD makeFD(unsigned int cumulative, unsigned int last)
{
    return FD{cumulative, last, ""};
};

std::ostream& operator<<(std::ostream& os, const FD& obj)
{
    os << "DummyFill[last:" << obj.last << ",cum:" << obj.cumulative << ",sym:" << obj.sym << "]";
    return os;
}

struct DummyLeg {};
struct DummySpread {};
using DLegFill = Fill<DummyLeg>;
using DSpreadFill = Fill<DummySpread>;

DSpreadFill makeAB(unsigned int cumulative, unsigned int last)
{
    return DSpreadFill{cumulative, last, "A-B"};
}

DLegFill makeA(unsigned int cumulative, unsigned int last)
{
    return DLegFill{cumulative, last, "A"};
}

DLegFill makeB(unsigned int cumulative, unsigned int last)
{
    return DLegFill{cumulative, last, "B"};
}

std::ostream& operator<<(std::ostream& os, const DSpreadFill& obj)
{
    os << "DSpreadFill[last:" << obj.last << ",cum:" << obj.cumulative << ",sym:" << obj.sym << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const DLegFill& obj)
{
    os << "DLegFill[last:" << obj.last << ",cum:" << obj.cumulative << ",sym:" << obj.sym << "]";
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
    BOOST_TEST(qs.nextInSequence() == 5);

    BOOST_TEST(qs.collapsable(18)); 
    BOOST_TEST(qs.inSequenceUntil() == 18);

    BOOST_TEST(qs.collapse(qs.inSequenceUntil())); // collapse
    BOOST_TEST(qs.processed() == 18);
    BOOST_TEST(qs.nextInSequence() == 0); // 28 is not received

    BOOST_TEST(qs.add(makeFD(28, 10)));
    BOOST_TEST(qs.inSequenceUntil() == 58);
    BOOST_TEST(qs.nextInSequence() == 28);
    BOOST_TEST(qs.add(makeFD(60, 2)));
    BOOST_TEST(qs.inSequenceUntil() == 60);
    
    BOOST_TEST(qs.merged().size() == 1);

    auto& firstMerge = qs.merged().back();
    BOOST_TEST(firstMerge.last == 18);
    BOOST_TEST(firstMerge.cumulative == 18);
    BOOST_TEST(firstMerge.sym.empty());

    BOOST_TEST(qs.collapse(qs.inSequenceUntil())); // collapse

    auto& secondMerge = qs.merged().back();
    BOOST_TEST(secondMerge.last == 42);
    BOOST_TEST(secondMerge.cumulative == 60);
    BOOST_TEST(secondMerge.sym.empty());

    // std::cout << qs << std::endl;
}


BOOST_AUTO_TEST_CASE(test_multi_fill)
{
    MultiFillCheck<DSpreadFill, DLegFill> mfc(
        "A-B", std::unordered_set<std::string>{"A", "B"});

    BOOST_TEST(mfc.add(makeAB(60, 42)) == 0);
    BOOST_TEST(mfc.add(makeA(28, 10)) == 0);
    BOOST_TEST(mfc.add(makeA(5, 5)) == 0);
    BOOST_TEST(mfc.add(makeB(5, 5)) == 0);
    BOOST_TEST(mfc.add(makeB(15, 10)) == 0);
    BOOST_TEST(mfc.add(makeB(18, 3)) == 0);
    BOOST_TEST(mfc.add(makeA(15, 10)) == 0);

    BOOST_TEST(mfc.add(makeA(18, 3)) == 0);
    BOOST_TEST(mfc.add(makeAB(18, 18)) == 1); // produced a multifill

    auto& firstMFill = mfc.multiFills().back();
    BOOST_TEST(firstMFill.sFill.cumulative == 18);
    BOOST_TEST(firstMFill.sFill.last == 18);

    auto firstA = firstMFill.lFills.find("A");
    auto hasFirstA = firstA != firstMFill.lFills.end();
    BOOST_TEST(hasFirstA);
    BOOST_TEST(firstA->second.cumulative == 18);
    BOOST_TEST(firstA->second.last == 18);

    auto firstB = firstMFill.lFills.find("B");
    auto hasFirstB = firstB != firstMFill.lFills.end();
    BOOST_TEST(hasFirstB);
    BOOST_TEST(firstB->second.cumulative == 18);
    BOOST_TEST(firstB->second.last == 18);

    BOOST_TEST(mfc.add(makeB(28, 10)) == 0);
    BOOST_TEST(mfc.add(makeA(60, 2)) == 0);
    BOOST_TEST(mfc.add(makeA(58, 30)) == 0); 
    BOOST_TEST(mfc.add(makeB(58, 30)) == 0);
    BOOST_TEST(mfc.add(makeB(60, 2)) == 1); // produced a multifill

    auto& secondMFill = mfc.multiFills().back();
    BOOST_TEST(secondMFill.sFill.cumulative == 60);
    BOOST_TEST(secondMFill.sFill.last == 42);

    auto secondA = secondMFill.lFills.find("A");
    auto hasSecondA = secondA != secondMFill.lFills.end();
    BOOST_TEST(hasSecondA);
    BOOST_TEST(secondA->second.cumulative == 60);
    BOOST_TEST(secondA->second.last == 42);

    auto secondB = secondMFill.lFills.find("B");
    auto hasSecondB = secondB != secondMFill.lFills.end();
    BOOST_TEST(hasSecondB);
    BOOST_TEST(secondB->second.cumulative == 60);
    BOOST_TEST(secondB->second.last == 42);

    // std::cout << mfc << std::endl;
}
