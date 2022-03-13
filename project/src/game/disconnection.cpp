#include "clockint.h"
#include "disconnection.h"

template <int np> Disconn<np>::Disconn(ci<np> i): index(i) {}
template <int np> ci<np> Disconn<np>::GetIndex() const {
    return index;
}

// test code, expected 1

// #include <iostream>
// #include "clockint.cpp"
// using namespace std;
// int main() {
//     cout << (Disconn<4>(2).GetIndex() == 2);
//     return 0;
// }
