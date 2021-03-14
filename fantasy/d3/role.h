#ifndef INCLUDED_DHG_ROLE_H
#define INCLUDED_DHG_ROLE_H

#include "util.h"

#include <string>
#include <vector>
#include <utility>
#include <tr1/memory>

#include <boost/optional.hpp>

namespace dhg{


// what defines a class in Diablo 3 is the resource type it utilizes 
// and the skill sets, item types it has access to. Strictly speaking
// there is no rigid difference between a demon hunter or a barbarian
// other than the facts aforementioned

class Role
{
    public:
        Role(const Resource& resource);
        Role(const std::vector<Resource>& resources);

        // return current level and max level of resource 
        boost::optional<std::pair<int, int> >
            getResourceLevel(const ResourceType type) const;

        void adjustResource(const ResourceType type, int cost);
        void adjustResourceLimit(const ResourceType type, int limitChange);

        void fillResource();
        void emptyResource();

    private:
        struct RoleResource
        {
            RoleResource(const Resource& resourceLimit, int current):
                m_resourceLimit(resourceLimit),
                m_current(current)
            {
            }
            Resource m_resourceLimit;
            int m_current;
        };

        struct RoleResourceFind
        {
            RoleResourceFind(const ResourceType type):m_type(type) {}
            bool operator()(const RoleResource& roleRes) const
            {
                return roleRes.m_resourceLimit.m_resourceType == m_type;
            }

            ResourceType m_type;
        };

        RoleResource& getResource(const ResourceType type);
        // const RoleResource& getResource(const ResourceType type) const;

        std::vector<RoleResource> m_resources;
};

} // end of namespace dhg

#endif
