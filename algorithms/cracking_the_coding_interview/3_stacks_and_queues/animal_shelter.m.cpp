// an animal shelter which holds only dogs and cats, operates on a strictly
// first in first out basis. people must adopt either the oldest (based on 
// arrival time) of all animals at the shelter. or they can select whether
// they woudl prefer a dog or a cat(and will receive the oldest animal of 
// that time). They can't select which specific animal they would like. Create
// the data structures to maintain this system and implement operations uch as 
// enqueue, dequeueAny, dequeueDog and dequeueCat. you may use the 
// built-in LinkedList data structure

#include "data_structure.h"

#include <string>
#include <map>
#include <iostream>

using std::cout;
using std::endl;
using ds::Queue;
using std::string;
using std::map;


class AnimalShelter
{
    public:
        enum class Animal {DOG, CAT};
        AnimalShelter() {}
        void enqueue(Animal type, const string& name, int arrivalTime)
        {
            switch (type)
            {
                case Animal::DOG:
                    enqueue(_dogs, _dogArrival, name, arrivalTime);
                    break;
                case Animal::CAT:
                    enqueue(_cats, _catArrival, name, arrivalTime);
                    break;
                default:
                    throw std::runtime_error("Unknown animal type");
            }
        }
        string dequeueAny()
        {
            int oldestDogArrival = _dogArrival.peek();
            int oldestCatArrival = _catArrival.peek();
            if (oldestDogArrival > oldestCatArrival)
            {
                return dequeueCat();
            }
            else
            {
                return dequeueDog();
            }
        }
        string dequeueDog()
        {
            return dequeue(_dogs, _dogArrival);
        }
        string dequeueCat()
        {
            return dequeue(_cats, _catArrival);
        }

    private:
        static string dequeue(
                Queue<string>& names, Queue<int>& arrivalTimes)
        {
            if (names.isEmpty() || arrivalTimes.isEmpty())
            {
                throw std::runtime_error("queue is empty");
            }
            arrivalTimes.remove();
            string name = names.peek();
            names.remove();
            return name;
        }
        static void enqueue(
                Queue<string>& names, Queue<int>& times, 
                const string& name, int arrival)
        {
            names.add(name);
            times.add(arrival);
        }

        Queue<string> _dogs;
        Queue<int> _dogArrival;

        Queue<string> _cats;
        Queue<int> _catArrival;
};

int main(int argc, char* argv[])
{
    AnimalShelter as;

    // cat1 dog1 cat2 dog2 dog3 dog4 cat3 dog5 cat4
    // 1    2    3    4    5    6     7   8    9

    as.enqueue(AnimalShelter::Animal::CAT, "cat1", 1);
    as.enqueue(AnimalShelter::Animal::DOG, "dog1", 2);
    as.enqueue(AnimalShelter::Animal::CAT, "cat2", 3);

    cout << as.dequeueAny() << endl; // "cat1"
    as.enqueue(AnimalShelter::Animal::DOG, "dog2", 4);
    as.enqueue(AnimalShelter::Animal::DOG, "dog3", 5);
    as.enqueue(AnimalShelter::Animal::DOG, "dog4", 6);
    cout << as.dequeueCat() << endl; // "cat2"
    


    return 0;
}
