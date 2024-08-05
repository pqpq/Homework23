# Homework23

## The Problem

From https://accu.org/journals/cvu/cvu36-3

Homework Challenge 23

Write a program that will evaluate a hand when playing 6-card Cribbage. There will be five cards: 4 in the hand and the starter. The distinction between hand and starter only matters for flushes. Here is a brief summary of the scoring:

Fifteen. Each combination of cards that totals 15 scores 2 (picture cards count as 10 and the ace – always low – counts as 1).
Pair. Each pair of cards of the same rank scores 2 (so a set of 3 scores 6 and a set of four scores 12 – there are 6 ways of selecting two from 4).
Run. Each combination of three or more cards in sequence scores 1 for each card in the sequence.
Flush. Four cards of the same suit in hand (excluding the starter) scores 4.
Four cards in hand the same suit as the starter scores 5
His Nobs. Jack of the same suit as starter scores 1 (no score if the starter is a Jack).
Web Editor's Note: The scoring of four of a kind has been corrected here to conform to normal Cribbage scoring. The print edition of this article gave the score for four of a kind as 24.

You score for each and every combination of two cards that make a pair, of two or more cards that make 15, or of three or more cards that make a run. Note that a run of 4 just counts 4 even though it includes 2 runs of 3.

Example: ♠6, ♠5, ♥4, ♦4, ♣5.

Four ways of making 15 scores 8 (♠6 ♠5 ♥4; ♠6 ♠5 ♦4; ♠6 ♣5 ♥4; ♠6, ♣5, ♦4).

Four runs of 6 5 4 score 12 (♠6 ♠5 ♥4; ♠6 ♠5 ♦4; ♠6 ♣5 ♥4; ♠6 ♣5 ♦4).

Two pairs score 4 (♥4, ♦4 and ♠5, ♣5).

Total 24.


My notes:

https://en.wikipedia.org/wiki/Rules_of_cribbage#The_show
https://cribbagecalculator.com/

My environment:
```
mrj@ktulu:~/code/Homework23$ gcc --version
gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
```

My command line:
```
mrj@ktulu:~/code/Homework23$ g++ -std=c++20 hw23.cpp && ./a.out
```

