#include "example.h"

bool Weekday::fromInt(int in, Weekday::Value& out)
{
    switch(in)
    {
    case 0: out = SUN; return true;
    case 1: out = MON; return true;
    case 2: out = TUE; return true;
    case 3: out = WED; return true;
    case 4: out = THU; return true;
    case 5: out = FRI; return true;
    case 6: out = SAT; return true;
    default: return false;
    }
}

int Weekday::toInt(Weekday::Value in)
{
    switch(in)
    {
    case SUN: return 0;
    case MON: return 1;
    case TUE: return 2;
    case WED: return 3;
    case THU: return 4;
    case FRI: return 5;
    case SAT: return 6;
    default: return -1;
    }
}

bool Weekday::fromString(const std::string& in, Weekday::Value& out)
{
    static const std::unordered_map<std::string, Value> m
    {
        {"SUN", SUN}
    ,   {"MON", MON}
    ,   {"TUE", TUE}
    ,   {"WED", WED}
    ,   {"THU", THU}
    ,   {"FRI", FRI}
    ,   {"SAT", SAT}
    };
    auto it = m.find(in);
    if (it == m.end()) 
    {
        return false;
    }
    out = it->second;
    return true;
}

std::string Weekday::toString(Weekday::Value in)
{
    switch(in)
    {
    case SUN: return "SUN";
    case MON: return "MON";
    case TUE: return "TUE";
    case WED: return "WED";
    case THU: return "THU";
    case FRI: return "FRI";
    case SAT: return "SAT";
    default: return "";
    }
}

int Weekday::size()
{
    return 7;
}

ShiftMethod::ShiftMethod():
    data_()
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

