#ifndef INCLUDED_DHG_CLUSTER_ARROW_H
#define INCLUDED_DHG_CLUSTER_ARROW_H

#include "skill.h"

namespace dhg{

// original skill
class ClusterArrow : public SkillInner
{
    public:
        ClusterArrow():SkillInner() {}
        virtual ~ClusterArrow();

        virtual std::vector<Effect> use(const AttributePtr& pAttribute);

    private:
};


// enhanced by Runes
class LoadedForBear : public SkillRune
{
    public:
        LoadedForBear(SkillPtr pSkill):SkillRune(pSkill) {}
        virtual ~LoadedForBear();

        virtual std::vector<Effect> use(const AttributePtr& pAttribute);
};

class Maelstrom : public SkillRune
{
    public:
        Maelstrom(SkillPtr pSkill):SkillRune(pSkill) {}
        virtual ~Maelstrom();
        
        virtual std::vector<Effect> use(const AttributePtr& pAttribute);
};

} // end of namespace dhg

#endif
