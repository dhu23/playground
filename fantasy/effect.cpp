#include "effect.h"
#include "character.h"
#include <stdexcept>

Buff Buff::create(BuffName name, int turns)
{
    int order;
    switch(name)
    {
    case BuffName::BaneOfTheTrapped:
        order = 10;
        break;
    case BuffName::Renew:
        order = 1;
        break;
    default:
        throw std::runtime_error("Wrong Buff name");
    }

    Buff b;
    b.name = name;
    b.order = order;
    b.remainingTurns = turns;

    return std::move(b);
}

Debuff Debuff::create(DebuffName name, int turns)
{
    int order;
    switch(name)
    {
    case DebuffName::CurseOfRust:
        order = 10;
        break;
    case DebuffName::CurseOfAgony:
        order = 1;
        break;
    default:
        throw std::runtime_error("wrong debuff name");
    }

    Debuff db;
    db.name = name;
    db.order = order;
    db.remainingTurns = turns;

    return std::move(db);
}

void Buff::onTurn(Character* pChar) const
{
    switch(name)
    {
    case BuffName::Renew:
        pChar->onHeal(5);
        break;
    default:
        break;
    }
    this->turn();
}

void Debuff::onTurn(Character* pChar) const
{
    switch(name)
    {
    case DebuffName::CurseOfAgony:
        pChar->onDamage(15);
        break;
    default:
        break;
    }
    this->turn();
}
std::ostream& operator<<(std::ostream& os, BuffName buffName)
{
    switch(buffName)
    {
    case BuffName::BaneOfTheTrapped:
        os << "BaneOfTheTrapped";
        break;
    case BuffName::Renew:
        os << "Renew";
        break;
    default:
        os << "Unknown-BuffName";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, DebuffName debuffName)
{
    switch(debuffName)
    {
    case DebuffName::CurseOfRust:
        os << "CurseOfRust";
        break;
    case DebuffName::CurseOfAgony:
        os << "CurseOfAgony";
        break;
    default:
        os << "Unknown-DebuffName";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Buff& rhs)
{
    os 
        << "Buff[name=" << rhs.name << ",order=" << rhs.order 
        << ",remainingTurns=" << rhs.remainingTurns;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Debuff& rhs)
{
    os 
        << "Debuff[name=" << rhs.name << ",order=" << rhs.order
        << ",remainingTurns=" << rhs.remainingTurns;
    return os;
}
