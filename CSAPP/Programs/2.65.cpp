//
// Created by yinxuhao on 2023/1/1.
// Exercise 2.65 ****
//

#include "chapter2.h"

using namespace std;

int odd_ones(unsigned x) {
    unsigned y = ~x;
    unsigned t = y >> 16;
    y ^= t;
    t = y >> 8;
    y ^= t;
    t = y >> 4;
    y ^= t;
    t = y >> 2;
    y ^= t;
    t = y >> 1;
    y ^= t;
    return (int) (y & 1);
}
