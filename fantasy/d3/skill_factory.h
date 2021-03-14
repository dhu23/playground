#ifndef INCLUDED_SKILL_FACTORY_H
#define INCLUDED_SKILL_FACTORY_H

#include <tr1/memory>

namespace dhg{

template<typename SKILL>
class SkillFactory
{
    public:
        SkillFactory(): 
            m_pSkill(new SKILL()) 
        {
        }
        std::tr1::shared_ptr<SKILL> get() {return m_pSkill;}
        
        template<typename RUNE> std::tr1::shared_ptr<SKILL> get();

    private:
        std::tr1::shared_ptr<SKILL> m_pSkill;
};

template<typename SKILL>
template<typename RUNE>
std::tr1::shared_ptr<SKILL> SkillFactory<SKILL>::get()
{
    return std::tr1::shared_ptr<SKILL>(new RUNE(m_pSkill));
}

}; // end of namespace dhg

#endif
