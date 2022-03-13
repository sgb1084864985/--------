#include "card.h"
#include "rule.h"

Analysis::Analysis(int f, int p, int k, int l, int v):
    flag(f), prim(p), kick(k), len(l), val(v)
{}
Analysis::Analysis(const CardSet &s, Rule r):
    flag(illegal), prim(0), kick(0), len(0), val(0) // may be changed later
{
    if (!s) {
        flag = pass;
        return;
    }
    switch (r) {
    case r_1v2_wiki:
    case r_1v2_pagat:
        if (s.GetNum(joker, B) && s.GetNum(joker, R)) {
            if (s.GetNumOfCards() == 2) flag = rocket;
            return;
        }
        break;
    case r_1v3:
    case r_2v2:
        if (s.GetNumOfCards() == 4 && s.GetNum(joker, B) == 2 && s.GetNum(joker, R) == 2) {
            flag = nuke;
            return;
        }
        // break; (When adding a case, please type a new "break;" and keep this a reminder)
    }
    int sum[15] = {};
    for (int i = 0; i < 54; i++) {
        sum[Card(i).GetRank()] += s.GetNum(Card(i));
    }
    for (int i = 0; i < 15; i++) {
        if (sum[i] && sum[i] != prim && sum[i] != kick) {
            if (kick) {
                return;
            }
            if (sum[i] > prim) {
                kick = prim;
                prim = sum[i];
            } else {
                kick = sum[i];
            }
        }
    }
    if ((prim < 3 || prim > 4) && kick || kick > 2) {
        return;
    }
    for (int i = 3; i <= 17; i++) {
        if (sum[s_std[i]] == prim) {
            if (!val) {
                val = i;
                if (i == 17) {
                    len = 1;
                }
            } else if (len) {
                return;
            }
        } else if (val && !len) {
            len = i - val;
        }
    }
    if (
        len > 1 && (
            prim == 1 && len < 5 ||
            prim == 2 && len < 3 ||
            prim > 4 ||
            val + len > 15
        ) ||
        prim == 3 && len * (3 + kick) != s.GetNumOfCards() ||
        prim == 4 && len * (4 + 2 * kick) != s.GetNumOfCards()
    ) {
        return;
    }
    switch (r) {
    case r_1v2_pagat:
        if (prim == 4 && len > 1) {
            return;
        }
        break;
    case r_1v3:
        if (
            prim == 3 && kick == 1 ||
            prim == 4 && (kick || len > 1)
        ) {
            return;
        }
        break;
    case r_2v2:
        if (kick) {
            return;
        }
        // break;
    }
    flag = prim < 4 || prim == 4 && (len > 1 || kick)? normal: prim;
}
int Analysis::GetFlag() const {
    return flag;
}
int Analysis::GetPrim() const {
    return prim;
}
int Analysis::GetKick() const {
    return kick;
}
int Analysis::GetLen() const {
    return len;
}
int Analysis::GetVal() const {
    return val;
}
bool Analysis::operator>(const Analysis &a) const {
    return
        flag > a.flag ||
            flag == a.flag &&
            prim == a.prim &&
            kick == a.kick &&
            len == a.len &&
            val > a.val
    ;
}

// test code

// #include <iostream>
// #include "card.cpp"
// using namespace std;
// int main() {
//     CardSet cs[2] = {CardSet(), CardSet()};
//     int i = 0, suit = spade;
//     Rule rule = r_1v2_wiki;
//     while (true) {
//         string s;
//         cin >> s;
//         if (s == "end") break;
//         if (s == "cs0") {
//             i = 0;
//         } else if (s == "cs1") {
//             i = 1;
//         } else if (s == "clear0") {
//             cs[0] = CardSet();
//         } else if (s == "clear1") {
//             cs[1] = CardSet();
//         } else if (s == "a0") {
//             Analysis a = Analysis(cs[0], rule);
//             cout << a.GetFlag() << " " << a.GetPrim() << " " << a.GetKick() << " " << a.GetLen() << " " << a.GetVal() << endl;
//         } else if (s == "a1") {
//             Analysis a = Analysis(cs[1], rule);
//             cout << a.GetFlag() << " " << a.GetPrim() << " " << a.GetKick() << " " << a.GetLen() << " " << a.GetVal() << endl;
//         } else if (s == "cmp") {
//             cout << (Analysis(cs[0], rule) > Analysis(cs[1], rule)) << endl;
//         } else if (s[0] == 'r') {
//             rule = (Rule)(s[1] - '0');
//         } else {
//             for (char c: s) {
//                 int r;
//                 if (c >= '2' && i <= '9') r = c - '0';
//                 if (c == 'X') r = 10;
//                 if (c == 'J') r = J;
//                 if (c == 'Q') r = Q;
//                 if (c == 'K') r = K;
//                 if (c == 'A') r = A;
//                 if (c == 'B') r = B;
//                 if (c == 'R') r = R;
//                 if (r == B || r == R) {
//                     cs[i].Insert(joker, r);
//                 } else {
//                     cs[i].Insert(suit, r);
//                 }
//             }
//         }
//     }
//     return 0;
// }
