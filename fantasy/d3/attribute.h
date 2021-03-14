#ifndef INCLUDED_DHG_ATTRIBUTE_H
#define INCLUDED_DHG_ATTRIBUTE_H

#include "util.h"

#include <map>
#include <vector>
#include <tr1/memory>
#include <iostream>

namespace dhg{


struct Stat
{
    StatType m_statType;
    int m_amount;
    
    int getStat(const StatType type) const
    {
        if(m_statType == type)
        {
            return m_amount;
        }
        else 
        {
            return 0.0;
        };
    }
};



struct ItemAttributeInfo
{
    std::vector<Stat> m_itemStats;

    int getStat(const StatType type) const
    {
        // TODO use STL algorithm to replace the explicit loop
        int ret = 0;
        for(std::vector<Stat>::const_iterator itr = m_itemStats.begin();
                m_itemStats.end() != itr; ++itr)
        {
            ret += itr->getStat(type);
        }
        return ret;
    }
};


struct AttributeInfo
{
    AttributeInfo();
    AttributeInfo(const AttributeInfo& aa) = default;

    int m_weaponDamage;

    int m_strength;
    int m_dexterity;
    int m_intelligence;
    int m_vitality;

    int m_armor;
    std::map<ElementSchool, int> m_resistances;

    AttributeInfo& setWeaponDamage(const int weaponDamage);

    AttributeInfo& setStrength(const int strength);
    AttributeInfo& setDexterity(const int dexterity);
    AttributeInfo& setIntelligence(const int intelligence);
    AttributeInfo& setVitality(const int vitality);
    
    AttributeInfo& setArmor(const int armor);
    AttributeInfo& setResistance(
            const ElementSchool element, const int resistance);

    int getResistance(const ElementSchool element) const;

    // AttributeInfo& operator+=(const AttributeInfo& aa);
};


// AttributeInfo operator+(const AttributeInfo& lhs, const AttributeInfo& rhs);


class AttributeInner
{
    public:
        AttributeInner();
        virtual ~AttributeInner();

        virtual int getWeaponDamage() const = 0;
        virtual int getStrength() const = 0;
        virtual int getDexterity() const = 0;
        virtual int getIntelligence() const = 0;
        virtual int getVitality() const = 0;


        std::ostream& print(std::ostream& os) const
        {
            os 
                << "wd=" << this->getWeaponDamage() 
                << "|str=" << this->getStrength()
                << "|dex=" << this->getDexterity() 
                << "|int=" << this->getIntelligence()
                << "|vit=" << this->getVitality();
            return os;
        }

    private:
};

typedef std::tr1::shared_ptr<AttributeInner> AttributePtr;

class Attribute : public AttributeInner
{
    public:
        Attribute(const AttributeInfo& attributeInfo):
            m_attributeInfo(attributeInfo)
        {
        }

        int getWeaponDamage() const {return m_attributeInfo.m_weaponDamage;}
        int getStrength() const {return m_attributeInfo.m_strength;}
        int getDexterity() const {return m_attributeInfo.m_dexterity;}
        int getIntelligence() const {return m_attributeInfo.m_intelligence;}
        int getVitality() const {return m_attributeInfo.m_vitality;}

    private:
        AttributeInfo m_attributeInfo;
        
};

class AttributeEnhanced: public AttributeInner
{
    public:
        AttributeEnhanced(AttributePtr pAttribute):m_pAttribute(pAttribute) {}

        virtual ~AttributeEnhanced();

        virtual int getWeaponDamage() const = 0;
        virtual int getStrength() const = 0;
        virtual int getDexterity() const = 0;
        virtual int getIntelligence() const = 0;
        virtual int getVitality() const = 0;

    protected:
        AttributePtr m_pAttribute;
};

class AttributeEnhancedByItem : public AttributeEnhanced
{
    public:
        AttributeEnhancedByItem(
                AttributePtr pAttribute, const ItemAttributeInfo& item);

        int getWeaponDamage() const;
        int getStrength() const;
        int getDexterity() const;
        int getIntelligence() const;
        int getVitality() const;

    private:
        ItemAttributeInfo m_item;
};

} // end of namespace dhg

#endif
