#include <iostream>
#include <string>


class Object
{
    public:
        Object(const std::string& name);
        Object(const Object& obj);
        Object& operator=(const Object& obj);
        ~Object();

        std::string& name();
        const std::string& name() const;

    private:
        std::string m_name;
};

Object::Object(const std::string& name):m_name(name)
{
    std::cout << "Constructing through Object(const string&)" << std::endl;
}

Object::Object(const Object& obj):m_name(obj.m_name)
{
    std::cout << "Constructing through Object(const Object&)" << std::endl;
}

Object& Object::operator=(const Object& obj)
{
    std::cout << "Assignment..." << std::endl;
    if(&obj != this)
    {
        this->m_name = obj.m_name;
    }
    return *this;
}

Object::~Object()
{
    std::cout << "Destructing..." << std::endl;
}

std::string& Object::name()
{
    return m_name;
}

const std::string& Object::name() const
{
    return m_name;
}

// return object
Object createObject(const std::string& name)
{
    return Object(name);
}

// fill object
void populateObject(const std::string& name, Object& obj)
{
    obj.name() = name;
}

int main(int argc, char* argv[])
{
    std::cout << "testing return object" << std::endl;
    Object o1(createObject("obj_name_1"));

    std::cout << "testing populate object" << std::endl;
    Object o2(o1);

    populateObject("obj_name_2", o2);
}
