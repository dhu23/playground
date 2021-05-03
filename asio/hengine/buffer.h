#ifndef _INCLUDED_RINGBUFFER_H_
#define _INCLUDED_RINGBUFFER_H_

#include "packing.h"

// this is the ring buffer that holds heterogeneous objects in a C style
// template<size_t N, size_t M>
// class RingBuffer
// {
// public:
//     
//     // normal situation of the buffer usage
//     // 0-------------------------------------------------------------->|end
//     // ^ head                      ^ tail
//     // |                           |
//     // |######### Used ############|
//     //
//     // After some reading from the head 
//     // 0-------------------------------------------------------------->|end
//     //          ^ head                                         ^ tail
//     //          |                                              |
//     //          |#################### Used ####################|
//     //
//     // if more writing is performanced and there is not enough space between
//     // the end of the tail and the end of the buffer, the buffer will wrap
//     // around to position 0 again, provided that there is enough room between
//     // 0 and head 
//     //
//     // 0-------------------------------------------------------------->|end
//     //       ^ tail       ^ head                                 ^
//     //       |            |                                      | padding
//     // ######|            |################ Used ################|00000
//     //          
//     // An alternative solution is to also move the stored data back to start 
//     // from position 0 again.
// 
//     // using std::size_t = typename std::array<uint8_t, N>::size_type;
// 
//     std::array<uint8_t, N> buffer_; // holds the main message buffer
//     std::array<uint8_t, M> tmpbuffer_; // for incomplete data
//     std::size_t head_;
//     std::size_t tail_; // points to the first empty byte
//     std::size_t incompleted_; // indicates num of bytes that it is still waiting for
//     // tracking used space because just head_ and tail_ position is not enought
//     // to tell the different between completely empty and completely full
//     bool wrapped_;
// private:
// 
//     bool isWrapped() const { return wrapped_; }
//     bool padTheRest()
//     {
//         // this should only happen when tail is after head. 
//         // not the other way around
//         if (this->isWrapped()) { return false; }
//         
//         // NOTE: std::fill is another alternative, however slower than memset
//         // because memset is written in Assembly
//         memset(&buffer_[tail_], 0, N-tail_);
//         tail_ = 0;
//         wrapped_ = true;
//         return true;
//     }
// 
// public:
//     RingBuffer():
//         buffer_(),
//         tmpbuffer_(),
//         head_(0),
//         tail_(0),
//         incompleted_(0),
//         wrapped_(false)
//     {}
// 
//     bool hasPartialData() const { return incompleted_ > 0; }
//     bool isEmpty() const { return head_ == tail_ && !wrapped_; }
//     bool isFull() const { return head_ == tail_ && wrapped_; }
// 
//     size_t tailRoom() const
//     {
//         return this->isWrapped() ? head_-tail_ : N-tail_;
//     }
//     
// public:
//     template<typename T> bool push(const T& obj)
//     {
//         // disable insertion when there is partial data. 
//         // just a safety measure to prevent bugs
//         if (this->hasPartialData()) { return false; }
//         if (this->isFull()) { return false; }
//         
//         const uint8_t* pstart = reinterpret_cast<const uint8_t*>(&obj);
//         const uint8_t* pend = pstart + sizeof(T);
// 
//         if (this->tailRoom())
//         {
//                
//         }
//         else
//         {
//         }
//         return true;
//     }
// 
// };


// Buffer classes do not manage memory directly. It defines how reading and 
// writing moves the index indicators.
class Buffer
{
protected:
    std::size_t capacity_;
    std::size_t head_;
    std::size_t tail_;
public:
    Buffer(std::size_t capacity):
        capacity_(capacity),
        head_(0),
        tail_(0)
    {}

    // available space from tail to the end of the buffer
    virtual std::size_t tailSpace() const = 0;
    // available space from the tail that is available for writing
    virtual std::size_t freeSpace() const = 0;
    // used space between head and tail
    virtual std::size_t usedSpace() const = 0;

    bool isEmpty() const { return this->freeSpace() == capacity_; }
    bool isFull() const { return this->usedSpace() == capacity_; }

    virtual bool read(std::size_t size) = 0;
    virtual bool write(std::size_t size) = 0;

    std::size_t capacity() const { return capacity_; }
};

// linear buffer reads and writes linearly. 
class LinearBuffer : public Buffer
{
public:
    LinearBuffer(std::size_t capacity):
        Buffer(capacity)
    {}

    std::size_t tailSpace() const override
    {
        return capacity_-tail_;
    }
    std::size_t freeSpace() const override
    {
        return this->tailSpace();
    }
    std::size_t usedSpace() const override
    {
        return tail_-head_;
    }

    bool read(std::size_t size) override
    {
        if (this->usedSpace() < size) { return false; }
        head_ += size;
        // resetting indices if both are the same
        if (head_ == tail_)
        {
            head_ = 0;
            tail_ = 0;
        }
        return true;
    }

    bool write(std::size_t size) override
    {
        if (this->tailSpace() < size) { return false; }
        tail_ += size;
        return true;
    }
};

// ring buffer reads and writes wrapping around
class RingBuffer : public Buffer
{
    bool wrapped_; // indicator of wrapping in the buffer when head==tail
public:
    RingBuffer(std::size_t capacity):
        Buffer(capacity),
        wrapped_(false)
    {}

    std::size_t tailSpace() const override
    {
        return wrapped_ ? head_-tail_ : capacity_-tail_;
    }
    std::size_t freeSpace() const override
    {
        return wrapped_ ? head_-tail_ : this->capacity() - (tail_-head_);
    }
    std::size_t usedSpace() const override
    {
        return this->capacity() - this->freeSpace();
    }

    bool read(std::size_t size) override
    {
        if (size > this->usedSpace()) { return false; }
        auto head = head_+size;
        if (head >= this->capacity()) 
        {
            head_ = head-this->capacity();
            wrapped_ = !wrapped_;
        }
        else
        {
            head_ = head;
        }
        return true;
    }

    bool write(std::size_t size) override
    {
        if (size > this->freeSpace()) { return false; }
        auto tail = tail_+size;
        if (tail >= this->capacity())
        {
            tail_ = tail-this->capacity();
            wrapped_ = !wrapped_;
        }
        else
        {
            tail_ = tail;
        }
        return true;
    }

};


#endif
