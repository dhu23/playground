#include "cluster_arrow.h"
#include "attribute.h"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;

namespace dhg{

ClusterArrow::~ClusterArrow()
{
}

// deals 500% weapon damage
vector<Effect> ClusterArrow::use(const AttributePtr& pAttribute)
{
    cout 
        << "Using Cluster Arrow--> dmg=" 
        << pAttribute->getWeaponDamage()*5 << endl;
    vector<Effect> effects;

    std::tr1::shared_ptr<Damage> pDamage(
            new Damage(ElementSchool::FIRE, pAttribute->getWeaponDamage()*5));
    
    effects.push_back(Effect(pDamage));
    return effects;
}

LoadedForBear::~LoadedForBear()
{
}

vector<Effect> LoadedForBear::use(const AttributePtr& pAttribute)
{
    cout << "Using Loaded For Bear(Cluster Arrow)" << endl;
    vector<Effect> effects;

    std::tr1::shared_ptr<Damage> pDamage(
            new Damage(ElementSchool::FIRE, pAttribute->getWeaponDamage()*7));

    effects.push_back(Effect(pDamage));
    return effects;
}

Maelstrom::~Maelstrom()
{
}

vector<Effect> Maelstrom::use(const AttributePtr& pAttribute)
{
    cout << "Using Maelstrom(Cluster Arrow)" << endl;
    return vector<Effect>();
}

} // end of namespace dhg
