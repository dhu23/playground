#ifndef INCLUDED_DHG_CHARACTER_H
#define INCLUDED_DHG_CHARACTER_H

#include "util.h"
#include "effect.h"
#include <string>
#include <vector>

#include <tr1/memory>


namespace dhg{

class Role;
typedef std::tr1::shared_ptr<Role> RolePtr;

// class Attribute;
// typedef std::tr1::shared_ptr<Attribute> AttributePtr;

// character class describes the underlying mechanism. 
// The main responsibility of the class is to 
// 1) generate attribute combined with external enhancement
// 2) use skills

class Character
{
    public:
        Character(const std::string& name, const RolePtr& pRole);
        /*
        Character(
                const std::string& name, 
                const RolePtr& pRole,
                const AdditiveAttribute& gearAttribute, 
                const AdditiveAttribute& paragonPoints);
        */
        ~Character() {}

        const std::string& name() const {return m_name;}
        RolePtr role() {return m_pRole;}


        void takeEffect(const Effect& effect)
        {
            boost::apply_visitor(EffectVisitor(*this), effect.m_effect);
        }

        void onEndOfRound();

        int hitPoints() const {return m_hitPoints;}
        bool isAlive() const;

        void reportStats() const;

    private:

        class EffectVisitor : public boost::static_visitor<>
        {
            public:
                EffectVisitor(Character& character):m_character(character) {}
            
                void operator()(
                        const std::tr1::shared_ptr<Damage>& pDamage) const;
                void operator()(
                        const std::tr1::shared_ptr<DamageOverTime>& pDot) const;
                void operator()(
                        const std::tr1::shared_ptr<Buff>& pBuff) const;
            private:
                Character& m_character;
        };


        std::string m_name;
        RolePtr m_pRole;

        std::vector<std::tr1::shared_ptr<DamageOverTime> > m_dots;
        std::vector<std::tr1::shared_ptr<Buff> > m_buffs;
        std::vector<std::tr1::shared_ptr<Buff> > m_debuffs;
 
        int m_hitPoints;
};

} // end of namespace dhg

#endif
