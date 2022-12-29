//
// Created by yinxuhao on 2022/12/29.
//

#include "chapter2.h"

using namespace std;

void get_bit_length(int i, int &a, int &b) {
    auto x = (unsigned long long) i;
    while (x) {
        x >>= 1;
        ++a;
    }
    auto y = (unsigned) i;
    while (y) {
        y >>= 1;
        ++b;
    }
}

bool int_shifts_are_arithmetic() {
    int a = 0;
    int b = 0;
    get_bit_length(-1, a, b);
    unsigned i = -1;
    int x = (int) i;
    int shift = a - b + 4;
//    return x / 2 != x >> 1;
    return x << shift >> shift == i;
}
