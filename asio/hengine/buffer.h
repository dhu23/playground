#ifndef _INCLUDED_RINGBUFFER_H_
#define _INCLUDED_RINGBUFFER_H_


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

    bool isEmpty() const { return this->freeSpace() == capacity_; }
    bool isFull() const { return this->usedSpace() == capacity_; }

    virtual bool read(std::size_t size) = 0;
    virtual bool write(std::size_t size) = 0;

    std::size_t capacity() const { return capacity_; }
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
