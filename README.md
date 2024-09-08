# Homework23

## The Problem

From https://accu.org/journals/cvu/cvu36-3

Homework Challenge 23

Write a program that will evaluate a hand when playing 6-card Cribbage. There will be five cards: 4 in the hand and the starter. The distinction between hand and starter only matters for flushes. Here is a brief summary of the scoring:

* Fifteen. Each combination of cards that totals 15 scores 2 (picture cards count as 10 and the ace – always low – counts as 1).
* Pair. Each pair of cards of the same rank scores 2 (so a set of 3 scores 6 and a set of four scores 12 – there are 6 ways of selecting two from 4).
* Run. Each combination of three or more cards in sequence scores 1 for each card in the sequence.
* Flush. Four cards of the same suit in hand (excluding the starter) scores 4.
Four cards in hand the same suit as the starter scores 5
* His Nobs. Jack of the same suit as starter scores 1 (no score if the starter is a Jack).

You score for each and every combination of two cards that make a pair, of two or more cards that make 15, or of three or more cards that make a run. Note that a run of 4 just counts 4 even though it includes 2 runs of 3.

Example: ♠6, ♠5, ♥4, ♦4, ♣5.

Four ways of making 15 scores 8 (♠6 ♠5 ♥4; ♠6 ♠5 ♦4; ♠6 ♣5 ♥4; ♠6, ♣5, ♦4).

Four runs of 6 5 4 score 12 (♠6 ♠5 ♥4; ♠6 ♠5 ♦4; ♠6 ♣5 ♥4; ♠6 ♣5 ♦4).

Two pairs score 4 (♥4, ♦4 and ♠5, ♣5).

Total 24.


### Some links

* https://en.wikipedia.org/wiki/Rules_of_cribbage#The_show
* https://cribbagecalculator.com/
* https://en.wikipedia.org/wiki/Glossary_of_card_game_terms

### My environment
```
mrj@ktulu:~/code/Homework23$ gcc --version
gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
```

### My command line
```
mrj@ktulu:~/code/Homework23$ g++ -std=c++20 hw23.cpp && ./a.out
```

### Thoughts

It is quite a long program. Even after making the Hand() function table driven, it seems clumsy.

The Card structure has redundancy. I tried to keep it data only, but if we have a getter for value, rather than a member,
we could calculate it from rank on demand. This is only used when finding 15s, so it would offer no benefit.

The flags for whether we have found a run or flush are annoying. I couldn't think of a neater way of doing this.

In early versions of the Score() function I explicitly handled groups of 2, 3, 4 then 5 cards,
only calling scoring functions that were valid for those numbers of cards.
This was lengthy and repetitive. It was much simpler to condense this into a loop over a list of the group sizes, then enforce the preconditions in
the scoring functions.

The Announce() function, which started life as raw cout debug printing, evolved into a multipurpose tool, and ended up being able to announce the score almost the same way a human would when scoring a real life game. An unexpected bonus.

As usual the code evolved through TDD, resulting in a good set of tests that prove it works. And these tests provide a solid framework within which to refactor.

Apart from the Card data type, the code isn't very object oriented. Putting all the scoring logic inside a class might simplify things, but a brief experiment showed this was just shuffling the existing code around and had little benefit.

I tried to use STL algorithms and ranges where possible but there are still too many C style for loops. They are a code smell that indicates 'should have tried harder'.

It would be possible to use our knowledge of how groups of cards are scored to hard code all the permutations that need to be evaluated for the different scoring functions. This would remove the need for all the code for finding combinations. The simples example is scoring flushes: we only really need to consider 2 combinations of cards - all 5, and the 4 excluding the start card.



