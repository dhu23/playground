#ifndef INCLUDED_DHG_DEMONHUNTER_H
#define INCLUDED_DHG_DEMONHUNTER_H

#include "character_class.h"
#include "skill.h"
#include "util.h"

#include <tr1/memory>
#include <vector>

namespace dhg{

class Skill;

typedef std::tr1::shared_ptr<Skill> SkillPtr;

class DemonHunter : public CharacterClass
{
    public:
        static std::tr1::shared_ptr<CharacterClass> getLevelOneDemonHunter();

        ~DemonHunter() {}

        SkillPtr getSkill(
                const SkillName skillName, const RuneName runeName);

    private:
        DemonHunter(
                const int level,
                const std::vector<Resource>& resources, 
                const AttributePtr pAttri):
            CharacterClass(level, resources, pAttri)
        {
        }
};

} // end of namespace dhg

#endif
