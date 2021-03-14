#include "dhu_exampleschema.h"

namespace dhu
{

namespace
{


std::ostream& print(std::ostream& os, int data, int offset, int indent)
{
    os << data;
    return os;
}


std::ostream& print(std::ostream& os, double data, int offset, int indent)
{
    os << data;
    return os;
}


std::ostream& print(
        std::ostream& os, const std::string& data, int offset, int indent)
{
    os << data;
    return os;
}

template< typename T > std::ostream& print(
        std::ostream& os, const T& data, int offset, int indent)
{
    return data.print(os, offset, indent);
}

// print vector of type 
// |<--- offset --->|field_name = [
// |<--- offset+indent---> element 1
//                         element 2
//                         .....
// |<--- offset --->|]

template< typename T > std::ostream& print(
        std::ostream& os, const std::vector< T >& data, int offset, int indent)
{
    const std::string offsetStr(offset, ' ');
    if(data.empty())
    {
        os << "[]";
        return os;
    }
    const std::string indentStr(indent, ' ');
    os << "[\n";
    for(const auto& d : data)
    {
        // os << offsetStr << indentStr << d << ",\n";
        os << offsetStr << indentStr;
        print(os, d, offset+indent, indent);
        os << ",\n";
    }
    os << offsetStr << "]";
    return os;
}

} // end of anonymous namespace

std::ostream& TypeOne::print(std::ostream& os, int offset, int indent) const
{
    const std::string offsetStr(offset, ' ');
    os << "[\n";
    
    const std::string indentStr(indent, ' ');
    
    os << offsetStr << indentStr << "stringData = " << _stringData << '\n';
    os << offsetStr << indentStr << "doubleData = " << _doubleData << '\n';
    os << offsetStr << "]";
    return os;
}

// print format:
// |<--offset-->|[
// |<--offset+indent-->|stringVecData = [
// |<--offset+indent+indent-->|string 1
//                             string 2
//                             string 3
//                      ]
//                      typeOneData = [
//                             stringData = string 4
//                             doubleData = double 1
//                      ]
//                      typeOneVecData = [
// |<--offset+indent*3 ------>|[          
//                                    stringData = string 5
//                                    doubleData = double 2
//                             ],
// |<--offset+indent*3 ------>|[          
//                                    stringData = string 5
//                                    doubleData = double 2
//                             ],
//                      ]
// |<--offset-->|[               
std::ostream& TypeTwo::print(std::ostream& os, int offset, int indent) const
{
    const std::string offsetStr(offset, ' ');
    os << "[\n";

    const std::string indentStr(indent, ' ');

    os << offsetStr << indentStr << "stringVecData = "; 
    dhu::print(os, _stringVecData, offset+indent, indent);
    os << '\n';
    os << offsetStr << indentStr << "typeOneData = ";
    _typeOneData.print(os, offset+indent, indent);
    os << '\n';
    os << offsetStr << indentStr << "typeOneVecData = ";
    dhu::print(os, _typeOneVecData, offset+indent, indent);
    os << '\n';

    os << offsetStr << ']';

    return os;
}


} // end of namespace dhu
