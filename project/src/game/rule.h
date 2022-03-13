#ifndef RULE_H
#define RULE_H

#include "card.h"

// The 1-vs-2 rule has many variants around China. We defaultly use the rules from Wikipedia.
enum Rule {r_1v2_wiki, r_1v2_pagat, r_1v3, r_2v2};

constexpr int
    illegal = -1,
    normal = 1,
    rocket = 6,
    nuke = 9,
    pass = 10
;

constexpr int s_std[18] = {0, 0, 0, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A, 2, B, R};

class Analysis {
    int flag, prim, kick, len, val;
public:
    Analysis(int f = 0, int p = 0, int k = 0, int l = 0, int v = 0); // returns an analysis with specified values
    explicit Analysis(const CardSet &s, Rule r); // analyzes s under r
    int GetFlag() const; // returns the type of the hand, which can be normal, rocket, etc. A bomb's flag equals its multiplicity. A hand of higher flag is always a legal response of one of lower flag.
    int GetPrim() const; // returns the multiplicity of primal card(s)
    int GetKick() const; // returns the multiplicity of kicker card(s)
    int GetLen() const; // returns the span of primal card(s)
    int GetVal() const; // returns the value of the smallest primal card(s), with J, Q, K, A, 2, black joker, red joker equals 11 - 17 respectively
    bool operator>(const Analysis &a) const; // returns whether this hand legally responds a
};
// e.g. 3777888J is analyzed "normal, 3, 1, 2, 7"

#endif
