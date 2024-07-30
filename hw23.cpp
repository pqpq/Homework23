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

// https://rosettacode.org/wiki/Combinations#C++
vector<vector<int>> Comb(int N, int K)
{
    vector<vector<int>> combinations;

    string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's

    do // permute bitmask
    {
        vector<int> values;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) values.push_back(i);
        }
        combinations.push_back(values);
    } while (prev_permutation(bitmask.begin(), bitmask.end()));

    return combinations;
}


void test(int actual, int expected)
{
    if (expected == actual)
        cout << "Test passed\n";
    else
        cerr << "Test FAILED! : expected " << expected << ", got " << actual << '\n';
}

struct Card
{
    int rank{};
    char suit{};
    auto operator<=>(const Card& other) const = default;
};

ostream& operator<<(ostream& o, const Card& c)
{
    return o << c.rank << c.suit;
}

vector<Card> Hand(string_view s)
{
    vector<Card> cards;

    constexpr array<string_view, 13> ranks =
    {
        "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" 
    };

    while (!s.empty())
    {
        bool found{};
        for (size_t i = 0; i < ranks.size(); ++i)
        {
            if (s.starts_with(ranks[i]))
            {
                cards.push_back({ .rank = static_cast<int>(i + 1) });
                s = s.substr(ranks[i].size());
                cards.back().suit = s[0];
                s.remove_prefix(1);
                found = true;
                break;
            }
        }

        if (!found)
        {
            s.remove_prefix(1);
        }
    }
    return cards;
}

vector<vector<Card>> CombCards(const vector<Card>& hand, int K)
{
    vector<vector<Card>> results;

    const auto combinations = Comb(hand.size(), K);
    for(const auto& c : combinations)
    {
        vector<Card> cards;
        ranges::transform(c, back_inserter(cards), [&](int i){ return hand[i]; });
        results.push_back(cards);
    }
    return results;
}


int Score(string_view s)
{
cout << s << '\n';

    int score{};

    auto hand = Hand(s);
    auto pairs = CombCards(hand,2 );

    for (const auto& p : pairs)
    {
cout << p[0] << ',' << p[1] << '\n';
        if (p[0].rank == p[1].rank)
        {
            score += 2;
            cout << "Pair: " << p[0] << ", " << p[1] << '\n';
        }
        if (p[0].rank + p[1].rank == 15)
        {
            score += 2;
            cout << "15: " << p[0] << ", " << p[1] << '\n';
        }
    }

    return score;
}

int main()
{
    // 20 for 2 card 15s and pairs. Should be 28 in total.
    test(Score("5S, 5C, 5H, 5D, 10C"), 20);

    return 0;
}
