#include <unordered_map>
#include <map>
#include <iostream>

using namespace std;

void remove1(unordered_map<int, std::string>& m)
{
    auto it = m.begin();
    while (it != m.end())
    {
        if (it->second == "2")
        {
            it = m.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void remove2(map<int, std::string>& m)
{
    auto it = m.begin();
    while (it != m.end())
    {
        if (it->second == "2")
        {
            m.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

template<typename Map>
void print(const Map& m)
{
    for (const auto& kv : m)
    {
        cout << kv.first << "--->" << kv.second << endl;
    }
}

struct Key
{
    std::string client;
    std::string inst;
};

struct KeyComparator
{
    bool operator()(const Key& lhs, const Key& rhs) const
    {
        if (lhs.client != rhs.client) return lhs.client < rhs.client;
        if (lhs.inst == "ALL") return rhs.inst != "ALL";
        if (rhs.inst == "ALL") return false;
        return lhs.inst < rhs.inst;
    }
};

void pmatch(const std::map<Key, int, KeyComparator>& m, 
            const Key& key)
{
    if (!m.empty())
    {
        auto it = m.upper_bound(key);
        if (it != m.begin())
        {
            --it;
            if (it->first.client == key.client)
            {
                cout << key.client << '|' << key.inst << "=>" << it->second << endl;
                return;
            }
        }
    }
    cout << "no match: " << key.client << '|' << key.inst << endl;
}

void testPartialMatch()
{
    std::map<Key, int, KeyComparator> m;
    m.emplace(Key{"QB", "ALL"}, 11);
    m.emplace(Key{"Blackrock", "ZN"}, 12);
    m.emplace(Key{"Blackrock", "ALL"}, 13);
    m.emplace(Key{"QB", "ZN"}, 14);
    m.emplace(Key{"QB", "UB"}, 15);

    pmatch(m, Key{"QB1", "ZN"}); // nothing
    pmatch(m, Key{"QB", "ES"}); // 11 
    pmatch(m, Key{"Blackrock", "ES"}); // 13
    pmatch(m, Key{"QB", "ZN"}); // 14
}

int main(int argc, char* argv[])
{
    /*
    unordered_map<int, std::string> um{{1, "1"}, {2, "2"}, {3, "3"}};
    print(um);
    remove1(um);
    print(um);

    map<int, std::string> m{{1, "1"}, {2, "2"}, {3, "3"}};
    print(m);
    remove2(m);
    print(m);
    */
    testPartialMatch();

    return 0;
}
