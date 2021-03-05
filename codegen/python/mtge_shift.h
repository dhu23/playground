#ifndef _INCLUDED_Mtge_shift_H_
#define _INCLUDED_Mtge_shift_H_
#include <iostream>
#include <string>
struct Shift
{
    enum class Value : int
    {
        Parallel = 0
    ,   Wave = 1
    ,   Tent = 2
    };
    static bool fromInt(int in, Shift::Value& out);
    static int toInt(Shift::Value in);
    static bool fromString(const std::string& in, Shift::Value& out);
    static std::string toString(Shift::Value in);
    static int size();
};
class ParallelMethod
{
public:
    ParallelMethod() {}
    std::ostream& print(std::ostream& os) const;
};
class WaveMethod
{
    int left_;
    int right_;
    bool upSlope_;
public:
    WaveMethod() {}
    int& left();
    const int& left() const;
    int& right();
    const int& right() const;
    bool& upSlope();
    const bool& upSlope() const;
    std::ostream& print(std::ostream& os) const;
};
class TentMethod
{
    int left_;
    int mid_;
    int right_;
public:
    TentMethod() {}
    int& left();
    const int& left() const;
    int& mid();
    const int& mid() const;
    int& right();
    const int& right() const;
    std::ostream& print(std::ostream& os) const;
};
class ShiftMethod
{
    union U
    {
        ParallelMethod parallelMethod_;
        WaveMethod waveMethod_;
        TentMethod tentMethod_;
        U() {new(&parallelMethod_) ParallelMethod();}
    };
    U data_;
    int type_;
public:
    ShiftMethod();
    void reset() {type_=-1;}
    bool isParallelMethod() const;
    bool isWaveMethod() const;
    bool isTentMethod() const;
    ParallelMethod& makeParallelMethod();
    WaveMethod& makeWaveMethod();
    TentMethod& makeTentMethod();
    ParallelMethod& parallelMethod();
    const ParallelMethod& parallelMethod() const;
    WaveMethod& waveMethod();
    const WaveMethod& waveMethod() const;
    TentMethod& tentMethod();
    const TentMethod& tentMethod() const;
    std::ostream& print(std::ostream& os) const;
};
//////////////////// inline functions for class WaveMethod ////////////////////

inline
int& WaveMethod::left()
{
    return left_;
}

inline
const int& WaveMethod::left() const
{
    return left_;
}
inline
int& WaveMethod::right()
{
    return right_;
}

inline
const int& WaveMethod::right() const
{
    return right_;
}
inline
bool& WaveMethod::upSlope()
{
    return upSlope_;
}

inline
const bool& WaveMethod::upSlope() const
{
    return upSlope_;
}
//////////////////// inline functions for class TentMethod ////////////////////

inline
int& TentMethod::left()
{
    return left_;
}

inline
const int& TentMethod::left() const
{
    return left_;
}
inline
int& TentMethod::mid()
{
    return mid_;
}

inline
const int& TentMethod::mid() const
{
    return mid_;
}
inline
int& TentMethod::right()
{
    return right_;
}

inline
const int& TentMethod::right() const
{
    return right_;
}
//////////////////// inline functions for class ShiftMethod ////////////////////

inline
ParallelMethod& ShiftMethod::parallelMethod()
{
    return data_.parallelMethod_;
}

inline
const ParallelMethod& ShiftMethod::parallelMethod()const
{
    return data_.parallelMethod_;
}
inline
WaveMethod& ShiftMethod::waveMethod()
{
    return data_.waveMethod_;
}

inline
const WaveMethod& ShiftMethod::waveMethod()const
{
    return data_.waveMethod_;
}
inline
TentMethod& ShiftMethod::tentMethod()
{
    return data_.tentMethod_;
}

inline
const TentMethod& ShiftMethod::tentMethod()const
{
    return data_.tentMethod_;
}
#endif