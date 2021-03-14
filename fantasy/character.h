#ifndef _INCLUDED_CHARACTER_H
#define _INCLUDED_CHARACTER_H

#include "types.h"
#include "effect.h"
#include <string>
#include <iostream>

class Character
{
private:
    std::string m_name;

    // PrimaryStats m_pstats;
    int m_hp;
    int m_maxHp;

    BuffSet m_buffs;
    DebuffSet m_debuffs;

public:

    Character(const std::string& name):
        m_name(name),
        m_hp(100),
        m_maxHp(100),
        m_buffs(),
        m_debuffs()
    {
    }

    bool alive() const {return m_hp > 0;}

    void onHeal(int h);

    void onDamage(int d);
    void onDamage(const Damage& d);
    void onBuff(const Buff& b);
    void onDebuff(const Debuff& db);
    void onTurn();

    std::ostream& print(std::ostream& os) const;

private:
    float damageReduction() const {return 1.0;}

    template<typename T> void onEffectTurn(std::set<T, EffectRanker<T>>& ts)
    {
        typename std::set<T, EffectRanker<T>>::iterator it = ts.begin();

        while (it != ts.end())
        {
            it->onTurn(this);
            if (it->active()) 
            {
                ++it;
            }
            else
            {
                it = ts.erase(it);
            }
        }
    }

    template<typename T> void onEffect(
        const T& t, std::set<T, EffectRanker<T>>& ts)
    {
        auto it = ts.begin();
        while (it != ts.end())
        {
            if (t.name == it->name)
            {
                it->remainingTurns = 
                    std::max(t.remainingTurns, it->remainingTurns);
                return;
            }
        }
        ts.insert(t);
    }
};

#endif
