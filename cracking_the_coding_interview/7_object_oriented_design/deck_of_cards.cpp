#include "deck_of_cards.h"
#include <stdexcept>
#include <string>
#include <map>

namespace 
{

    std::map<Suite, std::string> createSuiteNameMap()
    {
        std::map<Suite, std::string> m;
        m.emplace(Suite::SPADE, "Spade");
        m.emplace(Suite::HEART, "Heart");
        m.emplace(Suite::CLUB, "Club");
        m.emplace(Suite::DIAMOND, "Diamond");
        return std::move(m);
    }

    std::map<Value, std::string> createValueNameMap()
    {
        std::map<Value, std::string> m;
        m.emplace(Value::ACE, "A");
        m.emplace(Value::TWO, "2");
        m.emplace(Value::THREE, "3");
        m.emplace(Value::FOUR, "4");
        m.emplace(Value::FIVE, "5");
        m.emplace(Value::SIX, "6");
        m.emplace(Value::SEVEN, "7");
        m.emplace(Value::EIGHT, "8");
        m.emplace(Value::NINE, "9");
        m.emplace(Value::TEN, "10");
        m.emplace(Value::JACK, "J");
        m.emplace(Value::QUEEN, "Q");
        m.emplace(Value::KING, "K");
        return std::move(m);
    }

} // end of namespace



std::string getSuiteName(Suite s)
{
    const static std::map<Suite, std::string> suiteMap(createSuiteNameMap());
    auto it = suiteMap.find(s);
    if (it == suiteMap.end())
    {
        throw std::runtime_error("Suite name not recognized");
    }
    return it->second; 
}

std::string getValueName(Value v)
{
    const static std::map<Value, std::string> valueMap(createValueNameMap());
    auto it = valueMap.find(v);
    if (it == valueMap.end())
    {
        throw std::runtime_error("Value name not recognized");
    }
    return it->second;
}


std::ostream& Card::print(std::ostream& os) const
{
    os << getValueName(value) << '(' << getSuiteName(suite) << ')';
    return os;
}

Deck::Deck(const std::vector<Card>& cs):cards(cs) {}
Deck::Deck(std::vector<Card>&& cs):cards(cs) {}

Deck Deck::createStandDeck()
{
    Deck d;
    d.cards.reserve(4*13);
    for (auto s : { Suite::SPADE, Suite::HEART, Suite::CLUB, Suite::DIAMOND })
    {
        for (auto v : {
                Value::ACE, 
                Value::TWO,
                Value::THREE, 
                Value::FOUR, 
                Value::FIVE, 
                Value::SIX, 
                Value::SEVEN, 
                Value::EIGHT, 
                Value::NINE, 
                Value::TEN, 
                Value::JACK, 
                Value::QUEEN, 
                Value::KING })
        {
            d.cards.emplace_back(s, v);
        }
    }
    return std::move(d);
}

Deck Deck::createBlackJackDeck()
{
    throw std::runtime_error("not implemented");
}
