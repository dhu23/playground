#ifndef INCLUDED_DHG_UTIL_H
#define INCLUDED_DHG_UTIL_H


namespace dhg{

enum class ElementSchool : char
{
    PHYSICAL, FIRE, COLD, LIGHTNING, ARCANE, POISON, HOLY
};

enum class ResourceType : char
{
    ARCANE_POWER, RAGE, WRATH, HATRED, DISCIPLINE, SPIRIT, MANA
};
    
// this may not be necessary
enum class BuffType : char { POSITIVE, NEGATIVE };


enum class StatType : char
{
    STRENGTH, DEXTERITY, INTELLIGENCE, VITALITY, ARMOR, WEAPON_DAMAGE,
    DODGE, BLOCK, BLOCK_AMOUNT, LIFE_BONUS, 
    LIFE_PER_ROUND, LIFE_PER_HIT, LIFE_PER_KILL
};

struct Resource
{  
    Resource(const ResourceType type, const int amount):
        m_resourceType(type),
        m_amount(amount)
    {
    }

    const ResourceType m_resourceType;
    int m_amount;
};



enum class ClusterArrowRune : char 
{
    LOAD_FOR_BEAR,
    MAELSTROM
};

enum class MultishotRune : char
{
    FIRE_AT_WILL,
    ARSENAL
};

enum class ShadowPowerRune : char
{
    BLOOD_MOON,
    GLOOM
};

enum class WarCryRune : char
{
    INVIGORATE,
    IMPUNITY
};


} // end of namespace dhg

#endif
