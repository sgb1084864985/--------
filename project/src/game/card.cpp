#include <assert.h>
#include <stdlib.h>
#include <array>
#include <string>

#include "card.h"

using namespace std;

Card::Card(int i): id(i) {
    assert(i >= 0 && i < 54);
}
Card::Card(int s, int r) {
    assert(s >= 0 && s < 5);
    if (s == 4) {
        assert(!r || r == 14);
        id = r? 53: 0;
    } else {
        assert(r > 0 && r <= 13);
        id = 13 * s + r;
    }
}
int Card::GetID() const {
    return id;
}
int Card::GetSuit() const {
    if (id == 0 || id == 53) {
        return joker;
    }
    return (id - 1) / 13;
}
int Card::GetRank() const {
    int r = id;
    for (int i = 0; i < 3; i++) {
        if (r > 13) {
            r -= 13;
        }
    }
    return r;
}

CardSet::CardSet(int n): noc(54 * n) {
    assert(n >= 0 && n < 128);
    for (int i = 0; i < 54; i++) {
        num[i] = (char)n;
    }
}
CardSet::CardSet(const string &s) {
    assert(s.size() == 54);
    noc = 0;
    for (int i = 0; i < 54; i++) {
        assert(s[i] >= 0);
        num[i] = s[i];
        noc += s[i];
    }
}
int CardSet::GetNum(const Card &c) const {
    return num[c.GetID()];
}
int CardSet::GetNum(int s, int r) const {
    return GetNum(Card(s, r));
}
int CardSet::GetNumOfCards() const {
    return noc;
}
Card CardSet::GetCard(int k) const {
    assert(k >= 0 && k < noc);
    for (int i = 0; i < 54; i++) {
        k -= num[i];
        if (k < 0) {
            return Card(i);
        }
    }
}
void CardSet::Insert(const Card &c) {
    assert(num[c.GetID()] < 127);
    num[c.GetID()]++;
    noc++;
}
void CardSet::Insert(int s, int r) {
    Insert(Card(s, r));
}
void CardSet::Insert(const CardSet &s) {
    for (int i = 0; i < 54; i++) {
        assert(/*(int)*/num[i] + s.num[i] < 128);
        num[i] += s.num[i];
    }
    noc += s.noc;
}
void CardSet::Delete(const Card &c) {
    assert(num[c.GetID()]);
    num[c.GetID()]--;
    noc--;
}
void CardSet::Delete(int s, int r) {
    Delete(Card(s, r));
}
void CardSet::Delete(const CardSet &s) {
    for (int i = 0; i < 54; i++) {
        assert(num[i] - s.num[i] >= 0);
        num[i] -= s.num[i];
    }
    noc -= s.noc;
}
CardSet CardSet::Take(int k) {
    assert(k >= 0 && k <= noc);
    assert(noc <= max_card_set_size);
    bool a[max_card_set_size];
    if (2 * k < noc) {
        for (int i = 0; i < noc; i++) {
            a[i] = false;
        }
        while (k) {
            int r = rand() % noc;
            if (!a[r]) {
                a[r] = true;
                k--;
            }
        }
    } else {
        for (int i = 0; i < noc; i++) {
            a[i] = true;
        }
        while (k < noc) {
            int r = rand() % noc;
            if (a[r]) {
                a[r] = false;
                k++;
            }
        }
    }
    CardSet s;
    for (int i = 0; i < noc; i++) {
        if (a[i]) {
            s.Insert(GetCard(i));
        }
    }
    Delete(s);
    return s;
}
string CardSet::String() const {
    return string(num, 54);
}
CardSet::operator bool() const {
    return noc;
}
CardSet CardSet::operator+(const Card &c) const {
    CardSet t = *this;
    t.Insert(c);
    return t;
}
CardSet CardSet::operator+(const CardSet &s) const {
    CardSet t = *this;
    t.Insert(s);
    return t;
}
CardSet CardSet::operator-(const Card &c) const {
    CardSet t = *this;
    t.Delete(c);
    return t;
}
CardSet CardSet::operator-(const CardSet &s) const {
    CardSet t = *this;
    t.Delete(s);
    return t;
}
bool CardSet::operator==(const CardSet &s) const {
    if (noc != s.noc) {
        return false;
    }
    for (int i = 0; i < 54; i++) {
        if (num[i] != s.num[i]) {
            return false;
        }
    }
    return true;
}
bool CardSet::operator!=(const CardSet &s) const {
    return !operator==(s);
}
bool CardSet::operator<=(const CardSet &s) const {
    if (noc > s.noc) {
        return false;
    }
    for (int i = 0; i < 54; i++) {
        if (num[i] > s.num[i]) {
            return false;
        }
    }
    return true;
}

/*
    test code
    expected:
    1st line all 1's
    2nd line 15 1's
    3rd line 35 1's
*/

// #include <iostream>
// #include <time.h>
// int main() {
//     Card c(0);
//     cout << (c.GetID() == 0 && c.GetSuit() == joker && c.GetRank() == R);
//     c = Card(5);
//     cout << (c.GetID() == 5 && c.GetSuit() == heart && c.GetRank() == 5);
//     c = Card(38);
//     cout << (c.GetID() == 38 && c.GetSuit() == club && c.GetRank() == Q);
//     c = Card(53);
//     cout << (c.GetID() == 53 && c.GetSuit() == joker && c.GetRank() == B);
//     c = Card(joker, R);
//     cout << (c.GetID() == 0 && c.GetSuit() == joker && c.GetRank() == R);
//     c = Card(heart, K);
//     cout << (c.GetID() == 13 && c.GetSuit() == heart && c.GetRank() == K);
//     c = Card(diamond, A);
//     cout << (c.GetID() == 14 && c.GetSuit() == diamond && c.GetRank() == A);
//     c = Card(club, 10);
//     cout << (c.GetID() == 36 && c.GetSuit() == club && c.GetRank() == 10);
//     c = Card(joker, B);
//     cout << (c.GetID() == 53 && c.GetSuit() == joker && c.GetRank() == B);
//     CardSet s0, s1(1), s2(2);
//     s0.Insert(Card(15));
//     s1.Insert(s0);
//     s1.Delete(Card(39));
//     s2.Delete(s1);
//     cout << (s2.GetNum(53) == 1 && s2.GetNum(diamond, 2) == 0 && s2.GetNum(39) == 2);
//     cout << ((s0 + s1).GetNum(15) == 3 && (s2 - Card(39)).GetNum(39) == 1);
//     s2 = CardSet(s2.String());
//     cout << (s2.GetNum(53) == 1 && s2.GetNum(diamond, 2) == 0 && s2.GetNum(39) == 2 && s2.GetNumOfCards() == 54);
//     cout << (s0 == s0 && !(s0 != s0) && s0 != s1 && !(s0 == s1)) << endl;
//     srand(time(0));
//     CardSet s = CardSet(1).Take(15);
//     for (int i = 0; i < 54; i++) {
//         cout << s.GetNum(i);
//     }
//     cout << endl;
//     s = CardSet(1).Take(35);
//     for (int i = 0; i < 54; i++) {
//         cout << s.GetNum(i);
//     }
//     cout << endl;
//     return 0;
// }
