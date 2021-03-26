#include "character.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Character c("Diablo");
    c.print(std::cout) << std::endl;

    // c.onBuff(Buff::create(BuffName::BaneOfTheTrapped, 2));
    c.onDebuff(Debuff::create(DebuffName::CurseOfAgony, 3));
    c.print(std::cout) << std::endl;

    c.onTurn();
    c.print(std::cout) << std::endl;

    c.onTurn();
    c.print(std::cout) << std::endl;

    c.onBuff(Buff::create(BuffName::Renew, 5));
    c.onTurn();
    c.print(std::cout) << std::endl;

    c.onTurn();
    c.print(std::cout) << std::endl;

    c.onTurn();
    c.print(std::cout) << std::endl;

    c.onTurn();
    c.print(std::cout) << std::endl;

    c.onTurn();
    c.print(std::cout) << std::endl;

    return 0;
}
