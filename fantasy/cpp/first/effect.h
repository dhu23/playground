#ifndef INCLUDED_DHG_EFFECT_H
#define INCLUDED_DHG_EFFECT_H

#include "util.h"
#include "attribute.h"

#include <boost/variant.hpp>

namespace dhg{

class Character;

struct Damage
{
    Damage(const ElementSchool school, const double damage):
        m_school(school), 
        m_damage(damage)
    {
    }

    Damage(const ElementSchool school, const int damage):
        m_school(school), 
        m_damage(static_cast<double>(damage))
    {
    }

    Damage& operator*=(const double multiplier)
    {
        m_damage *= multiplier; 
        return *this;
    }

    ElementSchool m_school;
    double m_damage;
};

struct TimedEffect
{
    TimedEffect(const int duration);
    virtual ~TimedEffect();

    void onEndOfRound();
    bool isExpired() const;

    int m_duration;
};

struct DamageOverTime : public TimedEffect 
{
    DamageOverTime(const Damage tick, const int duration):
        TimedEffect(duration),
        m_tick(tick)
    {
    }

    DamageOverTime& operator*=(const double multiplier)
    {m_tick *= multiplier; return *this;}

    Damage m_tick;
};


struct Buff : public TimedEffect
{
    Buff(const int duration, const BuffType type):
        TimedEffect(duration),
        m_buffType(type)
    {
    }
    virtual ~Buff();
    
    // friend class Character;
    // virtual void applyBuff(Character& character) = 0;

    BuffType m_buffType;
};


struct Effect
{
    Effect(const std::tr1::shared_ptr<Damage>& pDamage):m_effect(pDamage) {}
    Effect(const std::tr1::shared_ptr<DamageOverTime>& pDot):m_effect(pDot) {}
    Effect(const std::tr1::shared_ptr<Buff>& pBuff):m_effect(pBuff) {}
    

    boost::variant<
        std::tr1::shared_ptr<Damage>, 
        std::tr1::shared_ptr<DamageOverTime>, 
        std::tr1::shared_ptr<Buff> > m_effect;
};


} // end of namespace dhg

#endif
