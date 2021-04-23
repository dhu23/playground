#include <cstdint>

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


    std::array<uint8_t, N> buffer_; // holds the main message buffer
    std::array<uint8_t, M> tmpbuffer_; // for incomplete data
    size_t head_;
    size_t tail_; // points to the first empty byte
    size_t incompleted_; // indicates num of bytes that it is still waiting for

public:
    bool hasPartialData() const { return incomplete_ > 0; }
    bool isWrappedRound() const { return tail < head; }
    
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
        for (decltype(tail) i = 0; i < N; ++i) { buffer_[i] = 0; }
        tail = 0;
        return true;
    }

    size_t tailRoom() const
    {
        return this->isWrappedRound() ? 0 : N-tail;
    }
    
public:
    RingBuffer():
        buffer_(),
        tmpbuffer_(),
        head_(0),
        tail_(0),
        incomplete_(0)
    {}

    // this function takes 
    template<typename T> bool push(const T& obj)
    {
        // disable insertion when there is partial data. 
        // just a safety measure to prevent bugs
        if (this->hasPartialData()) { return false; }
        if (this->freeSpace() < sizeof(T)) { return false; }
        
        uint8_t start = reinterpret_cast<uint8_t>(&T);
        uint8_t end = start + sizeof(T);

        if (this->isWrappedRound())
        {

        }

    }

};
