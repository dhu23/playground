#include <map>
#include <iostream>
#include <random>

int rand1(int lower, int upper)
{
    int diff = upper-lower;
    return lower+(diff * (rand()/(double)RAND_MAX));
}

int rand2(int lower, int upper)
{
    return rand() % (upper-lower+1) + lower;
}

int rand3(int lower, int upper)
{
    int n = upper-lower+1;
    int remainder = RAND_MAX%n;
    int x;
    do 
    {
        x = rand();
    }
    while (x >= RAND_MAX-remainder);
    return lower + x % n;
}

typedef int (*RandF)(int, int);

void test(int lower, int upper, RandF f)
{
    std::cout << "RAND_MAX=" << RAND_MAX << std::endl;
    std::map<int, int> counter;
    for (int i = 0; i < 100000; ++i)
    {
        int ret = f(lower, upper);
        auto it = counter.find(ret);
        if (it == counter.end())
        {
            counter.emplace(ret, 1);
        }
        else
        {
            ++(it->second);
        }
    }
    std::cout << "rand between " << lower << " and " << upper << std::endl;
    for (const auto& kv : counter)
    {
        std::cout << kv.first << "--->" << kv.second << std::endl;
    }
    std::cout << std::endl;
}

void testSeedSeq()
{
    std::seed_seq seq{1, 2, 3, 4};
    std::vector<std::uint32_t> seeds(10);
    seq.generate(seeds.begin(), seeds.end());
    for (std::uint32_t n : seeds) std::cout << n << ",";
    std::cout << std::endl;
}

void testMt()
{
    std::seed_seq seed2{1, 2}; 
    std::mt19937 e2(seed2);
    int count = 10;
    while (--count > 0)
    std::cout << e2() << std::endl;
}

/*
template<template<T> Distribution>
void get(T l, T h)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    Distribution<T> dist(l, h);

}
*/

int main(int argc, char* argv[])
{
    // test(1, 10, rand1);
    // test(1, 10, rand2);
    // test(1, 10, rand3);
    testSeedSeq();
    testMt();
}
