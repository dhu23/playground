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
    T* obj_;
public:
    UniquePointer();
    UniquePointer(const T& data);
    
    UniquePointer(const UniquePointer<T>& p) = delete;
    UniquePointer& operator=(const UniquePointer<T>& p) = delete;
    
    UniquePointer(UniquePointer<T>&& p);
    UniquePointer& operator=(UniquePointer<T>&& p);

    ~UniquePointer();

    // returns the managed resource and set the UniquePointer state to null
    T* release();
    
    // returns the managed resource
    T* get() const;

    // update the managed resource to the new resource
    void reset(T* ptr);
    
    // get the internal managed resource object
    T& operator*();

    // get the internal pointer
    T* operator->() const;

private:
    void clear();
};

template<typename T>
UniquePointer<T>::UniquePointer()
:obj_(nullptr) {
}

template<typename T>
UniquePointer<T>::UniquePointer(const T& data)
:obj_(new T(data)) {
}

template<typename T>
UniquePointer<T>::UniquePointer(UniquePointer<T>&& rhs)
:obj_(rhs.obj_) {
    rhs.obj_ = nullptr;
}

template<typename T>
UniquePointer<T>&
UniquePointer<T>::operator=(UniquePointer<T>&& p) {
    this->clear();
    obj_ = p.obj_;
    p.obj_ = nullptr;
}

template<typename T>
UniquePointer<T>::~UniquePointer() {
    this->clear();
}

template<typename T>
T*
UniquePointer<T>::release() {
    T* ret = this->obj_;
    this->obj_ = nullptr;
    return ret;
}

template<typename T>
T*
UniquePointer<T>::get() const {
    return this->obj_;
}

template<typename T>
void
UniquePointer<T>::reset(T* ptr) {
    
}

template<typename T>
T&
UniquePointer<T>::operator*() {
    return *(this->obj_);
}

template<typename T>
T*
UniquePointer<T>::operator->() const {
    return this->get();
}

template<typename T>
void 
UniquePointer<T>::clear() {
    if (this->obj_) {
        delete this->obj_;
    }
}



template<typename T>
class SharedPointer {

};

#endif