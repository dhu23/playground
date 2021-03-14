#include "character.h"
#include "role.h"
#include "skill.h"
#include "attribute.h"
#include "util.h"

#include <iostream>
#include <algorithm>

namespace dhg{

using std::vector;
using std::string;

namespace{

/*
AttributePtr applyBuffsToAttribute(
        const vector<Buff>& buffs, AttributePtr pUnbuffedAttribute)
{
    AdditiveAttribute totalAdditive;
    MultiplicativeAttribute totalMultiplicative;

    for(vector<Buff>::const_iterator itr = buffs.begin();
            buffs.end() != itr; ++itr)
    {
        totalAdditive += itr->m_additiveAttribute;
        totalMultiplicative *= itr->m_multiplicativeAttribute;
    }


    return AttributePtr(new AttributeEnhancedByMultiplicative(        
                AttributePtr(new AttributeEnhancedByAdditive(
                        pUnbuffedAttribute, totalAdditive)),         
                totalMultiplicative));
}
*/

} // end of anonymous namespace 

Character::Character(const string& name, const RolePtr& pRole):
    m_name(name), 
    m_pRole(pRole),
    m_dots(), 
    m_buffs(),
    m_debuffs(),
    m_hitPoints(100)
{
}

/*
Character::Character(
        const string& name,
        const RolePtr& pRole, 
        const AdditiveAttribute& gearAttribute, 
        const AdditiveAttribute& paragonPoints):
    m_name(name),
    m_pRole(pRole),
    m_dots(),
    m_buffs(),
    m_debuffs(),
    m_hitPoints(100)
{
    // re-evaluate m_pFinalAttribute and m_hitPoints
    // this->calculateFinalAttribute();
    // m_hitPoints = pRole->getHitPoints(*m_pFinalAttribute);
}
*/

void Character::EffectVisitor::operator()(
        const std::tr1::shared_ptr<Damage>& pDamage) const
{
    if(!m_character.isAlive())
    {
        return;
    }
    int damageTaken = std::min(
            m_character.m_hitPoints, 
            static_cast<int>(pDamage->m_damage));
    std::cout << "dmg taken = " << damageTaken << std::endl;
    m_character.m_hitPoints -= damageTaken;
}

void Character::EffectVisitor::operator()(
        const std::tr1::shared_ptr<DamageOverTime>& pDot) const
{
    m_character.m_dots.push_back(pDot);
}

void Character::EffectVisitor::operator()(
        const std::tr1::shared_ptr<Buff>& pBuff) const
{
    switch(pBuff->m_buffType)
    {
        case BuffType::POSITIVE:
            m_character.m_buffs.push_back(pBuff);
        case BuffType::NEGATIVE:
            m_character.m_debuffs.push_back(pBuff);
        default:
            throw std::runtime_error("Unknown buff type");
    }
}

void Character::onEndOfRound()
{
    if(!this->isAlive())
    {
        return;
    }

    /*
    for(vector<DamageOverTime>::iterator itr = m_dots.begin();
            m_dots.end() != itr; ++itr)
    {
        this->takeDamage(itr->m_tick);
        itr->onEndOfRound();
    }
    m_dots.erase(
            std::remove_if(
                m_dots.begin(), m_dots.end(), 
                [](DamageOverTime x){return x.isExpired();}),
            m_dots.end());
                

    for(vector<Buff>::iterator itr = m_buffs.begin();
            m_buffs.end() != itr; ++itr)
    {
        itr->onEndOfRound();
    }
    m_buffs.erase(
            std::remove_if(
                m_buffs.begin(), m_buffs.end(),
                [](Buff& x){return x.isExpired();}),
            m_buffs.end());
    */
}

/*
void Character::setGearAttribute(const AdditiveAttribute& gearAttribute)
{
    m_gearAttribute = gearAttribute;
    this->calculateFinalAttribute();
}

void Character::setParagonPoints(const AdditiveAttribute& paragonPoints)
{
    m_paragonPoints = paragonPoints;
    this->calculateFinalAttribute();
}
*/

bool Character::isAlive() const
{
    return m_hitPoints > 0;
}

void Character::reportStats() const
{
    /*
    std::cout
        << "---------- " << m_name << " stat summary ----------" << std::endl
        << " dmg:" << m_pFinalAttribute->getWeaponDamage()
        << " str:" << m_pFinalAttribute->getStrength() 
        << " dex:" << m_pFinalAttribute->getDexterity() 
        << " int:" << m_pFinalAttribute->getIntelligence()
        << " vit:" << m_pFinalAttribute->getVitality()
        << " hp:" << this->hitPoints()
        << " armor:" << m_pFinalAttribute->getArmor()
        << std::endl;
    */
}

/*
void Character::calculateFinalAttribute()
{
    AdditiveAttribute external = m_gearAttribute+m_paragonPoints;
    m_pFinalAttribute = AttributePtr(
            new AttributeEnhancedByAdditive(m_pRole->attribute(), external));
}
*/

} // end of namespace dhg
