#ifndef _INCLUDED_EFFECT_H
#define _INCLUDED_EFFECT_H

#include <iostream>
#include <set>

struct TurnEffect
{
    mutable int remainingTurns;

    bool active() const { return remainingTurns > 0; }
    void turn() const { --remainingTurns; }
};

struct RankedEffect
{
    int order;
};

template<typename T>
struct EffectRanker
{
    bool operator()(const T& lhs, const T& rhs) const
    {
        return lhs.order < rhs.order;
    }
};


class Character;

enum class BuffName
{
    BaneOfTheTrapped, // 50% extra dmg against cc'ed target
    Renew // again 5 hp each round for 5 rounds
};
struct Buff : public TurnEffect, public RankedEffect
{
    static Buff create(BuffName name, int turns);

    BuffName name;
    
    void onTurn(Character* pChar) const;
};

std::ostream& operator<<(std::ostream& os, const Buff& rhs);

enum class DebuffName
{
    CurseOfRust, // armor reduced by 50%
    CurseOfAgony // lose 15 life each round for 3 round
};
struct Debuff : public TurnEffect, public RankedEffect
{
    static Debuff create(DebuffName name, int turns);

    DebuffName name;
    
    void onTurn(Character* pChar) const;
};
std::ostream& operator<<(std::ostream& os, const Debuff& rhs);

using BuffRanker = EffectRanker<Buff>;
using DebuffRanker = EffectRanker<Debuff>;

using BuffSet = std::set<Buff, BuffRanker>;
using DebuffSet = std::set<Debuff, DebuffRanker>;

#endif
