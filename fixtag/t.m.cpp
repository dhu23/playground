#include "tags.h"
#include <sstream>
// build a fix message that looks like
// 17 ---> 2
//   1 -> TEST
//   11 -> ORD1
//   78 ---> 2
//       79 -> acct1
//       80 -> tag80_0
//       79 -> acct2
//       80 -> tag80_0
// 
//   1 -> TEST
//   11 -> ORD2
//   78 ---> 2
//       79 -> acct1
//       80 -> tag80_0
//       79 -> acct2
//       80 -> tag80_1
// 18 -> tag18
// 19 -> tag19
// 20 ---> 2
//    111 -> tag111
//    112 -> tag112
//
//    111 -> tag111
//    112 -> tag112
Tags* build0(const std::string& type)
{
    Tags* grp = new Tags();
    grp->m_kvmap.emplace(79, "acct_"+type);
    grp->m_kvmap.emplace(80, "tag80_"+type);
    return grp;
}

Tags* build1(const std::string& type, const std::vector<std::string>& types)
{
    Tags* t = new Tags();
    t->m_kvmap.emplace(1, "TEST"+type);
    t->m_rgmap.emplace(78, std::vector<Tags*>());

    for (const std::string& tt : types)
    {
        t->m_rgmap[78].emplace_back(build0(tt));
    }
    return t;
}

Tags* build2(const std::string& type)
{
    Tags* grp = new Tags();
    grp->m_kvmap.emplace(111, "tag111_"+type);
    grp->m_kvmap.emplace(112, "tag112_"+type);
    return grp;
}

Tags* build3(const std::vector<std::string>& types)
{
    Tags* t = new Tags();
    t->m_rgmap.emplace(20, std::vector<Tags*>());

    for (const std::string& tt : types)
    {
        t->m_rgmap[20].emplace_back(build2(tt));
    }
    return t;
}

Tags* build()
{
    Tags* ret = new Tags();
    ret->m_kvmap.emplace(18, "tag18");
    ret->m_kvmap.emplace(19, "tag19");

    ret->m_rgmap.emplace(17, std::vector<Tags*>());
    ret->m_rgmap[17].emplace_back(
        build1("A", std::vector<std::string>{"A1", "A2", "A3"}));
    ret->m_rgmap[17].emplace_back(
        build1("B", std::vector<std::string>{"B1", "B2"}));

    ret->m_rgmap.emplace(20, std::vector<Tags*>());
    ret->m_rgmap[20].emplace_back(build2("AA"));

    return ret;
}

void verify(Tags& t)
{
    std::cout << "input ==> " << std::endl;
    t.print(std::cout) << std::endl;

    std::stringstream ss;
    t.print(ss);
    std::string exp = ss.str();

    std::cout << std::endl << exp << std::endl << std::endl;

    std::istringstream iss(exp);
    
    Tags tt;
    iss >> tt;
    tt.print(std::cout) << std::endl;

    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    Tags* t = build();

    std::cout << *t;
    std::cout << std::endl;

    Tags t2(*t);

    delete t;

    std::cout << "t is cleaned up" << std::endl;

    std::cout << t2 << std::endl;
    t2.print(std::cout, 0);

    Tags* t0 = build0("A");
    verify(*t0);

    Tags* t1 = build1("A", std::vector<std::string>{"A1", "A2"});
    verify(*t1);

    verify(t2);
    
    // std::string exp2 = "[(18=tag18, 19=tag19), <17=2, [(1=TESTA), <78=3, [(79=acct_A1, 80=tag80_A1)], [(79=acct_A2, 80=tag80_A2)], [(79=acct_A3, 80=tag80_A3)]>], [(1=TESTB), <78=2, [(79=acct_B1, 80=tag80_B1)], [(79=acct_B2, 80=tag80_B2)]>]>, <20=1, [(111=tag111_AA, 112=tag112_AA)]>]";

    // std::stringstream ss(exp);
    // Tags t3;
    // ss >> t3;

    std::cout << "finishing programming" << std::endl;

    return 0;
}
