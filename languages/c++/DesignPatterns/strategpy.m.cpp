#include <iostream>
#include <string>


class HealthCalcFunc
{
    public:
        HealthCalcFunc(){}
        virtual ~HealthCalcFunc(){}
 
        virtual void report() const = 0;        
        virtual double operator()(const double damage) const = 0;
};


class Character
{
    public: 
        Character(const std::string name, const double health);
        virtual ~Character();

        virtual void report() const = 0;
        virtual void takeDamage(const double damage);

        void setHealthCalcFunc(HealthCalcFunc* func);
    protected:
        std::string name_;
        double health_;
        HealthCalcFunc* healthCalc_;
};

Character::Character(const std::string name, const double health): 
    name_(name), health_(health)
{
}

Character::~Character()
{
}

void Character::takeDamage(const double damage)
{
    health_ -= (*healthCalc_)(damage);
}

void Character::setHealthCalcFunc(HealthCalcFunc* func)
{
    healthCalc_ = func;
}

class GoodGuy: public Character
{
    public:
        GoodGuy(const std::string& name, const double health);
        virtual ~GoodGuy();
    
        virtual void report() const;
};

GoodGuy::GoodGuy(const std::string& name, const double health):
    Character(name, health)
{
}

GoodGuy::~GoodGuy()
{
}

void GoodGuy::report() const
{
    std::cout 
        << "GoodGuy: "
        << "name = " << name_ << " health = " << health_ << std::endl;
} 


class EvilGuy: public Character
{
    public: 
        EvilGuy(const std::string& name, const double health);
        virtual ~EvilGuy();
      
        virtual void report() const;
};

EvilGuy::EvilGuy(const std::string& name, const double health):
    Character(name, health)
{
}

EvilGuy::~EvilGuy()
{
}

void EvilGuy::report() const
{
    std::cout 
        << "EvilGuy: " 
        << "name = " << name_ << " health = " << health_ << std::endl;
}


class SlowHealthLoser: public HealthCalcFunc
{
    public:
        SlowHealthLoser(){}
        ~SlowHealthLoser(){}  
        
        void report() const;
        double operator()(const double damage) const;
};

void SlowHealthLoser::report() const
{
    std::cout << "Slow Health-losing Calculator" << std::endl;
}

double SlowHealthLoser::operator()(const double damage) const
{
    return damage/2.0;
}

class FastHealthLoser: public HealthCalcFunc
{
    public:
        FastHealthLoser(){}
        ~FastHealthLoser(){}

       void report() const;
       double operator()(const double damage) const;
};

void FastHealthLoser::report() const
{
    std::cout << "Fast Health-losing Calculator" << std::endl;
}

double FastHealthLoser::operator()(const double damage) const
{
    return damage*2.0;
}

int main(int argc, char** argv)
{
    Character* goodGuy = new GoodGuy("player", 100);
    Character* evilGuy = new EvilGuy("boss", 100);

    HealthCalcFunc* slowLoser = new SlowHealthLoser();
    HealthCalcFunc* fastLoser = new FastHealthLoser();

    goodGuy->setHealthCalcFunc(fastLoser);
    goodGuy->report();
    goodGuy->takeDamage(10.0);
    goodGuy->report();

    evilGuy->setHealthCalcFunc(slowLoser);
    evilGuy->report();
    evilGuy->takeDamage(10.0);
    evilGuy->report();

    return 0;
}
