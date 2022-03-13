#define WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <array>
#include <string>

#include "Server.h"

#include "clockint.h"
#include "card.h"
#include "rule.h"
#include "message.h"
#include "package.h"
#include "game.h"

using namespace std;

template <int np> void CardGame<np>::notify(ci<np> k, const Message &m) const {
    if (ishuman[k]) {
        server.SendGameMsg(Package(Header(true, k), m.String()));
    }
}
template <int np> Message CardGame<np>::request(ci<np> k) const {
    Package p = server.CollectGameMsg(k);
    if (!p.GetHeader().IsSuccess()) {
        throw k;
    }
    return Message(p.GetData());
}
template <int np> void CardGame<np>::broadcast(const Message &m) const {
    for (int i = 0; i < np; i++) {
        notify(i, m);
    }
}
template <int np> void CardGame<np>::broadcast(const MsgSeries<np> &ms) const {
    for (int i = 0; i < np; i++) {
        notify(i, ms[i]);
    }
}
template <int np> void CardGame<np>::disptext(const string &s) const {
    Message m(m_disptext);
    m.SetExtension(s);
    broadcast(m);
}
template <int np> void CardGame<np>::dispeffect(const string &s) const {
    Message m(m_dispeffect);
    m.SetPlayer(np);
    m.SetExtension(s);
    broadcast(m);
}
template <int np> void CardGame<np>::dispeffect(ci<np> k, const string &s) const {
    Message m(m_dispeffect);
    m.SetExtension(s);
    MsgSeries<np> ms(m);
    ms.SetPlayer(k);
    broadcast(ms);
}
template <int np> void CardGame<np>::dispscore() const {
    Message m(m_dispscore);
    MsgSeries<np> ms(m);
    ms.SetInts(0, score);
    broadcast(ms);
}
template <int np> void CardGame<np>::think(ci<np> k) const {
    Message m(m_think);
    MsgSeries<np> ms(m);
    ms.SetPlayer(k);
    broadcast(ms);
}
template <int np> CardGame<np>::CardGame(array<bool, np> ish, Server &s, const CardSet &c):
    ishuman(ish), server(s), ccards(c)
{
    srand(time(0));
}
template <int np> void CardGame<np>::Play() {
    broadcast(Message(m_start));
    try {
        play();
        Sleep(1000);
        dispscore();
    } catch (ci<np> k) {
        Message m(m_box);
        m.SetExtension(string("玩家“") + server.GetClientNameR(k) + string("”掉线，游戏中止。"));
        broadcast(m);
    } catch (exception &e) {
        Message m(m_box);
        m.SetExtension(string("游戏主程序出现未知错误，游戏中止。错误信息：\n") + string(e.what()));
        broadcast(m);
    }
    broadcast(Message(m_end));
}

template <int np> void WinnerBasedGame<np>::deal(array<int, np> noc) {
    tcards = this->ccards;
    for (int i = 0; i < np; i++) {
        cards[i] = tcards.Take(noc[i]);
    }
    Message m(m_deal);
    m.SetPar(np, tcards.GetNumOfCards());
    MsgSeries<np> ms(m);
    ms.SetPars(0, noc);
    ms.SetCards(cards);
    this->broadcast(ms);
}
template <int np> void WinnerBasedGame<np>::changestake(int s) {
    Message m(m_changestake);
    m.SetPar(0, stake = s);
    this->broadcast(m);
}
template <int np> void WinnerBasedGame<np>::playout(ci<np> k) {
    this->think(k);
    CardSet c;
    if (this->ishuman[k]) {
        Message m(m_playout, true);
        m.SetPar(0, npasses < np - 1);
        m.SetTime(t_playout);
        this->notify(k, m);
        while (true) {
            m = this->request(k);
            c = m.GetCards();
            if (Analysis(c, rule) > last) {
                break;
            }
            this->notify(k, Message(m_deny));
        }
    } else {
        c = playout_robot(k);
    }
    tcards.Insert(c);
    cards[k].Delete(c);
    Message m(m_playout);
    m.SetPar(0, cards[k].GetNumOfCards());
    m.SetCards(c);
    MsgSeries<np> ms(m);
    ms.SetPlayer(k);
    this->broadcast(ms);
    if (c) {
        npasses = 0;
        last = Analysis(c, rule);
        if (last.GetFlag() >= 4) {
            this->dispeffect(k, "bomb");
        } else if (last.GetLen() > 1) {
            if (last.GetPrim() >= 3) {
                this->dispeffect(k, "airplane");
            } else {
                this->dispeffect(k, "sequence");
            }
        }
    } else if(++npasses == np - 1) {
        last = {};
    }
}
template <int np> WinnerBasedGame<np>::WinnerBasedGame(
    array<bool, np> ish, Server &s, const CardSet &c, const Rule r
):
    CardGame<np>(ish, s, c), rule(r)
{}

