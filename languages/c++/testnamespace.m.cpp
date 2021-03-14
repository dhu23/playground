#include <iostream>
#include <tr1/memory>

namespace ns1
{
    struct TestObj
    {
        TestObj(): m_data1(1), m_data2(0.0){} 
        

        int m_data1;
        double m_data2;
    };
}

namespace ns2
{
    typedef std::tr1::shared_ptr<ns1::TestObj> TestObjPtr;
    
    TestObjPtr createTestObj()
    {
        return std::tr1::shared_ptr<ns1::TestObj>(new ns1::TestObj());
    }
}

namespace ns3
{
    typedef std::tr1::shared_ptr<ns1::TestObj> TestObjPtr;

    TestObjPtr createTestObj()
    {
        return std::tr1::shared_ptr<ns1::TestObj>(new ns1::TestObj());
    }
}

int main(int argc, char* argv[])
{
    ns2::TestObjPtr p1 = ns2::createTestObj();
    ns3::TestObjPtr p2 = p1;

    std::cout << "p2->m_data1 = " << p2->m_data1 << std::endl;
    return 0;
}
