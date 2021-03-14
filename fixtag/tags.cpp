#include "tags.h"
#include <iomanip>

namespace {

template<typename T>
std::vector<T*> deepcopy(const std::vector<T*>& from)
{
    std::vector<T*> ret;
    ret.reserve(from.size());

    for (const auto& p : from)
    {
        ret.push_back(new T(*p));
    }
    return std::move(ret);
}

std::istream& populateTag(std::istream& is, uint32_t& tag)
{
    char c;
    while (is >> c)
    {
        if (c == '=')
        {
            break;
        }
        tag = tag*10+c-'0';
    }
    return is;
}

bool populateValue(std::istream& is, std::string& value)
{
    char c;
    while (is >> c)
    {
        if (c == ',')
        {
            return false;
        }
        else if (c == ')')
        {
            return true;
        }
        value.append(1, c);
    }
}

std::istream& populateKVMap(std::istream& is, KVMap& kvMap)
{
    while (true)
    {
        // populate tag pairs
        uint32_t tag = 0;
        populateTag(is, tag);
        std::string value;
        bool done = populateValue(is, value);

        kvMap[tag] = value;
        if (done)
        {
            break;
        }
    }
    return is;
}

std::istream& populateRGMap(std::istream& is, RGMap& rgMap)
{
    uint32_t groupTag;
    char equals;
    size_t groupSize;
    char tmp;

    is >> groupTag >> equals >> groupSize >> tmp;
    std::vector<Tags*>& groups = rgMap[groupTag];
    groups.reserve(groupSize);
    
    while (groupSize > 0)
    {
        Tags* p = new Tags;
        Tags::populateTags(is, *p);
        groups.push_back(p);

        is >> tmp;
        if (tmp == ',') 
        {
            continue;
        }
        else if (tmp == '>')
        {
            break;
        }
    }
    return is;
}


} // end of anonymous namespace


Tags::Tags():
    m_kvmap(),
    m_rgmap()
{}

Tags::Tags(const Tags& other):
    m_kvmap(other.m_kvmap),
    m_rgmap()
{
    for (const auto& rgkv : other.m_rgmap)
    {
        m_rgmap.emplace(rgkv.first, deepcopy(rgkv.second));
    }
}

Tags::Tags(Tags&& other)
{
    clear();
    m_kvmap = std::move(other.m_kvmap);
    m_rgmap = std::move(other.m_rgmap);
}

Tags& Tags::operator=(const Tags& other)
{
    if (this == &other) 
    {
        return *this;
    }
    clear();
    *this = std::move(Tags(other));
    return *this;
}

Tags& Tags::operator=(Tags&& other)
{
    clear();
    m_kvmap = std::move(other.m_kvmap);
    m_rgmap = std::move(other.m_rgmap);
    return *this;
}

Tags::~Tags()
{
    clear();
}

void Tags::clear()
{
    m_kvmap.clear();
    for (auto& rgkv : m_rgmap)
    {
        std::cout 
            << "cleaning RG " << rgkv.first 
            << " size " << rgkv.second.size() << std::endl;
        for (auto& pTag : rgkv.second)
        {
            delete pTag;
        }
    }
    m_rgmap.clear();
}

std::ostream& Tags::print(std::ostream& os, int level) const
{
    for (const auto& kv : m_kvmap)
    {
        os << std::string(2*level, ' ');
        os << kv.first << " -> " << kv.second << std::endl;
    }
    for (const auto& rgkv : m_rgmap)
    {
        os << std::string(2*level, ' ');
        os << rgkv.first << " ---> " << rgkv.second.size() << std::endl;
        for (const auto& pTag : rgkv.second)
        {
            pTag->print(os, level+1) << std::endl;
        }
    }
    return os;
}

std::ostream& Tags::print(std::ostream& os) const
{
    os << "[";
    int c = 0;
    os << "(";
    for (const auto& kv : m_kvmap)
    {
        if (c != 0) os << ",";
        os << kv.first << "=" << kv.second;
        ++c;
    }
    os << ")";
    for (const auto& rgkv : m_rgmap)
    {
        os << ",<";
        os << rgkv.first << "=" << rgkv.second.size();
        for (const auto& pTag : rgkv.second)
        {
            os << ",";
            pTag->print(os);
        }
        os << ">";
    }
    os << "]";
    return os;
}

std::istream& Tags::populate(std::istream& is)
{
    char c;
    Tags tmp;
    while (is >> c)
    {
        if (c == '[')
        {
            populateTags(is, tmp);
        }
        break;
    }
    *this = std::move(tmp);

    return is;
}


std::istream& Tags::populateTags(std::istream& is, Tags& rhs)
{
    char c;
    while (is >> c)
    {
        if (c == ']')
        {
            break;
        }
        else if (c == '(')
        {
            populateKVMap(is, rhs.m_kvmap);
        }
        else if (c == '<')
        {
            populateRGMap(is, rhs.m_rgmap);
        }
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Tags& rhs)
{
    return rhs.print(os);
}

std::istream& operator>>(std::istream& is, Tags& rhs)
{
    return rhs.populate(is);
}
