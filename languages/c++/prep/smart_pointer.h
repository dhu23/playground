#ifndef SMART_POINTER_H
#define SMART_POINTER_H

// mimic unique pointer from standard library
// it owns the memory resource,
//    - acquire memory on constructor
//    - release memory on destructor
//    - transfer memory ownership on assignment
//    - no copy sematics support 
template<typename T>
class UniquePointer {
    T* pObj_;
public:
    UniquePointer();
    UniquePointer(T* ptr);
    
    UniquePointer(const UniquePointer<T>& p) = delete;
    UniquePointer<T>& operator=(const UniquePointer<T>& p) = delete;
    
    UniquePointer(UniquePointer<T>&& p);
    UniquePointer<T>& operator=(UniquePointer<T>&& p);

    ~UniquePointer();

    // returns the managed resource and set the UniquePointer state to null
    T* release();
    
    // returns the managed resource
    const T* get() const;
    T* get();

    // update the managed resource to the new resource
    void reset(T* ptr);
    
    // get the internal managed resource object
    T& operator*();
    const T& operator*() const;

    // get the internal pointer
    T* operator->() const;

    operator bool() const;

private:
    void clear();
};

template<typename T>
UniquePointer<T>::UniquePointer()
: pObj_(nullptr) {
}

template<typename T>
UniquePointer<T>::UniquePointer(T* ptr)
: pObj_(ptr) {    
}

template<typename T>
UniquePointer<T>::UniquePointer(UniquePointer<T>&& rhs)
: pObj_(rhs.pObj_) {
    rhs.pObj_ = nullptr;
}

template<typename T>
UniquePointer<T>&
UniquePointer<T>::operator=(UniquePointer<T>&& p) {
    this->clear();
    pObj_ = p.pObj_;
    p.pObj_ = nullptr;
    return *this;
}

template<typename T>
UniquePointer<T>::~UniquePointer() {
    this->clear();
}

template<typename T>
T*
UniquePointer<T>::release() {
    T* ret = this->pObj_;
    this->pObj_ = nullptr;
    return ret;
}

template <typename T> 
const T*
UniquePointer<T>::get() const {
     return this->pObj_; 
}

template<typename T>
T*
UniquePointer<T>::get() {
    return this->pObj_;
}

template<typename T>
void
UniquePointer<T>::reset(T* ptr) {
    
}

template<typename T>
T&
UniquePointer<T>::operator*() {
    return *(this->pObj_);
}

template <typename T> 
const T&
UniquePointer<T>::operator*() const {
     return *(this->pObj_); 
}

template<typename T>
T*
UniquePointer<T>::operator->() const {
    return this->pObj_;
}

template<typename T>
UniquePointer<T>::operator bool() const {
    return nullptr != this->pObj_;
}

template<typename T>
void 
UniquePointer<T>::clear() {
    if (this->pObj_) {
        delete this->pObj_;
    }
}



template<typename T>
class SharedPointer {
    int* pRefCount_;
    T* pObj_;
public:
    SharedPointer();
    SharedPointer(T* p);

    SharedPointer(const SharedPointer<T>& other);
    SharedPointer<T>& operator=(const SharedPointer<T>& other);

    SharedPointer(SharedPointer<T>&& other);
    SharedPointer<T>& operator=(SharedPointer<T>&& other);

    ~SharedPointer();

private:
    int countUp();
    int countDown();
};

template<typename T>
SharedPointer<T>::SharedPointer()
: pRefCount_(nullptr)
, pObj_(nullptr) {
}

template<typename T>
SharedPointer<T>::SharedPointer(T* p)
: pRefCount_(new int(1))
, pObj_(p) {
}

template<typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& other)
: pRefCount_(other.pRefCount_)
, pObj_(other.pObj_) {
    this->countUp();
}

template<typename T>
SharedPointer<T>&
SharedPointer<T>::operator=(const SharedPointer<T>& other) {
    // cases to consider in copy assignment:
    // 1. nullptr <- nullptr
    // 2. nullptr <- another-valid-ptr
    // 3. valid-ptr <- nullptr
    // 4. valid-ptr <- another-valid-ptr
    // 4. valid-ptr <- valid-ptr
    if (other.pObj_ != this->pObj_) {
        // if this is empty or this is different from other         
        this->countDown(); // no-op when this is empty
        this->pRefCount_ = other.pRefCount_;
        this->pObj_ = other.pObj_;
        this->countUp();
    }
    return *this;
}

template<typename T>
SharedPointer<T>::SharedPointer(SharedPointer<T>&& other)
: pRefCount_(other.pRefCount_)
, pObj_(other.pObj_) {
    other.pRefCount_ = nullptr;
    other.pObj_ = nullptr;
}

template<typename T>
SharedPointer<T>&
SharedPointer<T>::operator=(SharedPointer<T>&& other) {
    if (other.pObj_ == this->pObj_ || !this->pObj_) {
        other.pRefCount_ = nullptr;
        other.pObj_ = nullptr;
        return *this;
    }
    
}

template<typename T>
SharedPointer<T>::~SharedPointer() {
    this->countDown();
}

template<typename T>
int
SharedPointer<T>::countUp() {
    if (!this->pRefCount_) {
        this->pRefCount_ = new int(0);
    }
    return ++(*this->pRefCount_);
}

template<typename T>
int
SharedPointer<T>::countDown() {
    if (!this->pRefCount_ || 0 == *this->pRefCount_) {
        return 0;
    }
    int ret = --(*this->pRefCount_);
    if (ret == 0 && this->pObj_) {
        delete this->pObj_;
        this->pObj_ = nullptr;
    }
    return ret;
}


#endif