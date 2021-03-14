#include <thread>
#include <iostream>
#include <string>

void function1() 
{
    std::cout << "Beauty is only skin-deep" << std::endl;
}

class FuncObj 
{
public:
    void operator()() const
    {
        for (int i = 0; i > -100; --i)
        {
            std::cout << "from t1 " << i << std::endl;
        }
    }

    void operator()(const std::string& msg) 
    {
        std::cout << "t1 says: " << msg << std::endl;
    }
};

int main(int argc, char* argv[])
{
    // it can take any callable object, i.e. function object or lambda
    // std::thread t1(function1); // t1 starts running
    // FuncObj fo;
    // std::thread t1(fo);

    std::string s = "Where there is no trust, there is no love.";
    // a parameter to a thread is always passed by value. 
    // If you really want to pass by reference, do
    // std::thread t1((FuncObj()), std::ref(s));
    // if you don't want to use s in the main thread, and only want to 
    // use it in the t1 thread, use std::move(s)
    std::thread t1((FuncObj()), s);

    // to transfer t1 to t2 thread, use move as thread is not copyable
    // std::thread t2 = std::move(t1); 

    std::cout << std::this_thread::get_id() << std::endl;
    std::cout << t1.get_id() << std::endl;

    // std::thread t1(FuncObj()) won't compile. It is treated as a function
    // declaration. 
    //
    // fix: do it explicitly
    // std::thread t1((FuncObj()));

    // this is an alternative approach to the catch join
    // using RAII
    // wrapper w(t1);
    
    // t1.join(); // main thread waits for t1 to finish

    // t1.detach(); // t1 will run freely on its own --daemon process
    // once t1 is finished, C++ runtime library will be responsible for
    // reclaiming its resource. And now the main program runs so fast it
    // finishes before t1 prints out the message. 

    // once t1 is detached, it stays detached. (not joinable)
    // if (t1.joinable()) t1.join();


    // thread should be joined or detached ONLY ONCE. 
    // what if it is neither case? then the program will terminate if t1
    // terminates.

    try 
    {
        /*
        for (int i = 0; i < 100; ++i)
        {
            std::cout << "from main:" << i << std::endl;
        }
        */
        std::cout << "from main: " << s << std::endl;
    }
    catch (...) 
    {
        t1.join(); // ensure t1 is joined with/without exception
        throw;
    }

    

    // Oversubscription runs more threads than available cores. 
    // it causes context switch and reduces performance
    // indication function from C++ library
    std::cout << std::thread::hardware_concurrency() << std::endl;


    t1.join();

    return 0;
}
