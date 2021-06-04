#ifndef _INCLUDED_RINGBUFFER_H_
#define _INCLUDED_RINGBUFFER_H_

#include <cstdint>
// BufferIdx classes do not manage memory directly. It defines how reading and 
// writing moves the index indicators.
class BufferIdx
{
protected:
    std::size_t capacity_;
    std::size_t head_;
    std::size_t tail_;
public:
    BufferIdx(std::size_t capacity):
        capacity_(capacity),
        head_(0),
        tail_(0)
    {}

    std::size_t head() const { return head_; }
    std::size_t tail() const { return tail_; }

    // available space from tail to the end of the buffer
    virtual std::size_t tailSpace() const = 0;
    // available space from the tail that is available for writing
    virtual std::size_t freeSpace() const = 0;
    // used space between head and tail
    virtual std::size_t usedSpace() const = 0;
    // gives the next position
    virtual std::size_t next(std::size_t pos) const = 0;
    virtual bool isReadable(std::size_t pos) const = 0;
    virtual bool isWritable(std::size_t pos) const = 0;

    bool isEmpty() const { return this->freeSpace() == capacity_; }
    bool isFull() const { return this->usedSpace() == capacity_; }

    virtual bool read(std::size_t size) = 0;
    virtual bool write(std::size_t size) = 0;

    std::size_t capacity() const { return capacity_; }

    virtual bool readIn(uint8_t* target, const uint8_t* source, std::size_t len) = 0;
};

// linear buffer reads and writes linearly. 
class LinearBufferIdx : public BufferIdx
{
public:
    LinearBufferIdx(std::size_t capacity):
        BufferIdx(capacity)
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
    
    std::size_t next(std::size_t pos) const override
    {
        if (pos >= capacity_) { return capacity_; }
        return pos+1;
    }
    bool isReadable(std::size_t pos) const override
    {
        return pos >= head_ && pos < tail_;
    }
    bool isWritable(std::size_t pos) const override
    {
        return pos < capacity_ && pos >= tail_;
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

    bool readIn(uint8_t* target, const uint8_t* source, std::size_t len)
    {
        if (this->freeSpace() < len) { return false; }
        uint8_t* pos = target + this->tail();
        memcpy(pos, source, len);
        this->write(len);
        return true;
    }
};

// ring buffer reads and writes wrapping around
class RingBufferIdx : public BufferIdx
{
    bool wrapped_; // indicator of wrapping in the buffer when head==tail
public:
    RingBufferIdx(std::size_t capacity):
        BufferIdx(capacity),
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

    std::size_t next(std::size_t pos) const override
    {
        return (pos+1) % capacity_;
    }
    bool isReadable(std::size_t pos) const override
    {
        if (pos >= capacity_) { return false; }

        if (wrapped_) { return pos >= head_ || pos < tail_; }
        else { return pos >= head_ && pos < tail_; }
    }
    bool isWritable(std::size_t pos) const override
    {
        if (pos >= capacity_) { return false; }

        if (wrapped_) { return pos >= tail_ && pos < head_; }
        else { return pos < head_ || pos >= tail_; }
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

    bool readIn(uint8_t* target, const uint8_t* source, std::size_t len)
    {
        if (this->freeSpace() < len) { return false; }
        
        uint8_t* pos = target + this->tail();
        auto tailSpace = this->tailSpace();
        if (tailSpace >= len) 
        {
            memcpy(pos, source, len);
        }
        else
        {
            // this data is wrapped around
            memcpy(pos, source, tailSpace);
            memcpy(target, source+tailSpace, len-tailSpace);
        }
        this->write(len);
        return true;
    }
};


#endif
