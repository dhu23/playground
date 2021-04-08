#include <vector>

struct C0
{
    C0(int x): n(x) {}
    int n;
};

class P0
{
public:
    P0(): cs(), evens(0) {}
    void add(const C0& c);  
private:
    std::vector<C> cs;
    int evens;
};

void P0::add(const C0& c)
{
    cs.push_back(c);
    if (c.n % 2 == 0)
    {
        ++evens;
    }
}

////// TE style ///////

struct C
{
    C(int x): n(x) {}
    int n;
    P* p_;

    void onAddedToP();
};

class P
{
public:
    P(): cs(), evens(0) {}
    void addC(const C& c);
    void onAddedC(C* c);
};

void C::onAddedToP(P* p)
{
    p_ = p;
    p_->onAddedC(this);
}

void P::addC(const C& c)
{
    cs.push_back(c);
    cs.back().onAddedToP(this);
}

void P::onAddedC(C* c)
{
    if (c->n % 2 == 0)
    {
        ++evens;
    }
}