void DouDizhuGame::play() {
    changestake(0);
    last = {};
    while (!stake) {
        deal(array<int, 3>{17, 17, 17});
        Sleep(1000);
        starter = rand();
        npasses = 0;
        for (ci<3> i = starter; npasses < 3; i++) {
            think(i);
            int s;
            if (ishuman[i]) {
                Message m(m_bid, true);
                m.SetPar(1, stake + 1);
                m.SetPar(2, 3);
                m.SetTime(t_bid);
                notify(i, m);
                s = request(i).GetPar();
            } else {
                s = bid_robot(i);
            }
            Message m(m_bid);
            m.SetPar(0, s);
            MsgSeries<3> ms(m);
            ms.SetPlayer(i);
            broadcast(ms);
            if (s) {
                changestake(s);
                if (s == 3) {
                    starter = i;
                    break;
                }
                npasses = 0;
            } else if (++npasses == 2 && stake) {
                starter = i + 1;
                break;
            }
        }
    }
    Message m(m_setlandlord);
    m.SetPar(0, 20);
    m.SetCards(tcards);
    MsgSeries<3> ms(m);
    ms.SetPlayer(starter);
    broadcast(ms);
    cards[starter].Insert(tcards);
    tcards = CardSet();
    Sleep(1000);
    npasses = 2;
    int sprref = 20;
    for (ci<3> i = starter; ; i++) {
        playout(i);
        if (sprref == 20) {
            sprref = cards[starter].GetNumOfCards();
        }
        if (last.GetFlag() >= 4) {
            changestake(2 * stake);
        }
        if (!cards[i]) {
            if (
                i == starter &&
                cards[i + 1].GetNumOfCards() == 17 &&
                cards[i + 2].GetNumOfCards() == 17
            ||
                i != starter &&
                cards[starter].GetNumOfCards() == sprref
            ) {
                dispeffect("spring");
                changestake(2 * stake);
            }
            int sgn = i == starter? 1: -1;
            score[starter] = 2 * sgn  * stake;
            for (int j = 1; j < 3; j++) {
                score[starter + j] = -sgn  * stake;
            }
            break;
        }
    }
}
DouDizhuGame::DouDizhuGame(array<bool, 3> ish, Server &s):
    WinnerBasedGame<3>(ish, s, CardSet(1), r_1v2_wiki)
{}

