// implement a CircularArray class that supports an array like data structure
// which can be efficiently rotated. If possible the class should use a 
// generic type and should support iteration via standard 
// for (Obj o : circularArray) notation

#include <array>
#include <iostream>


using namespace std;

template<typename T, size_t N>
struct CircularArray
{
    CircularArray():data(), start(0) {}

    T& operator[](size_t i) 
    {
        return data[(start+i) % N];
    }

    const T& operator[](size_t i) const
    {
        return data[(start+i) % N];
    }

    void rotate(size_t k)
    {
        start = (start+k) % N;
    }

    size_t size() const {return N;}

    struct iterator
    {
        array<T, N>& data;
        size_t start;
        T* p;
        
        iterator(array<T, N>& data, size_t start, T* p):    
            data(data), start(start), p(p) {}

        const T& operator*() const
        {
            return *p;
        }

        T& operator*()
        {
            return *p;
        }

        bool operator==(const iterator& it) const
        {
            if (&it == this)
            {
                return true;
            }

            return &it.data == &data && it.start == start && it.p == p;
        }

        bool operator!=(const iterator& it) const
        {
            return !operator==(it);
        }

        iterator& operator++()
        {
            if (p == &data[(start+N-1)%N])
            {
                p = nullptr;
            }
            else if (p == &data[N-1])
            {
                p = &data[0];
            }
            else
            {
                ++p;
            }
            return *this;
        }
        iterator& operator++(int)
        {
            if (p == &data[(start+N-1)%N])
            {
                p = nullptr;
            }
            else if (p == &data[N-1])
            {
                p = &data[0];
            }
            else
            {
                ++p;
            }
            return *this;
        }
    };
    
    iterator begin() 
    {
        return iterator(data, start, &data[start]);
    }

    iterator end()
    {
        return iterator(data, start, nullptr);
    }

    /*
    const_iterator begin() const
    {
        return const_iterator(data, start, &data[start]);
    }

    const_iterator end() const
    {
        return iterator(data, start, nullptr);
    }
    */

    array<T, N> data;
    size_t start;
};



int main(int argc, char* argv[])
{
    CircularArray<int, 10> ca;
    cout << ca.size() << endl;
    cout << "-----------------------" << endl;
    for (size_t i = 0; i < ca.size(); ++i)
    {
        ca[i] = i+10;
        cout << ca[i] << endl;
    }

    ca.rotate(3);
    cout << "------------------------" << endl;
    for (size_t i = 0; i < ca.size(); ++i)
    {
        cout << ca[i] << endl;
    }

    
    ca.rotate(3);
    cout << "========================" << endl;
    for (CircularArray<int, 10>::iterator it = ca.begin(); // 16
            it != ca.end(); ++it)
    {
        cout << *it << endl;
    }
    
    ca.rotate(2);
    cout << "_________________________" << endl;
    for (auto x : ca)
    {
        cout << x << endl;
    }
    

    return 0;
}
