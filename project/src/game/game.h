#ifndef GAME_H
#define GAME_H

#include <array>
#include <string>

#include "Server.h"

#include "clockint.h"
#include "card.h"
#include "rule.h"
#include "message.h"
#include "package.h"

constexpr int t_bid = 10, t_playout = 20;

using namespace std;

template <int np> class CardGame {
protected:
    const array<bool, np> ishuman; // The index starts from the room creator and goes counterclockwise.
    Server &server; // through which messages are transmitted
    const CardSet ccards; // complete card set used
    array<int, np> score;
    void notify(ci<np> k, const Message &m) const; // notify the kth player of m
    Message request(ci<np> k) const;
    void broadcast(const Message &m) const;
    void broadcast(const MsgSeries<np> &ms) const;
    void disptext(const string &s) const;
    void dispeffect(const string &s) const; // global effect
    void dispeffect(ci<np> k, const string &s) const; // player effect
    void dispscore() const; // broadcasts everyone's score
    void think(ci<np> k) const; // the kth player is thinking
    virtual void play() = 0;
public:
    explicit CardGame(array<bool, np> ish, Server &s, const CardSet &c); // returns a card game with "is human" list ish and server s
    void Play();
};
template class CardGame<3>;
template class CardGame<4>;

template <int np> class WinnerBasedGame: public CardGame<np> {
protected:
    const Rule rule;
    int stake;
    CardSet tcards; // cards on the table
    array<CardSet, np> cards; // cards on each player's hand
    ci<np> starter; // the first to bid or play out
    int npasses; // number of successive passes
    Analysis last; // analysis of the last non-pass hand
    void deal(array<int, np> noc); // the ith player gets noc[i] cards
    void changestake(int s);
    virtual CardSet playout_robot(ci<np> k) const = 0;
    void playout(ci<np> k);
    virtual void play() = 0;
public:
    explicit WinnerBasedGame(array<bool, np> ish, Server &s, const CardSet &c, const Rule r);
};
template class WinnerBasedGame<3>;
template class WinnerBasedGame<4>;

class DouDizhuGame: public WinnerBasedGame<3> {
    int bid_robot(ci<3> k) const;
    void play();
protected:
	CardSet playout_robot(ci<3> k) const;
public:
    explicit DouDizhuGame(array<bool, 3> ish, Server &s);
};

class SirenDouDizhuGame: public WinnerBasedGame<4> {
    int bid_robot(ci<4> k) const;
    void play();
protected:
	CardSet playout_robot(ci<4> k) const;
public:
    explicit SirenDouDizhuGame(array<bool, 4> ish, Server &s);
};

class ShuangkouGame: public WinnerBasedGame<4> {
    void play();
protected:
	CardSet playout_robot(ci<4> k) const;
public:
    explicit ShuangkouGame(array<bool, 4> ish, Server &s);
};

#endif
