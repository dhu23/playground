#ifndef INCLUDED_DHG_SKILL_H
#define INCLUDED_DHG_SKILL_H

#include "effect.h"
#include "attribute.h"
#include <vector>
#include <tr1/memory>

namespace dhg{

class Character;

class SkillInner
{
    public:
        SkillInner() {}

        virtual ~SkillInner();

        virtual std::vector<Effect> use(const AttributePtr& pAttribute) = 0;
    protected:
};

typedef std::tr1::shared_ptr<SkillInner> SkillPtr;

class SkillRune : public SkillInner
{
    public:
        SkillRune(SkillPtr pSkill):m_pSkill(pSkill) {}

        virtual ~SkillRune();

        virtual std::vector<Effect> use(const AttributePtr& pAttribute) = 0;

    protected:
        SkillPtr m_pSkill;
};

} // end of namespace dhg

#endif
