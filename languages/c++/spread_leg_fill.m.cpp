#include <map>
#include <vector>
#include <iostream>

// qty cum
//   5   5
//  10  15
//   3  18
// ------------ 18/18
//  10  28
//  30  58
//   2  60
// ------------ 42/60


struct Qty
{
    int last;
    int cumulative;
};

std::ostream& operator<<(std::ostream& os, const Qty& q)
{
    os 
        << "Qty[last=" << q.last 
        << ",cumulative=" << q.cumulative 
        << "]";
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
        if (i++ > 0) {os << ",";}
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
        if (i++ > 0) {os << ",";}
        os << x;
    }
    os << "]";
    return os;
}

template<typename T>
class QtySequence
{
public:
    struct TData
    {
        T data;
        bool isSynthetic;
        friend std::ostream& operator<<(std::ostream& os, const TData& d)
        {
            os 
                << "TData[data=" << d.data 
                << ",isSynthetic=" << d.isSynthetic 
                << "]";
            return os;
        }
    };
private:
    std::map<Qty, TData, QtyCompare> m_seq;
    std::vector<T> m_merged;
public:
    int addQty(const T& t)
    {
        this->addQty(Qty{t.last, t.cumulative}, t, false);
        return this->merge();
    }
    void addQty(const Qty& qty, const T& t, bool isSynthetic=false)
    {
        m_seq.emplace(qty, TData{t, isSynthetic});
    }
    // get the front of the sequence, if it is the start of the correct
    // sequence, return the qty, otherwise -1
    
    int front() const
    {
        if (m_seq.empty()) return -1;
        const Qty& q = m_seq.begin()->first;
        if (q.last == q.cumulative) return q.last;
        else return -1;
    }
    // return if the merge happens
    bool mergeOnce()
    {
        if (m_seq.empty()) {return false;}

        auto fstIt = m_seq.begin();
        const Qty& fstQ = fstIt->first;

        if (fstQ.last != fstQ.cumulative) {return false;}

        // now attempt merge with the next node
        auto sndIt = std::next(fstIt);
        if (sndIt == m_seq.end()) {return false;}

        const Qty& sndQ = sndIt->first;

        if (fstQ.cumulative + sndQ.last != sndQ.cumulative) {return false;}

        // merge
        TData& fstTD = fstIt->second;
        TData& sndTD = sndIt->second;

        if (!fstTD.isSynthetic) {m_merged.push_back(fstTD.data);}
        m_merged.push_back(sndTD.data);
        
        T merged;
        merged.last = fstTD.data.last;
        merged.cumulative = fstTD.data.cumulative;

        merged.merge(sndTD.data);

        m_seq.erase(fstIt);
        m_seq.erase(sndIt);

        this->addQty(Qty{merged.last, merged.cumulative}, merged, true);

        return true;
    }

    // merge "till" times. If -1, merge till it can't merge
    int merge(int till=-1) // return how many times it merges
    {
        int mergeCount = 0;
        while ((till >= 0 &&mergeCount < till) || till < 0)
        {
            if (!mergeOnce()) break;
            ++mergeCount;
        }
        return mergeCount;
    }
    
    std::ostream& print(std::ostream& os) const
    {
        os << "QtySequence[m_seq=" << m_seq;
        os << ",m_merged=" << m_merged << "]";
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const QtySequence<T>& qs)
    {
        return qs.print(os);
    }
};


struct Spread {};
struct Leg {};

template<typename T>
struct Fill
{
    // double avgPx;
    int last;
    int cumulative;

    void merge(const Fill<T>& other)
    {
        // avgPx = other.cumulative > cumulative ? other.avgPx : avgPx;
        last += other.last;
        cumulative = std::max(other.cumulative, cumulative);
    }
};

using SpreadFill = Fill<Spread>;
using LegFill = Fill<Leg>;

std::ostream& operator<<(std::ostream& os, const SpreadFill& f)
{
    os 
        << "SpreadFill[last=" << f.last 
        << ",cumulative=" << f.cumulative 
        << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const LegFill& f)
{
    os 
        << "LegFill[last=" << f.last 
        << ",cumulative=" << f.cumulative 
        << "]";
    return os;
}
/*
class PartialFillCheck
{
    QtySequence<SpreadFill> m_spreads;
    QtySequence<LegFill> m_legs;
    int m_lastSpreadProcessed = -1;

public:
    void add(const SpreadFill& sf)
    {
        m_spreads.addQty(sf);
        int frontSpread = m_spreads.front();
        if (frontSpread < 0) return;
        if (m_lastSpreadProcessed > frontSpread) return; // error, shoudn't happen
        if (m_lastSpread ==a

        if (m_lastSpreadProcessed != frontSpread || m_spreads.merge() < 0) return;

        // spread fill is ready to go, check leg fills
        int frontLeg = m_legs.front();
        if (frontLeg > frontSpread) return; // shouldn't happen, ERROR
        
        if (frontLeg == frontSpread) // collected all legs for the spread
        {
        }
    }
    void add(const LegFill& lf)
    {
        m_legs.addQty(lf);
    }
};
*/

void testSequence()
{
    QtySequence<SpreadFill> spreads;
    
    spreads.addQty(SpreadFill{35, 75});
    std::cout << spreads << std::endl << std::endl;

    spreads.addQty(SpreadFill{20, 20});
    std::cout << spreads << std::endl << std::endl;

    spreads.addQty(SpreadFill{20, 40});
    std::cout << spreads << std::endl << std::endl;
}

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

int main(int argc, char* argv[])
{
    testSequence();
    // testInOrder();
    // testOutOfOrder();
    return 0;
}
