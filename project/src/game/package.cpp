#include <string>

#include "package.h"

using namespace std;

Header::Header(bool s, int r, int sp):
    suc(s), rec(r), spe(sp)
{}
bool Header::IsSuccess() const {
    return suc;
}
int Header::GetRecipient() const {
    return rec;
}
int Header::GetSpecial() const {
    return spe;
}

Package::Package(const Header &h, const string &d):
    head(h), data(d)
{}
Header Package::GetHeader() const {
    return head;
}
string Package::GetData() const {
    return data;
}
const string &Package::GetDataR() const {
    return data;
}

// test code, expected all 1's

// #include <iostream>
// int main() {
//     Package p(Header(true, i_server), string("abc"));
//     Header h = p.GetHeader();
//     cout
//         << h.IsSuccess()
//         << (h.GetRecipient() == i_server)
//         << (h.GetSpecial() == 0)
//         << (p.GetData() == string("abc"))
//         << (p.GetDataR() == string("abc"))
//     ;
//     return 0;
// }
