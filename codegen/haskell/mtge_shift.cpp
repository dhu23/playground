#include "mtge_shift.h"
#include <unordered_map>
bool Shift::fromInt(int in, Shift::Value& out)
{
    switch(in)
    {
    case 0: out = Value::Parallel; return true;
    case 1: out = Value::Wave; return true;
    case 2: out = Value::Tent; return true;
    default: return false;
    }
}
int Shift::toInt(Shift::Value in)
{
    switch(in)
    {
    case Value::Parallel: return 0;
    case Value::Wave: return 1;
    case Value::Tent: return 2;
    default: return -1;
    }
}
bool Shift::fromString(const std::string& in, Shift::Value& out)
{
    static const std::unordered_map<std::string, Value> m
    {
        {"Parallel", Value::Parallel},
        {"Wave", Value::Wave},
        {"Tent", Value::Tent}
    };
    auto it = m.find(in);
    if (it == m.end())
    {
        return false;
    }
    out = it->second;
    return true;
}
std::string Shift::toString(Shift::Value in)
{
    switch(in)
    {
    case Value::Parallel: return "Parallel";
    case Value::Wave: return "Wave";
    case Value::Tent: return "Tent";
    default: return "";
    }
}
int Shift::size()
{
    return 3;
}
ShiftMethod::ShiftMethod():
    data_(),
    type_(-1)
{
}
bool ShiftMethod::isParallelMethod() const
{
    return type_ == 0;
}
bool ShiftMethod::isWaveMethod() const
{
    return type_ == 1;
}
bool ShiftMethod::isTentMethod() const
{
    return type_ == 2;
}
ParallelMethod& ShiftMethod::makeParallelMethod()
{
    new (&data_.parallelMethod_) ParallelMethod;
    type_ = 0;
    return data_.parallelMethod_;
}
WaveMethod& ShiftMethod::makeWaveMethod()
{
    new (&data_.waveMethod_) WaveMethod;
    type_ = 1;
    return data_.waveMethod_;
}
TentMethod& ShiftMethod::makeTentMethod()
{
    new (&data_.tentMethod_) TentMethod;
    type_ = 2;
    return data_.tentMethod_;
}