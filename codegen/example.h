#ifndef SCHEMA1_H_
#define SCHEMA1_H_

struct Weekday 
{
    enum class Value : int 
    {
        SUN = 0 
    ,   MON = 1 
    ,   TUE = 2
    ,   WED = 3
    ,   THU = 4
    ,   FRI = 5
    ,   SAT = 6
    };

    static bool fromInt(int in, Weekday::Value& out);
    static int toInt(Weekday::Value in);

    static bool fromString(const std::string& in, Weekday::Value& out);
    static std::string toString(Weekday::Value in);
    static int size();
};

struct Month 
{
    enum class Value : int
    {
        JAN = 1
    ,   FEB = 2
    ,   MAR = 3
    ,   APR = 4
    ,   MAY = 5
    ,   JUN = 6
    ,   JUL = 7
    ,   AUG = 8
    ,   SEP = 9
    ,   OCT = 10
    ,   NOV = 11
    ,   DEC = 12
    };

    static bool fromInt(int in, Month::Value& out);
    static int toInt(Month::Value in);

    static bool fromString(const std::string& in, Month::Value& out);
    static std::string toString(Weekday::Value in);
    static size_t size();
};

class Instrument
{
    std::string instrumentName_;
    std::string exchangeCode_;

public:
    Instrument();

    std::string& instrumentName();
    const std::string& instrumentName() const;

    std::string& exchangeName();
    const std::string& exchangeName() const;

    std::ostream& print(std::ostream& os) const;

private:
};

inline
std::string& Instrument::instrumentName()
{
    return instrumentName_;
}

inline
const std::string& Instrument::instrumentName() const
{
    return instrumentName_;
}

inline 
std::string& Instrument::exchangeName()
{
    return exchangeCode_;
}

inline
const std::string& Instrument::exchangeCode() const
{
    return exchangeCode_;
}

class ParallelMethod
{
public:
    ParallelMethod();

    std::ostream& print(std::ostream& os) const;
};

class WaveMethod
{
    int left_;
    int right_;
    bool upSlope_;
public:
    WaveMethod();

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
    TentMethod();

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
    union
    {
        ParallelMethod parallelMethod_;
        WaveMethod waveMethod_;
        TentMethod tentMethod_;
    } data_;

    int type_;

public:
    ShiftMethod();

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

    void reset();
};

#endif