void SirenDouDizhuGame::play() {
    changestake(0);
    last = {};
    while (!stake) {
        deal(array<int, 4>{25, 25, 25, 25});
        Sleep(1000);
        starter = rand();
        for (int j = 0; j < 4; j++) {
            ci<4> i(starter + j);
            think(i);
            int s;
            if (ishuman[i]) {
                Message m(m_bid, true);
                m.SetPar(1, stake + 1);
                m.SetPar(2, 3);
                m.SetTime(t_bid);
                notify(i, m);
                s = request(i).GetPar();
            } else {
                s = bid_robot(i);
            }
            Message m(m_bid);
            m.SetPar(0, s);
            MsgSeries<4> ms(m);
            ms.SetPlayer(i);
            broadcast(ms);
            if (s) {
                changestake(s);
                starter = i;
                if (s == 3) {
                    break;
                }
            }
        }
    }
    Message m(m_setlandlord);
    m.SetPar(0, 33);
    m.SetCards(tcards);
    MsgSeries<4> ms(m);
    ms.SetPlayer(starter);
    broadcast(ms);
    cards[starter].Insert(tcards);
    tcards = CardSet();
    Sleep(1000);
    npasses = 3;
    for (ci<4> i = starter; ; i++) {
        playout(i);
        if (last.GetFlag() >= 6) {
            changestake(2 * stake);
        }
        if (!cards[i]) {
            int sgn = i == starter? 1: -1;
            score[starter] = 3 * sgn * stake;
            for (int j = 1; j < 4; j++) {
                score[starter + j] = -sgn * stake;
            }
            break;
        }
    }
}
SirenDouDizhuGame::SirenDouDizhuGame(array<bool, 4> ish, Server &s):
    WinnerBasedGame<4>(ish, s, CardSet(2), r_1v3)
{}

void ShuangkouGame::play() {
    changestake(1);
    last = {};
    deal(array<int, 4>{27, 27, 27, 27});
    score.fill(-3);
    npasses = 3;
    starter = rand();
    int nleft = 4;
    for (ci<4> i = starter; ; i++) {
        if (cards[i]) {
            playout(i);
            int s = last.GetFlag();
            if (s == nuke) {
                s = 5;
            } else {
                s -= 3;
            }
            if (s > stake) {
                changestake(s);
            }
            if (!cards[i]) {
                score[i] += --nleft;
                score[i + 2] += nleft;
                if (!cards[i + 2]) {
                    score[i + 1] = -score[i];
                    score[i + 3] = -score[i];
                    for (int i = 0; i < 4; i++) {
                        score[i] *= stake;
                    }
                    break;
                }
            }
        } else if (++npasses == 3) {
            last = {};
        } else if (npasses == 4) {
            i++;
        }
    }
}
ShuangkouGame::ShuangkouGame(array<bool, 4> ish, Server &s):
    WinnerBasedGame<4>(ish, s, CardSet(2), r_2v2)
{}

// test code

