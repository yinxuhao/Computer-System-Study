//
// Created by yinxuhao on 2022/12/30.
// Exercise 2.64 *
// 2022/12/31: Fix a bug
//

#include "chapter2.h"

using namespace std;

int any_odd_one(unsigned x) {
    /* define odd is read from right and
     * its count number start from 0 ,
     * so 101010 return 1 and also 000010
     * but not 10101 or 00001. */
    unsigned bit_1 = 1 << 1;
    unsigned bit_3 = 1 << 3;
    unsigned bit_5 = 1 << 5;
    unsigned bit_7 = 1 << 7;
    unsigned bit_9 = 1 << 9;
    unsigned bit_11 = 1 << 11;
    unsigned bit_13 = 1 << 13;
    unsigned bit_15 = 1 << 15;
    unsigned bit_17 = 1 << 17;
    unsigned bit_19 = 1 << 19;
    unsigned bit_21 = 1 << 21;
    unsigned bit_23 = 1 << 23;
    unsigned bit_25 = 1 << 25;
    unsigned bit_27 = 1 << 27;
    unsigned bit_29 = 1 << 29;
    unsigned bit_31 = 1 << 31;
    unsigned odd = bit_1 | bit_3 | bit_5 | bit_7 | bit_9 |
                   bit_11 | bit_13 | bit_15 | bit_17 | bit_19 |
                   bit_21 | bit_23 | bit_25 | bit_27 | bit_29 | bit_31;
    return (odd & x) != 0;
}
