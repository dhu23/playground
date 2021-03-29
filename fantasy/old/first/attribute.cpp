#include "attribute.h"
#include <stdexcept>

namespace dhg{

using std::map;

namespace{

double getMultiplier(const double multiplier1, const double multiplier2)
{
    return (1+multiplier1)*(1+multiplier2)-1;
}

} // end of anonymous namespace

AttributeInfo::AttributeInfo():
    m_weaponDamage(0),
    m_strength(0),
    m_dexterity(0),
    m_intelligence(0),
    m_vitality(0),
    m_armor(0),
    m_resistances{
        {ElementSchool::PHYSICAL, 0},
        {ElementSchool::FIRE, 0},
        {ElementSchool::COLD, 0},
        {ElementSchool::LIGHTNING, 0},
        {ElementSchool::ARCANE, 0}, 
        {ElementSchool::POISON, 0},
        {ElementSchool::HOLY, 0}
    }
{
}

AttributeInfo& AttributeInfo::setWeaponDamage(const int weaponDamage)
{
    m_weaponDamage = weaponDamage;
    return *this;
}

AttributeInfo& AttributeInfo::setStrength(const int strength)
{
    m_strength = strength;
    return *this;
}

AttributeInfo& AttributeInfo::setDexterity(const int dexterity)
{
    m_dexterity = dexterity;
    return *this;
}

AttributeInfo& AttributeInfo::setIntelligence(const int intelligence)
{
    m_intelligence = intelligence;
    return *this;
}

AttributeInfo& AttributeInfo::setVitality(const int vitality)
{
    m_vitality = vitality;
    return *this;
}

AttributeInfo& AttributeInfo::setArmor(const int armor)
{
    m_armor = armor;
    return *this;
}

AttributeInfo& AttributeInfo::setResistance(
        const ElementSchool element, const int resistance)
{
    m_resistances[element] = resistance;
    return *this;
}

int AttributeInfo::getResistance(const ElementSchool element) const
{
    map<ElementSchool, int>::const_iterator itr = m_resistances.find(element);
    if(itr == m_resistances.end())
    {
        throw std::runtime_error("element NOT recognized");
    }
    return itr->second;    
}

// class AttributeInner
AttributeInner::AttributeInner()
{
}

AttributeInner::~AttributeInner()
{
}

// class AttributeEnhanced
AttributeEnhanced::~AttributeEnhanced()
{
}

// class AttributeEnhancerByItem
AttributeEnhancedByItem::AttributeEnhancedByItem(
        AttributePtr pAttribute, const ItemAttributeInfo& item):
    AttributeEnhanced(pAttribute), 
    m_item(item)
{
}

int AttributeEnhancedByItem::getWeaponDamage() const
{
    return m_pAttribute->getWeaponDamage();
}

int AttributeEnhancedByItem::getStrength() const
{
    return m_pAttribute->getStrength() + m_item.getStat(StatType::STRENGTH);
}

int AttributeEnhancedByItem::getDexterity() const
{
    return m_pAttribute->getDexterity() + m_item.getStat(StatType::DEXTERITY);
}

int AttributeEnhancedByItem::getIntelligence() const
{
    return m_pAttribute->getIntelligence() + 
        m_item.getStat(StatType::INTELLIGENCE);
}

int AttributeEnhancedByItem::getVitality() const
{
    return m_pAttribute->getVitality() + m_item.getStat(StatType::VITALITY);
}

} // end of namespace dhg
