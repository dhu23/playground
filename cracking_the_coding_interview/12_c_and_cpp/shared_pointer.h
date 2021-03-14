// implementation of shared pointer

#include <cassert>
#include <iostream>

template<typename T>
class SharedPointer
{
    public:
        SharedPointer(): _pData(nullptr), _pRefCount(new unsigned short(0)) {}
        SharedPointer(T* p): _pData(p), _pRefCount(new unsigned short(1)) {}

        SharedPointer(const SharedPointer<T>& other):
            _pData(nullptr), _pRefCount(nullptr)
        {
            this->acquire(other._pData, other._pRefCount);
        }

        SharedPointer<T>& operator=(const SharedPointer<T>& other)
        {
            if (this == &other) return *this;
            // assigning from one pointer to the resource to another pointer
            if (this->_pData == other._pData) 
            {
                assert(this->_pRefCount == other._pRefCount);
            }
            // assigning from a different resource, overwriting the current
            this->release();
            this->acquire(other._pData, other._pRefCount);
            return *this;
        }

        T& operator*() const {return *_pData;}
        T* operator->() const {return _pData;}
        T* get() const {return _pData;}
        explicit operator bool() const {return _pData != nullptr;}

        std::ostream& print(std::ostream& os) const
        {
            os 
                << "_pData=" << _pData 
                << ",_pRefCount=" << _pRefCount 
                << ",count=" << *_pRefCount;
            return os;
        }

        ~SharedPointer() 
        {
            this->release();
        }

    private:
        void acquire(T* pData, unsigned short* pRefCount)
        {
            _pData = pData;
            _pRefCount = pRefCount;
            ++(*_pRefCount);
        }
        void release() 
        {
            if (*_pRefCount == 0) return;
            // ref count is greater than 0
            --(*_pRefCount);
            if (*_pRefCount == 0)
            {
                delete _pRefCount;
                delete _pData;
            }
        }

        T* _pData;
        unsigned short* _pRefCount;
};
