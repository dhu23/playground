#ifndef __INCLUDED_GENERAL_UTIL_H_
#define __INCLUDED_GENERAL_UTIL_H_

template<typename T>
class Oscillator
{
    T min_;
    T step_;
    std::size_t range_;

    bool direction_;
    std::size_t pos_;

public:
    Oscillator(T tmin, T tstep, std::size_t range, bool dir=true, std::size_t pos=0):
        min_(tmin),
        step_(tstep),
        range_(range),
        direction_(dir),
        pos_(pos)
    {}

    T gen()
    {
        auto ret = pos_*step_ + min_;
        if (pos_ == range_) 
        {
            direction_ = false;
            --pos_;
        }
        else if (pos_ == 0)
        {
            direction_ = true;
            pos_ = 1;
        }
        else
        {
            pos_ = direction_ ? pos_+1 : pos_-1;
        }
        return ret;
    }
};

#endif
