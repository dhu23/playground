#include "effect.h"

namespace dhg{

TimedEffect::TimedEffect(const int duration):
    m_duration(duration)
{
}

TimedEffect::~TimedEffect()
{
}

void TimedEffect::onEndOfRound()
{
    if(!this->isExpired())
    {
        --m_duration;
    }
}

bool TimedEffect::isExpired() const
{
    return 0 == m_duration;
}

Buff::~Buff()
{
}

} // end of namespace dhg
