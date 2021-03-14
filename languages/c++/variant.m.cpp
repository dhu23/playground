#include <variant>
#include <iostream>
#include <string>
#include <vector>

struct Pass 
{
    std::vector<std::string> cancelList;
};

struct Reject {};

struct ACCheckVal
{
    std::variant<Pass, Reject> val;
};

struct ShowACCheckVal
{
    std::ostream& os;
    ShowACCheckVal(std::ostream& os):os(os) {}

    void operator()(const Pass& p)
    {
        os << "received a Pass object:";
        for (const std::string& s : p.cancelList)
        {
            os << s << '|';
        }
        os << std::endl;
    }

    void operator()(const Reject& r)
    {
        os << "received a Reject object";
        os << std::endl;
    }
};

template<typename L>
struct Left
{
    L val;
};

template<typename R>
struct Right 
{
    R val;
};

template<typename L, typename R>
struct Either
{
    using type = std::variant<Left<L>, Right<R>>;
    type val;
};

template<typename L, typename R>
struct ShowEither
{
    void operator()(const Left<L>& l) const
    {
        std::cout << "left=" << l.val << std::endl;
    }
    void operator()(const Right<R>& r) const
    {
        std::cout << "right=" << r.val << std::endl;
    }
};

int main(int argc, char* argv[])
{
    Pass p{{"A","B"}};
    Reject r{};

    std::vector<ACCheckVal> vec{{p}, {r}};
    
    for (const ACCheckVal& a : vec)
    {
        std::visit(ShowACCheckVal(std::cout), a.val);
    }

    Either<std::string, int> e1 = Right<int>(1);
    std::visit(ShowEither<std::string, int>(), e1.val);
    
    return 0;
}
