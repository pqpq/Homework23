// ACCU Homework Challenge 23
//
// From https://accu.org/journals/cvu/cvu36-3
//
// Write a program that will evaluate a hand when playing 6-card Cribbage. 
//
// There will be five cards: 4 in the hand and the starter. 
// The distinction between hand and starter only matters for flushes. 
// Here is a brief summary of the scoring:
// 
// Fifteen. Each combination of cards that totals 15 scores 2 (picture cards count as 10 and the ace – always low – counts as 1).
// Pair.    Each pair of cards of the same rank scores 2 (so a set of 3 scores 6 and a set of four scores 12 – there are 6 ways of selecting two from 4).
// Run.     Each combination of three or more cards in sequence scores 1 for each card in the sequence.
// Flush.   Four cards of the same suit in hand (excluding the starter) scores 4.
//          Four cards in hand the same suit as the starter scores 5
// His Nobs. Jack of the same suit as starter scores 1 (no score if the starter is a Jack).
// 
// You score for each and every combination of two cards that make a pair, of two or more cards that make 15, or of three or more cards that make a run. Note that a run of 4 just counts 4 even though it includes 2 runs of 3.
// Example: ♠6, ♠5, ♥4, ♦4, ♣5.
// Four ways of making 15 scores 8 (♠6 ♠5 ♥4; ♠6 ♠5 ♦4; ♠6 ♣5 ♥4; ♠6, ♣5, ♦4).
// Four runs of 6 5 4 score 12 (♠6 ♠5 ♥4; ♠6 ♠5 ♦4; ♠6 ♣5 ♥4; ♠6 ♣5 ♦4).
// Two pairs score 4 (♥4, ♦4 and ♠5, ♣5).
// Total 24.

#include <algorithm>
#include <array>
#include <cctype>
#include <iostream>
#include <ranges>
#include <set>
#include <utility>
#include <vector>

using namespace std;


void test(int actual, int expected)
{
    if (expected == actual)
        cout << "Test passed\n";
    else
        cerr << "Test FAILED! : expected " << expected << ", got " << actual << '\n';
}

struct Card
{
    int value{};
    char suit{};
    auto operator<=>(const Card& other) const = default;
};


vector<Card> Hand(string_view s)
{
    vector<Card> cards;

    constexpr array<string_view, 4> suitsUnicode{ "♣", "♦", "♥", "♠" };
    constexpr array<char, 4> suitsAscii{ 'c','d','h','s' };
    constexpr char digits[]{ "0123456789" };

    while (!s.empty())
    {
        for (size_t i = 0; i < suitsUnicode.size(); ++i)
        {
            if (s.starts_with(suitsUnicode[i]))
            {
                cards.push_back({ .suit = suitsAscii[i] });
                s = s.substr(suitsUnicode[i].size());
            }
        }

        if (s.find_first_of(digits) != 0)
        {
            s.remove_prefix(1);
            continue;
        }

        auto pos = s.find_first_not_of(digits);
        auto part = s.substr(0, pos);
        try
        {
            auto value = stoi(string{part});
            cards.back().value = value;
        }
        catch(...) {}
        if (pos != string::npos)
        {
            s = s.substr(pos);
        }
        else
        {
            s = {};
        }
    }
    return cards;
}


int Score(string_view s)
{
cout << s << '\n';
    auto hand = Hand(s);

    set<pair<Card, Card>> pairs;
#if 0
    do
    {
        for (auto c : hand)
        {
            cout << c.value << c.suit << ',';
        }
        cout << endl;
        pairs.insert({hand[0], hand[1]});
    }
    while (std::ranges::next_permutation(hand).found);

#endif

    for (auto i : views::iota(0u, hand.size()))
    {
        for (auto j : views::iota(i+1, hand.size()))
        {
            cout << i << ", " << j << '\n';
            pairs.insert({hand[i], hand[j]});
        }
    }

    for (const auto& p : pairs)
    {
cout << p.first.value << p.first.suit << ',' << p.second.value << p.second.suit << ',' << '\n';
    }

    return 0;
}

int main()
{
/// @todo This isn't creating more than 1 permutation because all the 5s are equivalent.
/// We need to give each value a suit, so that comparison can include suites so the 5s
/// don't compare equal. Then they should permute properly?

    test(Score("♠5, ♣5, ♥5, ♦5, ♣10"), 14);     // Get 15s working

    return 0;
}
