#include <iostream>
#include <string> 

class Vehical
{
    public: 
        Vehical(const int capacity);
        Vehical(const Vehical& vehical);
        virtual ~Vehical();

        virtual int getCapacity() const;
        virtual void report() const;
    private:
        int capacity_;
};

Vehical::Vehical(const int capacity): capacity_(capacity)
{
}

Vehical::Vehical(const Vehical& v): capacity_(v.getCapacity())
{
}

Vehical::~Vehical()
{
}

int Vehical::getCapacity() const
{
    return this->capacity_;
}

void Vehical::report() const
{
    std::cout 
        << "Base Vehical: " 
        << "capacity=" << this->getCapacity()
        << std::endl;
}

class DecoratedVehical: public Vehical
{
    public: 
        DecoratedVehical(Vehical* v);
        virtual ~DecoratedVehical();

        virtual int getCapacity() const = 0;
        virtual void report() const = 0;
    protected:
        Vehical* vehical_;
};

DecoratedVehical::DecoratedVehical(Vehical* v): 
    Vehical(*v), vehical_(v)
{
}

DecoratedVehical::~DecoratedVehical()
{
}

class Suv: public DecoratedVehical
{
    public:
        Suv(Vehical* v);
        virtual ~Suv();

        virtual int getCapacity() const;
        virtual void report() const;
};

Suv::Suv(Vehical* v): DecoratedVehical(v)
{
}

Suv::~Suv()
{
}

int Suv::getCapacity() const
{
    return this->vehical_->getCapacity() + 3;
}

void Suv::report() const
{
    std::cout 
        << "SUV: "
        << "capacity = " << this->getCapacity()
        << std::endl;
}

class Hatchback: public DecoratedVehical
{
    public: 
        Hatchback(Vehical* v);
        virtual ~Hatchback();
        
        virtual int getCapacity() const;
        virtual void report() const;

        // there is a backdoor in hatchback
        virtual void openBackDoor() const;
};

Hatchback::Hatchback(Vehical* v): DecoratedVehical(v)
{
}

Hatchback::~Hatchback()
{
}

int Hatchback::getCapacity() const
{
    // hatchback doesn't provide more seats
    return this->vehical_->getCapacity();
}

void Hatchback::report() const
{
    std::cout 
        << "Hatchback: "
        << "capacity = " << this->getCapacity() << " " 
        << "has a backdoor"
        << std::endl;
}

void Hatchback::openBackDoor() const
{
    std::cout << "Open backdoor" << std::endl;
}

int main(int argc, char** argv)
{
    Vehical v1(5);
    v1.report();

    Suv s1(&v1);
    s1.report();

    Hatchback h1(&v1);
    h1.report();
    h1.openBackDoor();
    
    return 0;
}
