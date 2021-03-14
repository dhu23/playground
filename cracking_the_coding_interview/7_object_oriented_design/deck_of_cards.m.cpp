#include "deck_of_cards.h"

int main(int argc, char* argv[])
{
    Deck standDeck = Deck::createStandDeck();
    for (auto& c : standDeck.cards)
    {
        c.print(std::cout);
        std::cout << std::endl;
    }

    std::cout << toUnderlying(Suite::SPADE) << std::endl;
    std::cout << toUnderlying(Value::KING) << std::endl;
    return 0;
}
