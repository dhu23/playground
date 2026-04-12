// CTC technical interview questions
// the idea is to use strong-typed system wrapping around efficient 
// low-level implementation, similar to new type in Haskell

// A naive way is to use typedef or using to create alias
// but you could still mix up different types based on the same low level type

// each ID type, ProductId, OrderId, etc would only contain 1 field for id
// The idea is to use template instead of inheritance

// U - underlying data type
// T - a unique identifier so two classes based off int can be different

#include <iostream>


template<typename U, typename T>
class Handle {
    U id_;
public:
    explicit Handle(U id):id_(id) {} // no implicit conversion
    U id() const { return id_; }
    
    bool operator<(const Handle<U, T>& other) const {
        return id_ < other.id_;
    }

    bool operator==(const Handle<U, T>& other) const {
        return id_ == other.id_;
    }
};


using OrderId = Handle<short, struct order_id_type>;
using ProductId = Handle<short, struct product_id_type>;
using UserId = Handle<int, struct user_id_type>;


void process(OrderId order, ProductId product, UserId user) {
    std::cout << "processing order(" << order.id() 
        << "), product(" << product.id() << "), user("
        << user.id() << ')'
        << std::endl;
}

// clang++ -std=c++20 id_type.m.cpp -o id_type.tsk
int main(int argc, char* argv[]) {
    OrderId order{1};
    ProductId product{1111};
    UserId user{4151361};

    // process(product, order, user);
    process(order, product, user);

    return 0;
}