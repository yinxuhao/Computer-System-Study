---
author:
- yinxuhao \[xuhao_yin@163.com\]
date: 2022-12-30
title: 第二章习题编程
---

位级整数编码规则

在接下来的作业中，我们特意限制了你能使用的编程结构，
来帮你更好地理解C语言的位级、逻辑和算术运算。在回答
这些问题时，你的代码必须遵守以下规则：

::: description
假设

整数用补码形式表示。

有符号数的右移是算术右移。

禁止使用

条件语句(if 或者 ?:)、循环、分支语句、函数调用和宏调用。

除法、模运算和乘法。

相对比较运算(\<、\>、\<= 和\>=)。

允许的运算

所有的位级和逻辑运算。

左移和右移，但是位移量只能在0和$w-1$之间。

加法和减法。

对int和unsigned进行强制类型转换。
:::

# 2.61

写一个C表达式，在下列描述的条件下产生1，而在其他情况下
得到0。假设x是int类型。

**限制条件**

1\. 遵循位级整数编码规则

2\. 不能使用`==`和`!=`测试。

## x的任何位都等于1.

\|unsigned y = -1; int x = (int)y;\|

## x的任何位都等于0.

\|int x = 0;\|

## x的最低有效字节中的位都等于1.

!x = x \| 17;!

## x的最高有效字节中的位都等于0.

\|x = x & ((unsigned) -1 \>\> 8);\|

# 2.62

编写一个函数`int_shifts_are_arithmetic()`，
在对int类型的数使用算术右移的机器上运行时这个函数生成1，
而在其他情况下生成0.你的代码应该可以运行在任何字长的机器
上。在几种机器上测试你的代码。

``` C++
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
```

# 2.63

将下面的C函数代码补充完整。函数srl用算术右移(由值xsra给出)
来完成逻辑右移，后面的其他操作不包括右移或者除法。函数sra
用逻辑右移(由值xsrl给出)来完成算术右移，后面的其他操作不包括
右移或者除法。可以通过计算`8*sizeof(int)`来确定数据类型
int中的位数w。位移量k的取值范围为0$\sim$w-1。

``` C++
//
// Created by yinxuhao on 2022/12/30.
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
```
