#include "role.h"
#include "attribute.h"

#include <algorithm>
#include <stdexcept>

namespace dhg{

using std::string;
using std::vector;
using std::pair;


namespace{


} // end of anonymous namespace


Role::Role(const Resource& resource):m_resources()
{
    m_resources.push_back(RoleResource(resource, resource.m_amount));
}

Role::Role(const vector<Resource>& resources):m_resources()
{
    for(vector<Resource>::const_iterator itr = resources.begin();
            resources.end() != itr; ++itr)
    {
        m_resources.push_back(RoleResource(*itr, itr->m_amount));
    }
}


boost::optional<pair<int, int> >
Role::getResourceLevel(const ResourceType type) const
{
    vector<RoleResource>::const_iterator itr = 
        std::find_if(
                m_resources.begin(), m_resources.end(),
                RoleResourceFind(type));

    boost::optional<pair<int, int> > level;
    if(itr != m_resources.end())
    {
        level = pair<int, int>(itr->m_current, itr->m_resourceLimit.m_amount);
    }
    return level;
}


void Role::adjustResource(const ResourceType type, int cost)
{
    RoleResource& roleRes = this->getResource(type);
        
    if(cost > roleRes.m_current)
    {
        throw std::runtime_error("Insufficient resource");
    }
        
    roleRes.m_current -= cost;
    roleRes.m_current = 
        std::min(roleRes.m_current, roleRes.m_resourceLimit.m_amount);
}

void Role::adjustResourceLimit(const ResourceType type, int limitChange)
{
    RoleResource& roleRes = this->getResource(type);
    roleRes.m_resourceLimit.m_amount += limitChange;
    roleRes.m_resourceLimit.m_amount = 
        std::min(0, roleRes.m_resourceLimit.m_amount);
}

void Role::fillResource()
{
    for(vector<RoleResource>::iterator itr = m_resources.begin();
            m_resources.end() != itr; ++itr)
    {
        itr->m_current = itr->m_resourceLimit.m_amount;
    }
}

void Role::emptyResource()
{
    for(vector<RoleResource>::iterator itr = m_resources.begin();
            m_resources.end() != itr; ++itr)
    {
        itr->m_current = 0;
    }
}

Role::RoleResource& Role::getResource(const ResourceType type)
{   
    vector<RoleResource>::iterator itr = 
        std::find_if(
                m_resources.begin(), m_resources.end(),
                RoleResourceFind(type));

    if(itr == m_resources.end())
    {
        throw std::runtime_error("Resource type NOT found");
    } 
    return *itr;
}

/*
const Role::RoleResource& Role::getResource(const ResourceType type) const
{
    vector<RoleResource>::const_iterator itr = 
        std::find_if(
                m_resources.begin(), m_resources.end(),
                RoleResourceFind(type));

    if(itr == m_resources.end())
    {
        throw std::runtime_error("Resource type NOT found");
    }
    return *itr;
}
*/

} // end of namespace dhg
