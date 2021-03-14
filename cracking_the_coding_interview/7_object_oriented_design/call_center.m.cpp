// imagine you have a call center with 3 levels of empolyees, respondent
// manager and director. An incoming telephone call must be first allocated to 
// a respondent who is free. If the respondent can't handle the call, he or 
// she must escalate the call to a manager. If the manager is not free or not
// able to handle it, then the call should be escalated to a director. Design
// the classes and data structure for this problem. 

// implmement a method dispatchCall() which assigns a call to the first 
// available employee

#include <cassert>

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <list>

#include <algorithm>

#include <boost/optional/optional.hpp>

struct Call 
{
    int id;
    std::ostream& show(std::ostream& os) const
    {
        os << "Call=" << id;
        return os;
    }
};

enum class EmployeeLevel { RESPONDENT = 2, MANAGER = 1, DIRECTOR = 0 };
EmployeeLevel getSupervisorLevel(EmployeeLevel level)
{
    if (level == EmployeeLevel::DIRECTOR)
    {
        throw std::runtime_error("at the top of the hierarchy");
    }
    return static_cast<EmployeeLevel>(static_cast<int>(level)-1);
}
// employee does not decide if it is free or not. 
// being free or busy is a state within the call center, therefore
// it only makes sense within CallCenter class
struct Employee
{
    Employee(const std::string& n, EmployeeLevel l):level(l), name(n) {}
    EmployeeLevel level;
    std::string name;

    std::ostream& show(std::ostream& os) const
    {
        os << "Employee=" << name << '(' << static_cast<int>(level) << ')';
        return os;
    }

    bool operator<(const Employee& other) const
    {
        if (level == other.level)
        {
            return name < other.name; // names should be unique
        }
        return level < other.level;
    }
};

class CallCenter
{
    public:
        CallCenter():
            _assignments(), _unassigned(), _escalated(), _finished() {}

        CallCenter(
                const std::set<std::string>& respondents, 
                const std::set<std::string>& managers, 
                const std::set<std::string>& directors):    
            _assignments(),
            _unassigned(),
            _escalated(), 
            _finished()
        {
            for (const std::string& r : respondents)
            {
                _assignments.emplace(
                        Employee{r, EmployeeLevel::RESPONDENT},
                        boost::optional<Call>());
            }
            for (const std::string& m : managers)
            {
                _assignments.emplace(
                        Employee{m, EmployeeLevel::MANAGER},
                        boost::optional<Call>());
            }
            for (const std::string& d : directors)
            {
                _assignments.emplace(
                        Employee{d, EmployeeLevel::DIRECTOR},
                        boost::optional<Call>());
            }
        }

        CallCenter(
                std::vector<std::string>&& respondents, 
                std::vector<std::string>&& managers, 
                std::vector<std::string>&& directors):
            _assignments(),
            _unassigned(),
            _escalated(),
            _finished()
        {
            for (const std::string& r : respondents)
            {
                _assignments.emplace(
                        Employee{r, EmployeeLevel::RESPONDENT},
                        boost::optional<Call>());
            }
            for (const std::string& m : managers)
            {
                _assignments.emplace(
                        Employee{m, EmployeeLevel::MANAGER},
                        boost::optional<Call>());
            }
            for (const std::string& d : directors)
            {
                _assignments.emplace(
                        Employee{d, EmployeeLevel::DIRECTOR},
                        boost::optional<Call>());
            }
        }

        void addEmployee(const Employee& e)
        {
            auto it = _assignments.find(e);
            if (it == _assignments.end())
            {
                _assignments.emplace(e, boost::optional<Call>());
            }
        }

        void addEmployee(const std::string& name, EmployeeLevel level)
        {
            this->addEmployee(Employee{name, level});
        }
        
        // not concurrency safe yet
        boost::optional<Employee> receiveCall(const Call& c)
        {
            auto it = this->findAvailableEmployee(EmployeeLevel::RESPONDENT);
            if (it == _assignments.end())
            {
                // everybody is busy
                _unassigned.push_back(c);
                return boost::optional<Employee>();
            }
            else
            {
                it->first.show(std::cout) << std::endl;
                it->second = c;
                return boost::optional<Employee>(it->first);
            }
            
        }

