#include <vector>
// #include <set>
#include <iostream>
#include <type_traits>

enum class Suite { SPADE = 0, HEART = 1, CLUB = 2, DIAMOND = 4 };
enum class Value { 
    ACE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, 
    SIX = 6, SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10, 
    JACK = 11, QUEEN = 12, KING = 13 };

std::string getSuiteName(Suite s);
std::string getValueName(Value v);

template<typename EnumClass>
typename std::underlying_type<EnumClass>::type toUnderlying(EnumClass ec)
{
    return static_cast<typename std::underlying_type<EnumClass>::type>(ec);
}

struct Card
{
    Card(Suite s, Value v):suite(s), value(v) {}
    Suite suite;
    Value value; 

    std::ostream& print(std::ostream& os) const;
};


// I probably don't want a base class for game cards
// because I probably will never deal with them polymorphically
class GameCard
{
    public:
        GameCard(const Card& card):_card(card) {}
        virtual ~GameCard() = default;
        GameCard(const GameCard& gc) = default;
        GameCard& operator=(const GameCard& gc) = default;
        GameCard(GameCard&& gc) = default;
        GameCard& operator=(GameCard&& gc) = default;

    protected:
        Card _card;
};

class BlackJackCard : public GameCard
{
    public: 
        BlackJackCard(const Card& card):GameCard(card) {}

        std::vector<int> possibleValues() const
        {
            int faceValue = toUnderlying(_card.value);
            if (faceValue > 10)
            {
                return std::vector<int>(10);
            }
            else if (faceValue >= 2)
            {
                return std::vector<int>(faceValue);
            }
            else
            {
                return std::vector<int>{1, 11};
            }
        }
};

class TexasHoldemCard : public GameCard
{
    public:
        TexasHoldemCard(const Card& card):GameCard(card) {}
        
        int value() const
        {
            return toUnderlying(_card.value);
        }
};

class Deck
{
    public:
        static Deck createStandDeck();
        static Deck createBlackJackDeck(); 

        Card deal();
        std::vector<Card> deal(size_t num);

        std::vector<Card> cards;
    private:
        Deck() {}
        Deck(const std::vector<Card>& cs);
        Deck(std::vector<Card>&& cs);
        // Deck(size_t count);
};
