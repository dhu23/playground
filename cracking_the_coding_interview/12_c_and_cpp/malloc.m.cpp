// write an aligned malloc and free function that supports allocating memory
// such that the memory address returned is divisible by a specific power of 2
// example:
// align_malloc(1000, 128) will return a memory address that is multiple of 128
// and that points to memory of size 1000 bytes. 
// aligned_free() will free memory allocated by align_malloc

#include <stdlib.h>
#include <iostream>

// say one block of memory holds base bytes(e.g. 128 bytes in this case)
// we have to wrap all in a class to track how much memory at the front end
class AlignedPointer
{
    public: 
        AlignedPointer(unsigned int size, unsigned int base):
            _base(base), _total(0), _p(nullptr), _offset(0)
        {
            int blocks = size / base + 1; 
            _total = blocks*_base + (size % base);
            _p = (char*)malloc(_total);

            while ((uintptr_t)(_p+_offset) % base != 0)
                ++_offset;
        }

        operator void* () const {return (void*)(_p+_offset);}

        void* operator->() const {return (char*)(_p+_offset);}

        void* operator+(unsigned int i) const {return (void*)(_p+_offset+i);}

        void release()
        {
            if (_p) 
            {
                free(_p);
                _p = nullptr;
            }
            _total = 0;
            _offset = 0;
        }

        std::ostream& print(std::ostream& os) const
        {
            // cout on char* will prints the content instead of address
            os 
                << "base=" << _base << ",total=" << _total 
                << ",p=" << (void*)_p << ",offset=" << _offset;
            return os;
        }

        ~AlignedPointer()
        {
            this->release();   
        }
    private:
        unsigned int _base;
        unsigned int _total;

        char* _p;
        unsigned int _offset;
};

AlignedPointer aligned_malloc_obj(unsigned int size, unsigned int base)
{
    return AlignedPointer(size, base);
}

void aligned_free_obj(AlignedPointer& ptr) 
{
    ptr.release();
}

void testAlignedObject()
{
    AlignedPointer p = aligned_malloc_obj(1000, 128);
    p.print(std::cout) << std::endl;

    char v = 'a';
    for (size_t i = 0; i < 1000; ++i)
    {
        char* pc = (char*)(p+i);
        if (i == 999) 
        {
            *pc = '\0';
        }
        else 
        {
            *pc = v;
            v = (v == 'z' ? 'a' : v+1);
        }
        std::cout << *pc;
    }
    std::cout << std::endl;
    std::cout << (char*)(p+0) << std::endl;
}

// C style
void* aligned_malloc(unsigned int size, unsigned int base)
{
    unsigned int extra = sizeof(void*);
    unsigned int required = size + extra;
    unsigned int offset = base-1;

    // this guarantees the first address multiplier of base has 
    // at least required memory after it
    unsigned int total = required + offset; 
    void* p = (void*)malloc(total);
    if (p == nullptr) return nullptr;
    void* ret = (void*)((((uintptr_t)(p) + offset + extra) & (~offset)));
    ((void**)ret)[-1] = p;
    return ret;
}

void aligned_free(void *p)
{
    void* start = ((void**)p)[-1];
    free(start);
}

void testAligned()
{
    void* p = aligned_malloc(1000, 128);
    std::cout << (uintptr_t)p << std::endl;
    void* start = ((void**)p)[-1];
    std::cout << (uintptr_t)start << std::endl;
}


int main(int argc, char* argv[])
{
    testAligned();
    return 0;
}
