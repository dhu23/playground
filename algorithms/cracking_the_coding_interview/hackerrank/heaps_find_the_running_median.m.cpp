#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include <random>

using namespace std;

template<typename T>
struct MediumTracker
{
    MediumTracker(): lower(), upper() {}

    void add(T t)
    {
        upper.push_back(t);
        push_heap(upper.begin(), upper.end(), greater<T>());

        while (upper.size() - lower.size() > 1)
        {
            pop_heap(upper.begin(), upper.end(), greater<T>());
            T move = upper.back();
            upper.pop_back();

            lower.push_back(move);
            push_heap(lower.begin(), lower.end());
        }

        if (!lower.empty() && lower.front() > upper.front())
        {
            T upperFront = upper.front();
            upper.front() = lower.front();

            pop_heap(lower.begin(), lower.end());
            lower.back() = upperFront;
            push_heap(lower.begin(), lower.end());
        }
    }

    T medium() const
    {
        if (upper.size() > lower.size())
        {
            return upper.front() * 2;
        }
        else if (upper.size() == lower.size())
        {
            return upper.front() + lower.front();
        }
        else
        {
            throw runtime_error("bad");
        }
    }

    vector<T> lower;
    vector<T> upper;
};

template<typename T>
T getMediumBySort(const vector<T>& v)
{
    vector<T> vec(v);
    sort(vec.begin(), vec.end());
    if (vec.size() % 2 == 0)
    {
        size_t mid = vec.size() / 2;
        return vec[mid-1] + vec[mid];
    }
    else
    {
        return 2*vec[vec.size()/2];
    }
}

int main(int argc, char* argv[])
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, 50000);
    
    MediumTracker<size_t> mt;

    vector<size_t> v;
    for (size_t i = 0; i < 40; ++i)
    {
        size_t r = dis(gen);
        v.push_back(r);
        mt.add(r);

        cout << r << "->" << getMediumBySort(v) << " " << mt.medium() << endl;
    }
}

