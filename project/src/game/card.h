#ifndef CARD_H
#define CARD_H

#include <string>

using namespace std;

constexpr int
    heart = 0,
    diamond = 1,
    club = 2,
    spade = 3,
    joker = 4
;

constexpr int
    R = 0, // red joker
    A = 1,
    J = 11,
    Q = 12,
    K = 13,
    B = 14 // black joker
;

constexpr int max_card_set_size = 108;

class Card {
    int id;
public:
    // Each card is mapped to an integer in [0, 54), called its ID.
    // Do not care about the mapping rule when using the class.
    // Please use ID only for traverals. Otherwise use suit and rank.
    Card(int i); // returns a card of ID i
    Card(int s, int r); // returns a card of suit s and rank r
    int GetID() const; // returns the card's ID
    int GetSuit() const; // returns the card's suit
    int GetRank() const; // returns the card's rank
};

class CardSet {
    char num[54];
    int noc;
public:
    explicit CardSet(int n = 0); // returns a set of n full decks, containing 54n cards
    explicit CardSet(const string &s); // decodes a string encoded by CardSet::String()
    int GetNum(const Card &c) const; // returns the number of c's in the set
    int GetNum(int s, int r) const; // returns the number of Card(s, r)'s in the set
    int GetNumOfCards() const; // returns the total number of cards in the set
    Card GetCard(int k) const; // returns the kth card in the set sorted with ID
    void Insert(const Card &c); // inserts a c into the set
    void Insert(int s, int r); // inserts a Card(s, r) into the set
    void Insert(const CardSet &s); // inserts s into the set
    void Delete(const Card &c); // deletes a c from the set
    void Delete(int s, int r); // deletes a Card(s, r) from the set
    void Delete(const CardSet &s); // subtracts s from the set
    CardSet Take(int k = 1); // randomly takes and subtracts k cards from the set
    string String() const; // encodes the set to a string
    operator bool() const; // returns whether the set is empty
    CardSet operator+(const Card &s) const; // returns the sum of the set and c
    CardSet operator+(const CardSet &s) const; // returns the sum of the set and s
    CardSet operator-(const Card &c) const; // returns the difference of the set and c
    CardSet operator-(const CardSet &s) const; // returns the difference of the set and s
    bool operator==(const CardSet &s) const; // returns whether the set is exactly the same as s
    bool operator!=(const CardSet &s) const; // returns whether the set is different from s
    bool operator<=(const CardSet &s) const; // returns whether the set is a subset of s
};

#endif
