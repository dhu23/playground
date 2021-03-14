#ifndef INCLUDED_DHU_EXAMPLE_SCHEMA_H
#define INCLUDED_DHU_EXAMPLE_SCHEMA_H

#include <string>
#include <vector>
#include <iostream>

namespace dhu
{

class TypeOne
{
    std::string _stringData;
    double _doubleData;

public:
    // TypeOne();
    // TypeOne(const TypeOne& other);
    // TypeOne& operator=(const TypeOne& other);

    std::string& stringData();
    const std::string& stringData() const;

    double& doubleData();
    const double doubleData() const;

    std::ostream& print(std::ostream& os, int offset, int indent) const;
};

std::ostream& operator<<(std::ostream& os, const TypeOne& rhs);

class TypeTwo
{
    std::vector<std::string> _stringVecData;
    TypeOne _typeOneData;
    std::vector<TypeOne> _typeOneVecData;

public:
    // TypeTwo();
    // TypeTwo(const TypeTwo& other);
    // TypeTwo& operator=(const TypeTwo& other);

    std::vector<std::string>& stringVecData();
    const std::vector<std::string>& stringVecData() const;

    TypeOne& typeOneData();
    const TypeOne& typeOneData() const;

    std::vector<TypeOne>& typeOneVecData();
    const std::vector<TypeOne>& typeOneVecData() const;

    std::ostream& print(std::ostream& os, int offset, int indent) const;
};

std::ostream& operator<<(std::ostream& os, const TypeTwo& rhs);

inline 
std::string& TypeOne::stringData()
{
    return _stringData;
}

inline 
const std::string& TypeOne::stringData() const
{
    return _stringData;
}

inline
double& TypeOne::doubleData()
{
    return _doubleData;
}

inline
const double TypeOne::doubleData() const
{
    return _doubleData;
}

inline
std::vector<std::string>& TypeTwo::stringVecData()
{
    return _stringVecData;
}

inline
const std::vector<std::string>& TypeTwo::stringVecData() const
{
    return _stringVecData;
}

inline
TypeOne& TypeTwo::typeOneData()
{
    return _typeOneData;
}

inline
const TypeOne& TypeTwo::typeOneData() const
{
    return _typeOneData;
}

inline
std::vector<TypeOne>& TypeTwo::typeOneVecData()
{
    return _typeOneVecData;
}

inline
const std::vector<TypeOne>& TypeTwo::typeOneVecData() const
{
    return _typeOneVecData;
}


inline
std::ostream& operator<<(std::ostream& os, const TypeOne& rhs)
{
    return rhs.print(os, 0, 4);
}

inline
std::ostream& operator<<(std::ostream& os, const TypeTwo& rhs)
{
    return rhs.print(os, 0, 4);
}

} // end of namespace dhu

#endif
