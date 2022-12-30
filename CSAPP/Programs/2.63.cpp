//
// Created by yinxuhao on 2022/12/30.
// Exercise 2.63 ***
//

#include "chapter2.h"

using namespace std;

unsigned srl(unsigned x, int k) {
    /* Perform shift arithmetically */
    unsigned xsra = (int) x >> k;
    /* Begin solve */
    int int_bits = 8 * sizeof(int);
    unsigned xsla = 1 << (int_bits - k);
    unsigned mask = (INT_MAX + xsla) << 1;
    return xsra & mask;
}

unsigned sra(int x, int k) {
    /* Perform shift logically */
    int xsrl = (unsigned) x >> k;
    /* Begin solve */
    int int_bits = 8 * sizeof(int);
    unsigned negative = -1 * ((x | INT_MIN) == x);
    unsigned mask = negative << (int_bits - k);
    return xsrl | mask;
}
