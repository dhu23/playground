// #include "character.h"
// #include "demon_hunter.h"

#include "attribute.h"
#include "role.h"
#include "character.h"
#include "cluster_arrow.h"

#include <vector>
#include <iostream>

using namespace dhg;
using std::cout;
using std::endl;
using std::vector;


void testAttribute()
{
    cout << "Attribute test" << endl;

    AttributeInfo ai;
    ai.setStrength(100).setDexterity(50).setIntelligence(70);
    AttributePtr pOriginal(new Attribute(ai));

    ItemAttributeInfo item1;
    item1.m_itemStats = {{StatType::STRENGTH, 200}};

    AttributePtr pEnhanced = AttributePtr(
            new AttributeEnhancedByItem(pOriginal, item1));

    pEnhanced->print(cout);
    cout << endl;

    ItemAttributeInfo item2;
    item2.m_itemStats = {{StatType::STRENGTH, 150}};

    pEnhanced = AttributePtr(
            new AttributeEnhancedByItem(pEnhanced, item2));

    pEnhanced->print(cout);
    cout << endl;

    ItemAttributeInfo item3;
    item3.m_itemStats = {
        {StatType::DEXTERITY, 20},
        {StatType::INTELLIGENCE, 20}};

    pEnhanced = AttributePtr(
            new AttributeEnhancedByItem(pEnhanced, item3));

    pEnhanced->print(cout);
    cout << endl;
}

void testRole()
{
    Role dh(std::vector<Resource>{
            Resource(ResourceType::HATRED, 100), 
            Resource(ResourceType::DISCIPLINE, 50)});
    
    boost::optional<std::pair<int, int> > rageLevel =
        dh.getResourceLevel(ResourceType::RAGE);

    if(!rageLevel)
    {
        cout << "No Rage in Demon hunter!" << endl;
    }

    dh.adjustResource(ResourceType::HATRED, 20);
    boost::optional<std::pair<int, int> > hatredLevel =
        dh.getResourceLevel(ResourceType::HATRED);

    cout 
        << "now hatred is = " 
        << hatredLevel->first << '|' << hatredLevel->second << endl;
}

void testCharacter()
{
    std::tr1::shared_ptr<Role> pDh(
            new Role(std::vector<Resource>{
                Resource(ResourceType::HATRED, 100), 
                Resource(ResourceType::DISCIPLINE, 50)}));
    Character rhea("Rhea", pDh);

    cout << "hp = " << rhea.hitPoints() << endl;

    std::tr1::shared_ptr<Damage> pDmg1(
            new Damage(ElementSchool::FIRE, 10));
    Effect dmg1(pDmg1);

    rhea.takeEffect(dmg1);

    cout << "hp = " << rhea.hitPoints() << endl;
}

void testSkill()
{
    AttributeInfo ai;
    ai.setStrength(100).setDexterity(50).setIntelligence(70);
    ai.setWeaponDamage(500);

    AttributePtr pOriginal(new Attribute(ai));

    SkillPtr pClusterArrow(new ClusterArrow());
    pClusterArrow->use(pOriginal);

    SkillPtr pLoadedForBear(new LoadedForBear(pClusterArrow));
    pLoadedForBear->use(pOriginal);
}

/*
void testSimpleDamageTaking(Character& c)
{
    DamageOverTime dot1(PHYSICAL, 10, 3);
    c.takeDamageOverTime(dot1);

    for(int i = 0; i < 3; ++i)
    {
        DamageOverTime dot(ARCANE, 15, 2);
        c.takeDamageOverTime(dot);
        c.onEndOfRound();
        c.reportStats();
        cout << ">>>>>>>>>>> End of Round <<<<<<<<<<<" << endl;
    }

    c.onEndOfRound();
    c.onEndOfRound();
    c.onEndOfRound();

    Damage d1(PHYSICAL, 10);
    c.takeDamage(d1);

    c.onEndOfRound();
    
    c.reportStats();
}
*/


int main(int argc, char* argv[])
{
    testAttribute();
    testRole();
    testCharacter();
    testSkill();

    return 0;
}
