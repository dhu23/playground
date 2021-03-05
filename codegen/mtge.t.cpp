#include "mtge_shift.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    ParallelMethod pm;
    
    WaveMethod wm;
    wm.left() = 1;
    wm.right() = 5;
    wm.upSlope() = true;

    TentMethod tm;
    tm.left() = 1;
    tm.mid() = 3;
    tm.right() = 10;

    ShiftMethod sm;
    if (sm.isParallelMethod())
    {
        cout << "is parallel method" << endl;
    }

    WaveMethod& w = sm.makeWaveMethod();
    w = wm;

    if (sm.isWaveMethod())
    {
        cout << "is Wave method" << endl;
        cout << sm.waveMethod().left() << endl;
        cout << sm.waveMethod().right() << endl;
        cout << sm.waveMethod().upSlope() << endl;
    }

    sm.reset();
    if (sm.isWaveMethod())
    {
        cout << "is Wave method" << endl;
    }
    else
    {
        cout << "no longer Wave method" << endl;
    }

    sm.makeTentMethod() = tm;
    if (sm.isTentMethod())
    {
        cout << "is tent method" << endl;
        cout << sm.tentMethod().left() << endl;
        cout << sm.tentMethod().mid() << endl;
        cout << sm.tentMethod().right() << endl;
    }

    return 0;
}
