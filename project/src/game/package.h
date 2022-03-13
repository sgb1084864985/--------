#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>

using namespace std;

constexpr int i_server = -1;

class Header {
    bool suc;
    int rec;
    int spe;
public:
    Header(bool s, int r, int sp = 0); // returns a header of success state s and recipient r
    // When a function with return type Package is unable to return the required package, it should return a package with false success code and empty payload
    bool IsSuccess() const; // returns the success code
    int GetRecipient() const; // returns the index of recipient defined by the server
    int GetSpecial() const; // reserved
};

class Package {
    Header head;
    string data;
public:
    Package(const Header &h, const string &d); // returns a package with h as header and d as payload data
    Header GetHeader() const; // returns the header
    string GetData() const; // returns the payload data
    const string &GetDataR() const; // returns the payload data by reference
};

#endif
