#include <iostream>
#include <string>
struct Shift
{
    enum class Value : int
    {
        Parallel = 0,
        Wave = 1,
        Tent = 2
    };
    static bool fromInt(int in,Shift::Value& out);
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
    int& left() { return left_; }
    const int& left() const { return left_; }
    int& right() { return right_; }
    const int& right() const { return right_; }
    bool& upSlope() { return upSlope_; }
    const bool& upSlope() const { return upSlope_; }
    std::ostream& print(std::ostream& os) const;
};
class TentMethod
{
    int left_;
    int mid_;
    int right_;
public:
    TentMethod() {}
    int& left() { return left_; }
    const int& left() const { return left_; }
    int& mid() { return mid_; }
    const int& mid() const { return mid_; }
    int& right() { return right_; }
    const int& right() const { return right_; }
    std::ostream& print(std::ostream& os) const;
};
class ShiftMethod
{
    union U
    {
        ParallelMethod parallelMethod_;
        WaveMethod waveMethod_;
        TentMethod tentMethod_;
        U() { new(&parallelMethod_) ParallelMethod(); }
    };
    U data_;
    int type_;
public:
    ShiftMethod();
    void reset() { type_ = -1; }
    bool isParallelMethod() const;
    bool isWaveMethod() const;
    bool isTentMethod() const;
    ParallelMethod& makeParallelMethod();
    WaveMethod& makeWaveMethod();
    TentMethod& makeTentMethod();
    ParallelMethod& parallelMethod() { return data_.parallelMethod_; }
    const ParallelMethod& parallelMethod() const { return data_.parallelMethod_; }
    WaveMethod& waveMethod() { return data_.waveMethod_; }
    const WaveMethod& waveMethod() const { return data_.waveMethod_; }
    TentMethod& tentMethod() { return data_.tentMethod_; }
    const TentMethod& tentMethod() const { return data_.tentMethod_; }
    std::ostream& print(std::ostream& os) const;
};