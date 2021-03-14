#include "character.h"

void Character::onHeal(int h)
{
    if (this->alive())
    {
        m_hp += h;
        m_hp = std::min(m_maxHp, m_hp);
    }
}

void Character::onDamage(int d)
{
    m_hp -= static_cast<int>(d * damageReduction());
    m_hp = std::max(0, m_hp);
}

void Character::onDamage(const Damage& d)
{
    this->onDamage(d.dmg);
}

void Character::onBuff(const Buff& b)
{
    this->onEffect(b, m_buffs);
}

void Character::onDebuff(const Debuff& db)
{
    this->onEffect(db, m_debuffs);
}

void Character::onTurn()
{
    this->onEffectTurn(m_buffs);
    this->onEffectTurn(m_debuffs);
}

std::ostream& Character::print(std::ostream& os) const
{
    os 
        << "Character[name=" << m_name 
        << ",hp=" << m_hp
        << "]" << std::endl;
    os << "Buff: " << std::endl;
    for (const auto& buff : m_buffs)
    {
        os << std::string(6, ' ') << buff << std::endl;
    }
    os << "Debuff: " << std::endl;
    for (const auto& debuff : m_debuffs)
    {
        os << std::string(6, ' ') << debuff << std::endl;
    }
    return os;
}
