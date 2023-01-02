//
// Created by yinxuhao on 2022/12/29.
// Exercise 2.62 ***
// 20230101: Get rid of the while loop because of the rule.
//

#include "chapter2.h"

using namespace std;

bool int_shifts_are_arithmetic() {
    int a = 8 * sizeof(int);
    int b = 8 * sizeof(long);
    //get_bit_length(-1, a, b);
    unsigned i = -1;
    int x = (int) i;
    int shift = a - b + 4;
    //return x / 2 != x >> 1;  /* method 1*/
    /* method 2 */
    return x << shift >> shift == i;
}
