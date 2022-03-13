#include <assert.h>
#include <string>
#include <array>

#include "clockint.h"
#include "card.h"
#include "message.h"

using namespace std;

Message::Message(MsgType t, bool b):
    type(t),
    isr(b),
    spar(string(8, 0)),
    scard(string(54, 0)),
    ext(string())
{}
Message::Message(const string &s):
    type((assert(s.size() >= 64), (MsgType)s[0])),
    isr(s[1]),
    spar(string(s, 2, 8)),
    scard(string(s, 10, 54)),
    ext(string(s, 64))
{}
MsgType Message::GetType() const {
    return type;
}
bool Message::IsRequest() const {
    return isr;
}
int Message::GetPar(int k) const {
    assert(k >= 0 && k < 8);
    return spar[k];
}
int Message::GetPlayer() const {
    return GetPar(7);
}
int Message::GetTime() const {
    return GetPar(6);
}
CardSet Message::GetCards() const {
    return CardSet(scard);
}
string Message::GetExtension() const {
    return ext;
}
const string &Message::GetExtensionR() const {
    return ext;
}
int Message::GetInt(int k) const {
    assert(k >= 0 && (unsigned)k < ext.size() / 4);
    int sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += (unsigned char)ext[4 * k + i] << 8 * i;
    }
    return sum;
}
void Message::SetType(MsgType t) {
    type = t;
}
void Message::SetIsRequest(bool b) {
    isr = b;
}
void Message::SetPar(int k, int v) {
    assert(k >= 0 && k < 8);
    assert((char)v == v);
    spar[k] = (char)v;
}
void Message::SetPlayer(int p) {
    SetPar(7, p);
}
void Message::SetTime(int t) {
    SetPar(6, t);
}
void Message::SetCards(const CardSet &s) {
    scard = s.String();
}
void Message::SetExtension(const string &s) {
    ext = s;
}
void Message::SetInt(int k, int v) {
    if (ext.size() < (unsigned)4 * (k + 1)) {
        ext.resize(4 * (k + 1));
    }
    for (int i = 0; i < 4; i++) {
        ext[4 * k + i] = v;
        v >>= 8;
    }
}
string Message::String() const {
    return string({(char)type, (char)isr}) + spar + scard + ext;
}

template <int np> MsgSeries<np>::MsgSeries(const Message &m) {
    this->fill(m);
}
template <int np> void MsgSeries<np>::SetPar(int k, array<int, np> a) {
    for (int i = 0; i < np; i++) {
        (*this)[i].SetPar(k, a[i]);
    }
}
template <int np> void MsgSeries<np>::SetPar(int k, ci<np> v) {
    for (int i = 0; i < np; i++, v--) {
        (*this)[i].SetPar(k, v);
    }
}
template <int np> void MsgSeries<np>::SetPars(int k, array<int, np> a) {
    for (int i = 0; i < np; i++) {
        for (int j = 0; j < np; j++) {
            (*this)[i].SetPar(k + j, a[ci<np>(i) + j]);
        }
    }
}
template <int np> void MsgSeries<np>::SetPlayer(ci<np> v) {
    SetPar(7, v);
}
template <int np> void MsgSeries<np>::SetCards(const array<CardSet, np> &a) {
    for (int i = 0; i < np; i++) {
        (*this)[i].SetCards(a[i]);
    }
}
template <int np> void MsgSeries<np>::SetInt(int k, array<int, np> a) {
    for (int i = 0; i < np; i++) {
        (*this)[i].SetInt(k, a[i]);
    }
}
template <int np> void MsgSeries<np>::SetInts(int k, array<int, np> a) {
    for (int i = 0; i < np; i++) {
        for (int j = 0; j < np; j++) {
            (*this)[i].SetInt(k + j, a[ci<np>(i) + j]);
        }
    }
}

// test code, expected all 1's

// #include <iostream>
// #include "clockint.cpp"
// #include "card.cpp"
// int main() {
//     Message m;
//     m.SetType(m_playout);
//     m.SetIsRequest(false);
//     m.SetPar(0, 13);
//     m.SetPar(1, 5);
//     m.SetPlayer(2);
//     m.SetTime(20);
//     CardSet s;
//     for (int i = 3; i <= 8; i++) s.Insert(Card(i));
//     m.SetCards(s);
//     m.SetExtension(string("abc"));
//     m = Message(m.String());
//     cout
//         << (m.GetType() == m_playout)
//         << (m.IsRequest() == false)
//         << (m.GetPar() == 13)
//         << (m.GetPar(1) == 5)
//         << (m.GetPlayer() == 2)
//         << (m.GetTime() == 20)
//         << (m.GetCards() == s)
//         << (m.GetExtension() == string("abc"))
//     ;
//     m.SetInt(0, 123456789);
//     m.SetInt(2, -987654321);
//     cout << (
//         m.GetInt() == 123456789 &&
//         m.GetInt(2) == -987654321
//     );
//     MsgSeries<4> ms(m);
//     cout << (ms[2].String() == m.String());
//     ms.SetPar(0, array<int, 4>{5, 6, 7, 8});
//     cout << (
//         ms[0].GetPar() == 5 &&
//         ms[1].GetPar() == 6 &&
//         ms[2].GetPar() == 7 &&
//         ms[3].GetPar() == 8
//     );
//     ms.SetPar(0, ci<4>(2));
//     cout << (
//         ms[0].GetPar() == 2 &&
//         ms[1].GetPar() == 1 &&
//         ms[2].GetPar() == 0 &&
//         ms[3].GetPar() == 3
//     );
//     ms.SetPars(0, array<int, 4>{1, 2, 3, 4});
//     cout << (
//         ms[0].GetPar() == 1 &&
//         ms[0].GetPar(1) == 2 &&
//         ms[0].GetPar(2) == 3 &&
//         ms[0].GetPar(3) == 4 &&
//         ms[1].GetPar() == 2 &&
//         ms[1].GetPar(1) == 3 &&
//         ms[2].GetPar() == 3 &&
//         ms[2].GetPar(1) == 4
//     );
//     ms.SetInts(0, array<int, 4>{-1, -2, -3, -4});
//     cout << (
//         ms[0].GetInt() == -1 &&
//         ms[0].GetInt(1) == -2 &&
//         ms[0].GetInt(2) == -3 &&
//         ms[0].GetInt(3) == -4 &&
//         ms[1].GetInt() == -2 &&
//         ms[1].GetInt(1) == -3 &&
//         ms[2].GetInt() == -3 &&
//         ms[2].GetInt(1) == -4
//     );
//     ms.SetPlayer(ci<4>(0));
//     cout << (
//         ms[0].GetPlayer() == 0 &&
//         ms[1].GetPlayer() == 3 &&
//         ms[2].GetPlayer() == 2 &&
//         ms[3].GetPlayer() == 1
//     );
//     CardSet t, u, v;
//     for (int i = 5; i <= 9; i++) t.Insert(Card(i));
//     for (int i = 4; i <= 13; i++) u.Insert(Card(i));
//     for (int i = 2; i <= 3; i++) v.Insert(Card(i));
//     ms.SetCards(array<CardSet, 4>{s, t, u, v});
//     cout << (
//         ms[0].GetCards() == s &&
//         ms[1].GetCards() == t &&
//         ms[2].GetCards() == u &&
//         ms[3].GetCards() == v
//     );
//     return 0;
// }
