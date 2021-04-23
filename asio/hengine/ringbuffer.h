#ifndef _INCLUDED_RINGBUFFER_H_
#define _INCLUDED_RINGBUFFER_H_

#include <cstdint>
#include <array>

template<typename T>
struct Packet
{
    uint16_t size;
    T data;
};


// this is the ring buffer that holds heterogeneous objects in a C style
template<size_t N, size_t M>
class RingBuffer
{
public:
    
    // normal situation of the buffer usage
    // 0-------------------------------------------------------------->|end
    // ^ head                      ^ tail
    // |                           |
    // |######### Used ############|
    //
    // After some reading from the head 
    // 0-------------------------------------------------------------->|end
    //          ^ head                                         ^ tail
    //          |                                              |
    //          |#################### Used ####################|
    //
    // if more writing is performanced and there is not enough space between
    // the end of the tail and the end of the buffer, the buffer will wrap
    // around to position 0 again, provided that there is enough room between
    // 0 and head 
    //
    // 0-------------------------------------------------------------->|end
    //       ^ tail       ^ head                                 ^
    //       |            |                                      | padding
    // ######|            |################ Used ################|00000
    //          
    // An alternative solution is to also move the stored data back to start 
    // from position 0 again.

    using RbSize = typename std::array<uint8_t, N>::size_type;

    std::array<uint8_t, N> buffer_; // holds the main message buffer
    std::array<uint8_t, M> tmpbuffer_; // for incomplete data
    RbSize head_;
    RbSize tail_; // points to the first empty byte
    RbSize incompleted_; // indicates num of bytes that it is still waiting for
    // tracking used space because just head_ and tail_ position is not enought
    // to tell the different between completely empty and completely full
    RbSize used_; 

public:
    bool hasPartialData() const { return incompleted_ > 0; }
    bool isEmpty() const { return used_ == 0; }
    bool isFull() const { return used_ == N; }

public:
    bool isWrappedRound() const 
    { 
        return 
            tail_ < head_ || 
            (tail_ == head_ && used_ > 0) // complete full
            ; 
    }
    
    // size_t freeSpace() const
    // {
    //     return this->isWrappedRound() ? head-tail : N-(tail-head);
    // }

    bool padTheRest()
    {
        // this should only happen when tail is after head. 
        // not the other way around
        if (this->isWrappedRound()) { return false; }
        
        // NOTE: consider memset if a performance optimization is needed
        // because memset is written in Assembly. 
        // NOTE: std::fill is another alternative, however slower than memset
        for (size_t i = tail_; i < N; ++i) { buffer_[i] = 0; }
        used_ += N-tail_;
        tail_ = 0;
        return true;
    }

    size_t tailRoom() const
    {
        return this->isWrappedRound() ? head_-tail_ : N-tail_;
    }
    
public:
    RingBuffer():
        buffer_(),
        tmpbuffer_(),
        head_(0),
        tail_(0),
        incompleted_(0),
        used_(0)
    {}

    // this function takes 
    template<typename T> bool push(const T& obj)
    {
        // disable insertion when there is partial data. 
        // just a safety measure to prevent bugs
        if (this->hasPartialData()) { return false; }
        // if (this->freeSpace() < sizeof(T)) { return false; }
        
        const uint8_t* pstart = reinterpret_cast<const uint8_t*>(&obj);
        const uint8_t* pend = pstart + sizeof(T);

        if (this->isWrappedRound())
        {

        }
        return true;
    }

};

#endif
