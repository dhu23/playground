#ifndef _TAGS_H_
#define _TAGS_H_

#include <vector>
#include <string>
#include <map>
#include <iostream>

class Tags;

typedef std::map<int, std::string> KVMap;
typedef std::map<int, std::vector<Tags*>> RGMap;

class Tags
{
public:
    
    Tags();
    Tags(const Tags& other);
    Tags(Tags&& other);
    Tags& operator=(const Tags& other);
    Tags& operator=(Tags&& other);

    ~Tags();

    void clear();

    std::ostream& print(std::ostream& os, int level) const;
    std::ostream& print(std::ostream& os) const;
    std::istream& populate(std::istream& is);

    static std::istream& populateTags(std::istream& is, Tags& rhs);

// private:
    KVMap m_kvmap;
    RGMap m_rgmap;
};

std::ostream& operator<<(std::ostream& os, const Tags& rhs);
std::istream& operator>>(std::istream& is, Tags& rhs);
#endif
