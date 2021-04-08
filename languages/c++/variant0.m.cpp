#include <vector>
#include <string>
#include <boost/variant.hpp>
// #include <boost/variant/apply_visitor.hpp>
#include <boost/optional/optional.hpp>
struct Left 
{
    std::string err;
};

struct Right
{
    std::vector<int> vals;
};

using Either = boost::variant<Left, Right>;

struct EitherShow : public boost::static_visitor<>
{
    void operator()(const Left& l) const
    {
        std::cout << "left=" << l.err << std::endl;
    }
    void operator()(const Right& r) const
    {
        std::cout << "right=";
        for (int i : r.vals)
        {
            std::cout << i << '|';
        }
        std::cout << std::endl;
    }
};

void print(const Either& e)
{
    boost::apply_visitor(EitherShow(), e);
}

struct LeftBind : public boost::static_visitor<Either>
{
    Left l;
    LeftBind(const Left& left):l(left) {}
    Either operator()(const Left& other) const {return Left{l.err+other.err};}
    Either operator()(const Right& other) const {return l;}
};

struct RightBind : public boost::static_visitor<Either>
{
    Right r;
    RightBind(const Right& right):r(right) {}
    Either operator()(const Left& other) const {return other;}
    Either operator()(const Right& other) const 
    {
        std::vector<int> vals(r.vals);
        vals.insert(vals.end(), other.vals.begin(), other.vals.end());
        return Right{std::move(vals)};
    }
};

struct Bind : public boost::static_visitor<Either>
{
    Either e;
    Bind(const Either& either):e(either) {}
    Either operator()(const Left& l) const 
    {
        return boost::apply_visitor(LeftBind(l), e);
    }
    Either operator()(const Right& r) const 
    {
        return boost::apply_visitor(RightBind(r), e);
    }
};


int main(int argc, char* argv[])
{
    Either a(Left{"ABC"});
    print(a);
    Either b(Right{{1, 2, 3, 4}});
    print(b);
      
    Either a1(Left{"abc"});
    Either a2(Left{"def"});
    Either a3(Left{"DEF"});
    Either a4(Left{"xyz"});

    Either b1(Right{{11, 21, 31, 41}});
    Either b2(Right{{11, 22, 33, 44}});

    print(boost::apply_visitor(Bind{a}, a1));
    print(boost::apply_visitor(LeftBind(Left{"ABCD"}), a1));
    print(boost::apply_visitor(Bind{b1}, a1));
    print(boost::apply_visitor(Bind{a1}, a1));
    print(boost::apply_visitor(Bind{b1}, b1));
    print(boost::apply_visitor(Bind{a1}, b1));

    return 0;
}
