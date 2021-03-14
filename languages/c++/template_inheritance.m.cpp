#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>

using std::vector;
using std::map;
using std::runtime_error;
using std::make_pair;

using std::cout;
using std::endl;

using std::ostream;
using std::stringstream;

struct Date
{
    int m_year;
    int m_month;
    int m_day;

    ostream& print(ostream& os) const
    {
        os << m_year << '/' << m_month << '/' << m_day;
        return os;
    }

    bool operator<(const Date& d) const
    {
        if(m_year < d.m_year)
        {
            return true;
        }
        else
        {
            if(m_month < d.m_month)
            {
                return true;
            }
            else
            {
                return m_day < d.m_day;
            }
        }
    }
};

ostream& operator<<(ostream& os, const Date& d)
{
    return d.print(os);
}


template<typename KeyType>
struct Cache
{
    Cache();

    virtual KeyType getKey(const KeyType& key, int yearIndex) const = 0;

    double getData(const KeyType& key) const;
    void fillVec(const KeyType& key, int numOfYears, vector<double>& vec) const;

    map<KeyType, double> m_data;
};

template<typename KeyType>
Cache<KeyType>::Cache():m_data() {}

template<typename KeyType>
double Cache<KeyType>::getData(const KeyType& key) const
{
    typename map<KeyType, double>::const_iterator itr = m_data.find(key);
    if(m_data.end() == itr)
    {
        stringstream ss;
        ss << "key not found=" << key;
        throw runtime_error(ss.str());
    }
    return itr->second;
}

template<typename KeyType>
void Cache<KeyType>::fillVec(
        const KeyType& key, int numOfYears, vector<double>& vec) const
{
    vec.resize(numOfYears);
    for(int i = 0; i < numOfYears; ++i)
    {
        vec[i] = this->getData(this->getKey(key, i));
    }
}

struct CacheInt : public Cache<int>
{
    CacheInt(int numOfYears, int numOfRates, double ttff);

    virtual int getKey(const int& key, int yearIndex) const;
};

CacheInt::CacheInt(int numOfYears, int numOfRates, double ttff)
{
    for(int i = 0; i < numOfRates; ++i)
    {
        for(int j = 0; j <= numOfYears; ++j)
        {
            m_data.insert(make_pair(i+12*j, (i+12*j)*ttff));
        }
    }
}

int CacheInt::getKey(const int& key, int yearIndex) const
{
    return key+12*yearIndex;
}

struct CacheDate : public Cache<Date>
{
    CacheDate(int numOfYears, const vector<Date>& dates);

    virtual Date getKey(const Date& key, int yearIndex) const;
};

CacheDate::CacheDate(int numOfYears, const vector<Date>& dates)
{
    for(const Date& d : dates)
    {
        for(int i = 0; i <= numOfYears; ++i)
        {
            Date d1{d.m_year+i, d.m_month, d.m_day};
            m_data.insert(make_pair(d1, 0.01));
        }
    }
}

Date CacheDate::getKey(const Date& key, int yearIndex) const
{
    return {key.m_year+yearIndex, key.m_month, key.m_day};
}

template<typename T>
void printVec(const vector<T>& vec)
{
    cout << "size=" << vec.size() << " ";
    for(const T& t : vec)
    {
        cout << t << ',';
    }
    cout << endl;
}


int main(int argc, char* argv[])
{
    CacheInt ci(30, 360, 0.1);

    vector<double> vec1;
    ci.fillVec(200, 30, vec1);

    printVec(vec1);


    vector<Date> dates{
            {2016, 10, 1},
            {2016, 11, 1}, 
            {2016, 12, 1}, 
            {2017, 1, 1},
            {2017, 2, 1},
            {2017, 3, 1},
            {2017, 4, 1},
            {2017, 5, 1},
            {2017, 6, 1},
            {2017, 7, 1},
            {2017, 8, 1},
            {2017, 9, 1},
            {2017, 10, 1},
            {2017, 11, 1},
            {2017, 12, 1},
            {2018, 1, 1},
            {2018, 2, 1},
            {2018, 3, 1},
            {2018, 4, 1},
            {2018, 5, 1},
            {2018, 6, 1},
            {2018, 7, 1},
            {2018, 8, 1},
            {2018, 9, 1},
            {2018, 10, 1},
            {2018, 11, 1},
            {2018, 12, 1}
    };
    CacheDate cd(2, dates); 

    vector<double> vec2;
    cd.fillVec({2016, 10, 1}, 2, vec2);

    printVec(vec2);

    return 0;
}