// #include <iostream>
// #include "clockint.cpp"
// #include "card.cpp"
// #include "rule.cpp"
// #include "message.cpp"
// #include "package.cpp"
// #include "game_robot.cpp"
// using namespace std;
// int n;
// CardSet cards[4];
// Message reply;
// void disp(const CardSet &s) {
//     int sum[15] = {};
//     for (int i = 0; i < 54; i++) sum[Card(i).GetRank()] += s.GetNum(Card(i));
//     for (int i = 0; i < sum[R]; i++) cout << 'R';
//     for (int i = 0; i < sum[B]; i++) cout << 'B';
//     for (int i = 0; i < sum[2]; i++) cout << 2;
//     for (int i = 0; i < sum[A]; i++) cout << 'A';
//     for (int i = 0; i < sum[K]; i++) cout << 'K';
//     for (int i = 0; i < sum[Q]; i++) cout << 'Q';
//     for (int i = 0; i < sum[J]; i++) cout << 'J';
//     for (int i = 0; i < sum[10]; i++) cout << 'X';
//     for (int i = 9; i >= 3; i--) for (int j = 0; j < sum[i]; j++) cout << i;
// }
// CardSet from(CardSet u) {
//     CardSet cs;
//     string s;
//     cin >> s;
//     if (s == "p") return cs;
//     for (char c: s) {
//         int r;
//         if (c >= '2' && c <= '9') r = c - '0';
//         if (c == 'X') r = 10;
//         if (c == 'J') r = J;
//         if (c == 'Q') r = Q;
//         if (c == 'K') r = K;
//         if (c == 'A') r = A;
//         if (c == 'B') r = B;
//         if (c == 'R') r = R;
//         if (r != B && r != R) {
//             for (int j = 0; j < 4; j++) if (u.GetNum(j, r)) {
//                 u.Delete(j, r);
//                 cs.Insert(j, r);
//                 break;
//             }
//         } else {
//             u.Delete(joker, r);
//             cs.Insert(joker, r);
//         }
//     }
//     return cs;
// }
// void Port::SendGameMsg(const Package &p) {
//     cout << "[" << p.GetHeader().GetRecipient() << "] ";
//     Message m(p.GetData());
//     int k, s;
//     switch (m.GetType()) {
//     case m_start:
//         cout << "start";
//         break;
//     case m_end:
//         cout << "end";
//         break;
//     case m_dispeffect:
//         cout << "effect: ";
//         k = m.GetPlayer();
//         if (k < n) cout << "(player " << k << ") ";
//         cout << m.GetExtension();
//         break;
//     case m_dispscore:
//         cout << "score: ";
//         for (int i = 0; i < n; i++) cout << m.GetInt(i) << " ";
//         break;
//     case m_think:
//         cout << "player " << m.GetPlayer() << " is thinking";
//         break;
//     case m_deal:
//         cout << "got cards: ";
//         cards[p.GetHeader().GetRecipient()] = m.GetCards();
//         disp(m.GetCards());
//         cout << " (";
//         for (int i = 0; i < n; i++) cout << m.GetPar(i) << ", ";
//         cout << m.GetPar(n) << ")";
//         break;
//     case m_bid:
//         if (m.IsRequest()) {
//             if (m.GetTime() != t_bid || m.GetPar(2) != 3) cout << "! ";
//             cout << "please bid 0 or >= " << m.GetPar(1) << ": ";
//             cin >> s;
//             m.SetPar(0, s);
//             reply = m;
//         } else {
//             cout << "player " << m.GetPlayer() << " bids " << m.GetPar();
//         }
//         break;
//     case m_changestake:
//         cout << "stake becomes " << m.GetPar();
//         break;
//     case m_setlandlord:
//         cout << "player " << m.GetPlayer() << " becomes the landlord; ";
//         disp(m.GetCards());
//         cout << " (" << m.GetPar() << ")";
//         if (!m.GetPlayer()) cards[p.GetHeader().GetRecipient()].Insert(m.GetCards());
//         break;
//     case m_playout:
//         if (m.IsRequest()) {
//             if (m.GetTime() != t_playout) cout << "! ";
//             if (!m.GetPar()) cout << "(cannot pass) ";
//             cout << "please play out from ";
//             disp(cards[p.GetHeader().GetRecipient()]);
//             cout << ": ";
//             m.SetCards(from(cards[p.GetHeader().GetRecipient()]));
//             reply = m;
//         } else {
//             cout << "player " << m.GetPlayer() << " plays out ";
//             disp(m.GetCards());
//             cout << " (" << m.GetPar() << ")";
//             if (!m.GetPlayer()) cards[p.GetHeader().GetRecipient()].Delete(m.GetCards());
//         }
//         break;
//     case m_deny:
//         cout << "please try again: ";
//         reply.SetCards(from(cards[p.GetHeader().GetRecipient()]));
//         break;
//     }
//     cout << endl;
// }
// Package Port::CollectGameMsg(int k) {
//     return Package(Header(true, -1), reply.String());
// }
// const string &Server::GetClientNameR(int k) const {}
// int main() {
//     Server s;
//     cout << "Dou dizhu" << endl;
//     n = 3;
//     DouDizhuGame(array<bool, 3>{true, true, true}, s).Play();
//     cout << endl;
//     Sleep(1000);
//     cout << "Siren Dou dizhu" << endl;
//     n = 4;
//     SirenDouDizhuGame(array<bool, 4>{true, true, true, true}, s).Play();
//     cout << endl;
//     Sleep(1000);
//     cout << "Shuangkou" << endl;
//     ShuangkouGame(array<bool, 4>{true, true, true, true}, s).Play();
//     cout << endl;
//     return 0;
// }