        Call escalate(const Employee& e)
        {
            std::pair<const Employee, boost::optional<Call>>& p = 
                findEmployee(e);
            if (!p.second)
            {
                throw std::runtime_error("employee is free. cannot escalate");
            }
            EmployeeLevel level = e.level;
            if (level == EmployeeLevel::DIRECTOR)
            {
                throw std::runtime_error("Director cannot escalate");
            }
            
            std::map<Employee, boost::optional<Call>>::iterator it;
            do
            {
                level = getSupervisorLevel(level);
                it = findAvailableEmployee(level);
            }
            while (it == _assignments.end() && level != EmployeeLevel::DIRECTOR);

            if (it == _assignments.end())
            {
                // added back to the escalated queue
                _escalated.push_back(*p.second);
            }
            else
            {
                assert(!it->second); // assert the employee is free
                it->second = p.second;
            }
            p.second.reset();

            if (!_unassigned.empty())
            {
                Call temp = _unassigned.front();
                _unassigned.pop_front();
                p.second = temp;
            }
        }


        std::ostream& inspect(std::ostream& os) const
        {
            os << "%%%%%% Call Center Review %%%%%%" << std::endl;
            os << "assignments:" << std::endl;
            for (auto& kv : _assignments)
            {
                kv.first.show(os);
                os << "--->";
                if (kv.second)
                {
                    kv.second->show(os);
                }
                else
                {
                    os << "free";
                }
                os << std::endl;
            }
            os << "unassigned calls:" << std::endl;
            for (auto& c : _unassigned)
            {
                c.show(os) << std::endl;
            }
            os << "escalated calls:" << std::endl;
            for (auto& e : _escalated)
            {
                e.show(os) << std::endl;
            }
            os << "finished calls:" << std::endl;
            for (auto& f : _finished)
            {
                f.show(os) << std::endl;
            }
            return os;
        }

        void finish(const Employee& e)
        {
            std::pair<const Employee, boost::optional<Call>>& p = 
                this->findEmployee(e);
            if (!p.second)
            {
                throw std::runtime_error("employee is free. cannot finish");
            }
            _finished.push_back(*p.second);
            p.second.reset();
        }

    private:
        std::pair<const Employee, boost::optional<Call>>& 
            findEmployee(const Employee& e)
        {
            auto it = _assignments.find(e);
            if (it == _assignments.end())
            {
                throw std::runtime_error("Employee not participating");
            }
            return *it;
        }

        std::map<const Employee, boost::optional<Call>>::iterator
            findAvailableEmployee(EmployeeLevel level)
        {
            return std::find_if(
                    _assignments.begin(), _assignments.end(), 
                    [level](const std::pair<Employee, boost::optional<Call>>& kv)
                    {
                        return kv.first.level == level && !kv.second;
                    }
                );
        }

        std::map<Employee, boost::optional<Call>> _assignments;
        std::list<Call> _unassigned;
        std::list<Call> _escalated;
        std::vector<Call> _finished;
};


// CallCenter assign a call to an employee
// employee will process the call
// some time later the employee might escalate the call back to the center

// data structure should look like:
// respondent1 --- free
// respondent2 --- call 100
// respondent3 --- call 101
// respondent4 --- free
// manager1 --- call 102
// manager2 --- free
// director --- free

// unhandled calls --- call 103, call 104, call 105

int main(int argc, char* argv[])
{
    std::set<std::string> respondents{"r1", "r2", "r3", "r4"};
    std::set<std::string> managers{"m1", "m2"};
    std::set<std::string> directors{"d1"};
    CallCenter cc{respondents, managers, directors};
    
    cc.receiveCall(Call{100});
    cc.receiveCall(Call{101});
    cc.receiveCall(Call{102});
    cc.receiveCall(Call{103});
    cc.receiveCall(Call{104});
    cc.inspect(std::cout);

    cc.escalate({"r1", EmployeeLevel::RESPONDENT});
    cc.inspect(std::cout);

    try
    {
        cc.escalate({"r1", EmployeeLevel::RESPONDENT});
    }
    catch(std::exception& exc)
    {
        std::cout << exc.what() << std::endl;
    }
    
    cc.escalate({"r2", EmployeeLevel::RESPONDENT});
    cc.escalate({"r3", EmployeeLevel::RESPONDENT});
    cc.escalate({"r4", EmployeeLevel::RESPONDENT});
    cc.inspect(std::cout);

    cc.receiveCall(Call{200});
    cc.receiveCall(Call{201});
    cc.inspect(std::cout);

    cc.finish({"r1", EmployeeLevel::RESPONDENT});
    cc.finish({"d1", EmployeeLevel::DIRECTOR});
    cc.escalate({"m1", EmployeeLevel::MANAGER});
    cc.inspect(std::cout);

    return 0;
}
