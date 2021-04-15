// design a parking lot using object oriented principles

// though cars get in and out of parking lot by their drivers
// sometimes the parking lot has to make adjustment. So once the car is in the
// parking lot, the lot *owns* the car in the sense of parking lot management

// a parking lot has slots with numbers on it
//    at the end of the day, parking lot is a map of spot to optional<car>

// a commercial parking lot also has to track time for each in and out
// it provides a rule of rates

// it prints out ticket

// it also provides certain driving rules

// it tracks finances

#include <chrono>
#include <string>
#include <map>

#include <boost/optional/optional.hpp>

using namespace std;
using namespace boost;

struct Accounting;
struct Ticket; // tracks car information and check in time
struct LotSpace; // parking spots
struct ParkingLot;

// a bunch of rules of calculating rates. If there are ever going to be one
// set of rules, make all member functions static
// if there are potentially more accounting rules, make it stateful and 
// potentially a hierarchy
struct Accounting 
{
    double calculate(
            const chrono::system_clock::time_point& start, 
            const chrono::system_clock::time_point& end)
    {
        return 10.0;
    }
};

struct Ticket
{
    Ticket(
            const string& licensePlate, 
            const chrono::system_clock::time_point checkInTime):
        licensePlate(licensePlate), 
        checkInTime(checkInTime)
    {
    }

    void printTicket() const
    {
    }

    string licensePlate;
    chrono::system_clock::time_point checkInTime;
};


// for simplicity, use int to represent spots
struct LotSpace
{
    LotSpace(size_t lotCount):usage(), numFreeSpots(lotCount)
    {
        for (size_t i = 0; i < lotCount; ++i)
        {
            usage.emplace(i, optional<Ticket>());
        }
    }

    // sequentially sent back spot
    // can use a random selection algorithm too to keep better distribution
    optional<Ticket>& getTicketSpot()
    {
        if(numFreeSpots == 0)
        {
            throw std::runtime_error("parking lot is full");
        }
        for (auto& kv : usage)
        {
            if (!kv.second)
            {
                return kv.second;
            }
        }
        throw std::runtime_error("internal consistency in LotSpace object");
    }

    const Ticket& driveIn(
            const chrono::system_clock::time_point& checkInTime, 
            const string& licensePlate)
    {
        optional<Ticket>& ticketSpot = this->getTicketSpot();
        ticketSpot = Ticket(licensePlate, checkInTime);
        assert(numFreeSpots > 0);
        --numFreeSpots;
    }

    map<int, optional<Ticket>> usage;
    size_t numFreeSpots;
};


struct ParkingLot
{
    ParkingLot(size_t lotCount):space(lotCount), acct() {}

    double calculateReceivable(
            int lotId,
            const chrono::system_clock::time_point& checkOutTime) const
    {
    }

    LotSpace space;
    Accounting acct;
};

int main(int argc, char* argv[])
{


    return 0;
}
