#ifndef CLOCKINT_H
#define CLOCKINT_H

template <int m> class ci {
    int val;
public:
    ci(int x = 0); // x must be in [0, m)
    operator int() const;
    ci<m> &operator++();
    ci<m> operator++(int);
    ci<m> &operator--();
    ci<m> operator--(int);
    ci<m> &operator=(int x); // x can be any integer
    ci<m> &operator+=(int x);
    ci<m> &operator-=(int x);
    ci<m> operator+(int x) const;
    ci<m> operator-(int x) const;
};
template class ci<3>;
template class ci<4>;

#endif
