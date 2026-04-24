// SIG interview question

// imagine you are implementing the STL container 'vector' for any element
// type 'T'. 

// 1. propose and initialize any necessary data members
// 2. implement the push_back method, taking care to properly manage all resources
// 3. push_back implementation should provide the same guarantees made by std::vector::push_back

#include <iostream>

template <typename T> class vector {
public:
    vector(size_t capacity = 0)
    : pdata_(nullptr), capacity_(capacity), size_(0) {
        // pdata_ = static_cast<T*>(std::malloc(sizeof(T) * capacity_));
        pdata_ = new T[capacity_];
    }
    ~vector() {
        delete[] pdata_; 
    }
    // vector(const vector<T>& other) {}
    void push_back(const T& t) {
        // check if it is full
        if (size_ == capacity_) {
            // allocate new memory and copy things over
            int newcap = capacity_ == 0 ? 1 : capacity_ * 2;
            T* pAllocate = new T[newcap]; // throws mem alloc, default T()

            try {
                for (int i = 0; i < size_; ++i) {
                    // placement new
                    *(pAllocate + i) = *(pdata_ + i); //// T needs operator=() can also throw
                }
            } catch (...) {
                delete[] pAllocate;
                throw;
            }

            // making mod to the obj
            delete[] pdata_;
            pdata_ = pAllocate;
            capacity_ = newcap;
        }
        T* pavailable = pdata_ + size_;
        *pavailable = t; // this can throw
        ++size_;
    }

    std::ostream& print(std::ostream& os) const {
        os << '[';
        for (int i = 0; i < size_; ++i) {
            os << *(pdata_ + i) << ',';
        }
        os << ']';
        return os;
    }

  private:
    T* pdata_;
    int capacity_;
    int size_;
};

int main() {
    vector<int> v;

    // 1->2 2
    // 2->4 4
    // 4->8 8

    // 1 2, 4,..64 for allocation, T(), copy=  64*2
    // push-back from t, so copy assignment  64

    for (int i = 0; i < 100; ++i) {
        v.push_back(i);
    }
    v.print(std::cout) << std::endl;
    return 0;
}