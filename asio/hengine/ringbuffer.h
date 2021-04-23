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

    // using std::size_t = typename std::array<uint8_t, N>::size_type;

    std::array<uint8_t, N> buffer_; // holds the main message buffer
    std::array<uint8_t, M> tmpbuffer_; // for incomplete data
    std::size_t head_;
    std::size_t tail_; // points to the first empty byte
    std::size_t incompleted_; // indicates num of bytes that it is still waiting for
    // tracking used space because just head_ and tail_ position is not enought
    // to tell the different between completely empty and completely full
    bool wrapped_;
private:

    bool isWrapped() const { return wrapped_; }
    bool padTheRest()
    {
        // this should only happen when tail is after head. 
        // not the other way around
        if (this->isWrapped()) { return false; }
        
        // NOTE: std::fill is another alternative, however slower than memset
        // because memset is written in Assembly
        memset(&buffer_[tail_], 0, N-tail_);
        tail_ = 0;
        wrapped_ = true;
        return true;
    }

public:
    RingBuffer():
        buffer_(),
        tmpbuffer_(),
        head_(0),
        tail_(0),
        incompleted_(0),
        wrapped_(false)
    {}

    bool hasPartialData() const { return incompleted_ > 0; }
    bool isEmpty() const { return head_ == tail_ && !wrapped_; }
    bool isFull() const { return head_ == tail_ && wrapped_; }

    size_t tailRoom() const
    {
        return this->isWrapped() ? head_-tail_ : N-tail_;
    }
    
public:
    template<typename T> bool push(const T& obj)
    {
        // disable insertion when there is partial data. 
        // just a safety measure to prevent bugs
        if (this->hasPartialData()) { return false; }
        if (this->isFull()) { return false; }
        
        const uint8_t* pstart = reinterpret_cast<const uint8_t*>(&obj);
        const uint8_t* pend = pstart + sizeof(T);

        if (this->tailRoom())
        {
               
        }
        else
        {
        }
        return true;
    }

};

#endif
