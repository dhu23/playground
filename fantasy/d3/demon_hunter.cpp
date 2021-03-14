#include "demon_hunter.h"
#include "attribute.h"
#include "skill_factory.h"
#include "cluster_arrow.h"

#include <stdexcept>

namespace dhg{

using std::vector;
using std::tr1::shared_ptr;

shared_ptr<CharacterClass> DemonHunter::getLevelOneDemonHunter()
{
    vector<Resource> resources;
    resources.push_back(Resource(Resource::HATRED, 125));
    resources.push_back(Resource(Resource::DISCIPLINE, 30));

    AdditiveAttribute nakedAdditiveAttribute;
    nakedAdditiveAttribute
        .setStrength(25).setDexterity(40).setIntelligence(15)
        .setVitality(30).setArmor(80);

    return std::tr1::shared_ptr<CharacterClass>(
            new DemonHunter(
                1, resources, 
                AttributePtr(new Attribute(nakedAdditiveAttribute))));
}

SkillPtr DemonHunter::getSkill(
        const SkillName skillName, const RuneName runeName)
{
    switch(skillName)
    {
        case CLUSTER_ARROW:
            {
                SkillFactory<ClusterArrow> clusterArrowFactory;
                switch(runeName)
                {
                    case NONE:
                        return clusterArrowFactory.get();
                    case LOADED_FOR_BEAR:
                        return clusterArrowFactory.get<LoadedForBear>();
                    case MAELSTROM:
                        return clusterArrowFactory.get<Maelstrom>();
                    default:
                        throw std::runtime_error("Rune NOT recognized");
                }
            }
        default:
            throw std::runtime_error("Skill NOT recognized");
    }
}

} // end of namespace dhg
