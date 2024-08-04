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
#include <numeric>
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
    // these are after rank and suit so they aren't involved in operator<=>
    int value{};
    string_view printable;
    auto operator<=>(const Card& other) const = default;
};

ostream& operator<<(ostream& o, const Card& c)
{
    return o << c.printable;
}

int operator+(int a, const Card& b)
{
    return a + b.value;
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
                const auto rank = static_cast<int>(i + 1);
                cards.push_back({ .rank = rank });
                cards.back().printable = s.substr(0, ranks[i].size() + 1); // include the suit
                s = s.substr(ranks[i].size());
                cards.back().suit = s[0];
                cards.back().value = min(10, rank);
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

void Dump(const char* context, int score, const vector<Card>& cards)
{
    if (score) cout << score << " for ";
    cout << context << ": ";
    ranges::for_each(cards, [&](const Card& c){ cout << c << ','; });
    cout << '\n';
}

int Fifteens(const vector<Card>& cards)
{
    if (accumulate(cards.begin(), cards.end(), 0) == 15)
    {
        Dump("15", 2, cards);
        return 2;
    }
    return 0;
}

int Pairs(const vector<Card>& cards)    // assumes size of 2
{
    if (cards[0].rank == cards[1].rank)
    {
        Dump("pair", 2, cards);
        return 2;
    }
    return 0;
}

int Runs(const vector<Card>& cards)    // should be 3 or more
{
    bool run = true;
    for (size_t i = 1; i < cards.size(); ++i)
    {
        run = run && cards[i].rank == cards[i-1].rank + 1;
    }
    if (run)
    {
        const auto score = static_cast<int>(cards.size());
        Dump("run", score, cards);
        return score;
    }
    return 0;
}


int Score(string_view s)
{
    int score{};

    auto hand = Hand(s);

Dump("Hand", 0, hand);

    bool foundARunOf5{};
    ranges::sort(hand);
    score += Fifteens(hand);
    const auto run = Runs(hand);
    score += run;
    if (run) foundARunOf5 = true;
    // todo: flush

    bool foundARunOf4{};
    auto quads = CombCards(hand, 4);
    for (auto& q : quads)
    {
        ranges::sort(q);
Dump("   4", 0, q);
        score += Fifteens(q);
        if (!foundARunOf5)
        {
            const auto run = Runs(q);
            score += run;
            if (run) foundARunOf4 = true;
        }
        // todo: flush. Need same "found 5" block logic
        // Pairs scored later
    }

    auto triples = CombCards(hand, 3);
    for (auto& t : triples)
    {
        ranges::sort(t);
Dump("   3", 0, t);
        score += Fifteens(t);
        if (!foundARunOf4 && !foundARunOf5)
        {
            score += Runs(t);
        }
        // Pairs scored later
    }

    auto pairs = CombCards(hand, 2);
    for (const auto& p : pairs)
    {
Dump("   2", 0, p);
        score += Pairs(p);
        score += Fifteens(p);
    }

    return score;
}

int main()
{
    // 8 for 2 card 15s (10 and 5)
    // 8 for 3 card 15s (3 x 5)
    // 12 for 4 5s (6 sets of pairs)
    test(Score("5S, 5C, 5H, 5D, 10C"), 28);

    // 4 for 15s (J+5, J+2+3)
    // 3 for run (1, 2, 3)
    test(Score("JS, 5C, AH, 2D, 3C"), 7);

    // Example from the article
    // Four ways of making 15 scores 8 (♠6 ♠5 ♥4; ♠6 ♠5 ♦4; ♠6 ♣5 ♥4; ♠6, ♣5, ♦4).
    // Four runs of 6 5 4 score 12 (♠6 ♠5 ♥4; ♠6 ♠5 ♦4; ♠6 ♣5 ♥4; ♠6 ♣5 ♦4).
    // Two pairs score 4 (♥4, ♦4 and ♠5, ♣5). Total 24.
    test(Score("6S, 5S, 4H, 4D, 5C"), 24);

    // 6 for 15s
    // 3 for run (J, Q, K)
    test(Score("KD, 2D, JS, QD, 5H"), 9);

    // 2 for the 5 card 15
    // 5 for the run of 5
    test(Score("AD, 2H, 3C, 4D, 5H"), 7);

    return 0;
}
