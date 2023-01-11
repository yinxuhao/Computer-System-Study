---
author:
- yinxuhao \[xuhao_yin@163.com\]
date: 2023-01-11
title: 第二单元学习笔记
---

信息的表示和处理

# 引言

孤立地讲，**单个的位不是非常有用，将位组合在一起，再加上某种解释
(interpretation)，即赋予不同的可能位模式以含意**。我们就能表示任何有限
集合的元素。

三种重要的数字表示：

1\. **无符号**`unsigned`编码给予传统的二进制表示法

2\. **补码**`two's-complement`编码是表示有符号整数的最常见的方式。

3\.
**浮点数**`floating-point`编码是表示实数的科学计数法的以2为基数的版本。

数据**溢出**`overflow`是产生bug的一大原因。负数下溢产生极大的正数；
正数上溢产生极小的负数。

浮点运算有完全不同的数学属性。

1\. **由于表示的精度有限，浮点运算是不可结合的**。 例如
$$(3.14+1e_{20})-1e_{20} = 0.0$$ but $$(3.14+1e_{20}-1e_{20}) = 3.14$$

2\. 该属性不同的原因，是处理数字表示有限性的方式不同------

*书中建议的本章学习方式*：

[深入学习数学语言]{.underline}

[学习编写公式和方程式]{.underline}

[以及重要属性的推导]{.underline}

# 信息存储

大多数计算机**使用8位的块或者字节作为最小的可寻址内存单位**，而不是内存
中单独的比特。

机器级程序将内存视为一个非常大的字节数组，称为**虚拟内存**，所有可能
的地址的集合称为**虚拟地址空间**`virtual address space`.

每个程序对象可以简单地视为一个字节块，而程序本身就是一个字节序列。

## 十六进制表示法

![](hex_number.png){#fig:hex_number-png width="80%"}

十六进制转二进制： 将十六进制的每一位转换为二进制格式，然后拼接。 例如：

  ---------- ------ ------ ------ ------ ------ ------
  十六进制        1      7      3      A      4      C
  二进制       0001   0111   0011   1010   0100   1100
  ---------- ------ ------ ------ ------ ------ ------

所以$binary_{0x173a4c_{16}} = 000101110011101001001100_2$。

二进制转十六进制：
将二进制从右到左做4个一组的划分，如最左侧不足4位则以0补之。
然后将每个4位转换为对应的十六进制数字拼接即可。 例如：

  ---------- ---- ------ ------ ------ ------ ------
  二进制       11   1100   1010   1101   1011   0011
  十六进制      3      C      A      D      B      3
  ---------- ---- ------ ------ ------ ------ ------

所以，$hex_{1111001010110110110011_2} = 3cadb3_{16}$

## 字数据大小

每台计算机都有一个字长，指明指针数据的标称大小。

C数据类型的典型大小见下图：

![基本C数据类型的典型大小(以字节为单位)](size.png){#fig:size-png
width="80%"}

## 寻址和字节顺序

**小端法**`little endian`: 最低有效字节在最前面放着。

**大端法**`big endian`: 最高有效字节在最前面放着。

具体示例见下图：

![大端法与小端法](endian.png){#fig:endian-png width="80%"}

``` C
    #include <stdio.h>

    typedef unsigned char *byte_pointer;

    void show_bytes(byte_pointer start, size_t len) {
        size_t i;
        for(i = 0; i < len; i++) {
            printf(" %.2x", start[i]);
        }
        printf("\n");
     }

     void show_int(int x) {
        show_bytes((byte_pointer) &x, sizeof(int));
    }

    void show_float(float x);

    void show_pointer(void *x);

    void test_show_bytes(int val) {
        int ival = val;
        float fval = (float) val;
        int *pval = &ival;
        show_int(ival);
        show_float(fval);
        show_pointer(pval);
    }
```

通过以上代码，可以打印出数据的两位十六进制格式输出。
对比结果可以发现，`int`和`float`的结果一样，只是排列的
大小端不同，而指针值不同，与机器相关。

二进制代码是不兼容的。

## 布尔代数

![布尔代数的运算。二进制0和1代表逻辑值TRUE和FALSE.
以上四张图依次是逻辑运算符NOT AND OR
EXCLUSIVE-OR](bitwise.png){#fig:bitwise-png width="80%"}

位向量一个很有用的应用就是**表示有限集合**。利用位向量
$[a_{w-1}, \ldots, a_1, a_0]$可以编码任何子集$A \in {0, 1, \ldots, w - 1}$。

例如，定义规则$a_i = 1 \iff i \in A$。

位向量$a \dot{=} [01101001]$表示集合$A = {0, 3, 5, 6}$，
而位向量$b \dot{=} [01010101]$表示集合$B = {0, 2, 4, 6}$。

编码集合的使用方法是使用布尔运算。

例如：$a \& b \rightarrow [010000001]$, 对应于$A \cap B = {0, 6}$。

它的实际应用，还有使用位向量作为掩码有选择地使用或屏蔽一些信号，
该掩码就是设置为有效信号的集合。

C语言中的位级运算，其实是按照各个位对应的位运算来的。

而C语言中的逻辑运算(\|\|、`&&`、`!`)则是把所有的
非零参数都表示TRUE，参数0表示为FALSE。它们只返回1或0.
而位级运算只在参数特殊时才与之有相同的结果。

## 移位运算

`x<<k`: 左移k位，即丢弃最高k位，右端补充k个0.

`x>>k`: 右移k位，支持逻辑右移和算术右移。
逻辑右移在左端补充k个0，算术右移则在左端补充k个最高有效位(符号位)。

**对无符号数，右移必须是逻辑的**。

**移位运算符是从左至右可结合的**。

# 整数表示

![整数的数据与算术操作术语。下标w表示数据中表示中的位数](terms.png){#fig:terms-png
width="80%"}

## 无符号数的编码

::: theorem
**原理 1**. *无符号数编码的定义*

*对向量$\vec{x} = [x_{w-1}, x_{w-2}, \ldots, x_{0}]$:
$$B2U_w(\vec{x}) \dot{=} \sum_{i=0}^{w-1}x_i2^i \refstepcounter{equation}\tag{\theequation}\label{a}$$*
:::

形象的展示如下图：

![w=4的无符号数示例。当二进制表示中位i为1，数值就会相应加上$2^i$](unsi.png){#fig:unsi-png
width="80%"}

::: theorem
**原理 2**. *无符号数编码的唯一性*

*函数$B2U_w$是一个双射*
:::

## 补码编码

::: theorem
**原理 3**. *补码编码的定义*

*对向量$\vec{x} = [x_{w-1}, x_{w-2}, \ldots, x_0]$:
$$B2T_w(\vec{x}) \dot{=} -x_{w-1}2^{w-1} + \sum_{i=0}^{w-2}x_i2^i \refstepcounter{equation}\tag{\theequation}\label{b}$$*
:::

形象地展示如下图：

![w=4的补码示例。把位3作为符号位，因此当它为1时，对数值的影响
是$-2^3=-8$。这个权重在图中用带向左箭头的条表示](twop.png){#fig:twop-png
width="80%"}

::: theorem
**原理 4**. *补码编码的唯一性*

*函数$B2T_w$是一个双射。*
:::

1\.
补码的范围是不对称的：$|TMin| = |TMax| + 1$，即TMin没有与之对应的正数。
这是因为0是非负数。

2\. 最大的无符号数值刚好比补码的最大值的两倍大一点：$UMax_w = 2TMax_w+1$

## 有符号和无符号数之间的转换

::: theorem
**原理 5**. *补码转换为无符号数*

*对满足$TMin_w \le x \le TMax_w$的x有： $$T2U_w(x) = \left\{
            \begin{array}{ll}
                x+2^w, & x < 0 \\
                x, & x\ge 0
            \end{array}
        \right. \refstepcounter{equation}\tag{\theequation}\label{c}$$*
:::

::: proof
*Proof.* 补码转换为无符号数

比较式[\[a\]](#a){reference-type="ref"
reference="a"}和[\[b\]](#b){reference-type="ref"
reference="b"}，我们发现对于位模式$\vec{x}$，
如果我们计算$B2U_w(\vec{x}) - B2T_w(\vec{x})$之差，得到：
$$B2U_w(\vec{x}) - B2T_w(\vec{x}) = x_{w-1}2^w$$ 由此得到一个关系：
$$B2U_w(\vec{x}) = x_{w-1}2^w + B2T_w(\vec{x}) \refstepcounter{equation}\tag{\theequation}\label{d}$$
由此可得：
$$B2U_w(T2B_w(x)) = T2U_w(x) = x + x_{w-1}2^w \refstepcounter{equation}\tag{\theequation}\label{e}$$
式[\[e\]](#e){reference-type="ref"
reference="e"}的计算：将$T2B_w(x)$当作x代入[\[d\]](#d){reference-type="ref"
reference="d"}后得到。 由于运算$T2B_w$与$B2T_w$是对$\vec{x}$的逆运算，故
$$\because B2U_w(T2B_w(x)) = x_{w-1}2^w + B2T_w(T2B_w(x))\\
            \therefore T2U_w(x) = x + x_{w-1}2^w$$
根据[\[c\]](#c){reference-type="ref"
reference="c"}的两种情况，在x的补码中，位$x_{w-1}$决定了x是否为负。
$\blacksquare$ ◻
:::

![比较当w=4时无符号数表示和补码表示(对补码和无符号数来说，
最高有效位的权重分别是-8和+8，因此产生一个差为16)](cmp.png){#fig:cmp-png
width="80%"}

::: theorem
**原理 6**. *无符号数转换为补码*

*对满足$0 \le x \le UMax_w$的u有： $$U2T_w(u) = \left\{
                \begin{array}{ll}
                    u, & u \le TMax_w \\
                    u-2^w, & u >TMax
                \end{array}
            \right. \refstepcounter{equation}\tag{\theequation}\label{1}$$*
:::

::: proof
*Proof.* 设$\vec{x} = U2B_w(u)$，则这个位向量也是$U2T_w(u)$的补码表示。
式[\[a\]](#a){reference-type="ref"
reference="a"}和式[\[b\]](#b){reference-type="ref"
reference="b"}结合起来有
$$U2T_w(u) = -u_{w-1}2^w + u \refstepcounter{equation}\tag{\theequation}\label{2}$$
在u的无符号表示中，对式[\[1\]](#1){reference-type="ref"
reference="1"}的两种情况来说，位$u_{w-1}$决定
了u是否大于$TMax_w = 2^{w-1} - 1$。 $\blacksquare$ ◻
:::

以下图说明了函数U2T的行为。对于小的数，从无符号到有符号保留原值；
一旦大于$TMax_w$，数字将被转换为一个负数值。

![image](T2U.png){width="0.9\\linewidth"} []{#T2U label="T2U"}

![image](U2T.png){width="0.7\\linewidth"} []{#U2T label="U2T"}

## 扩展一个数字的位表示

用于将数据类型转换为一个更大的数据类型，例如32位$\rightarrow$`<!-- -->`{=html}64位。

::: theorem
**原理 7**. *无符号数的零扩展*

*定义宽度为w的位向量$\vec{u} = [u_{w-1}, u_{w-2}, \ldots, u_0]$和宽度为
$w^{\prime}$的位向量$\vec{u}^{\prime} = [\textcolor{blue}{0, \ldots, 0}, u_{w-1}, u_{w-1},
        \ldots, u_0]$，其中，$w^{\prime} > w$。则$B2U_w(\vec{u}) = B2U_{w^{\prime}}(\vec{u}^{\prime})$。*
:::

::: theorem
**原理 8**. *补码数的符号扩展*

*定义宽度为w的位向量$\vec{x} = [\textcolor{blue}{x_{w-1}}, x_{w-2}, \ldots, x_0]$
和宽度为w的位向量$\vec{x}^{\prime} = [\textcolor{blue}{x_{w-1}, \ldots, x_{w-1}, x_{w-1}}, x_{w-2}, \ldots, x_0]$，其中$w^{\prime} > w$。则$B2T_w(\vec{x}) = B2T_{w^{\prime}}(\vec{x}^{\prime})$。*
:::

::: proof
*Proof.* 补码数值的符号扩展

令$w^{\prime} = w + k$，证明
$$B2T_{w+k}([\underbrace{\textcolor{blue}{x_{w-1}, \ldots, x_{w-1}}}_{\textcolor{black}{k\ times}}, \textcolor{blue}{x_{w-1}}, x_{w-2}, \ldots, x_0]) = B2T_{w}([\textcolor{blue}{x_{w-1}}, x_{w-2}, \ldots, x_0])$$
下面的证明是对k进行归纳。即: **如果我们能够证明符号扩展一位
保持了数值不变，那么符号扩展任意位都能保持这种属性**。即：
$$B2T_{w+1}([\textcolor{blue}{x_{w-1}, x_{w-1}}, x_{w-2}, \ldots, x_0]) = B2T_w([\textcolor{blue}{x_{w-1}}, x_{w-2}, \ldots, x_0])$$
用式[\[b\]](#b){reference-type="ref"
reference="b"}展开左边的表达式，得：

$$\begin{aligned}
            B2T_{w+1}([\textcolor{blue}{x_{w-1}, x_{w-1}}, x_{w-2}, \ldots, x_0]) &= -\textcolor{blue}{x_{w-1}}2^w + \sum^{w-1}_{i=0}x_i2^i \\
                                                                                   &= - \textcolor{blue}{x_{w-1}}2^w + \textcolor{blue}{x_{w-1}}2^{w-1} + \sum_{i=0}^{w-2}x_i2^i \\
                                                                                   &= -\textcolor{blue}{x_{w-1}}(2^w - 2^{w-1}) + \sum_{i=0}^{w-2}x_i2^i \\
                                                                                   &= -\textcolor{blue}{x_{w-1}}2^{w-1} + \sum^{w-2}_{i=0}x_i2^i \\
                                                                                   &= B2T_w([\textcolor{blue}{x_{w-1}}, x_{w-2}, \ldots, x_0])
        .
\end{aligned}$$
其中使用的关键属性是$2^w - 2^{w-1} = 2^{w-1}$。$\blacksquare$ ◻
:::

## 截断数字

::: theorem
**原理 9**. *截断无符号数*

*令$\vec{x}$等于位向量$[x_{w-1}, x_{w-2}, \ldots, x_0]$，而$\vec{x}^{\prime}$是将其截断为k位的结果：$\vec{x}^{\prime} = [x_{k-1}, x_{k-2}, \ldots, x_0]$。
令$x = B2U_w(\vec{x}{^\prime})$。则$\vec{x}^{\prime} = x\ mod\ 2^k$。*
:::

::: proof
*Proof.* 截断补码数值

使用无符号数截断相同参数，则有
$$B2U_w([x_{w-1}, x_{w-2}, \ldots, x_0])\ mod\ 2^k = B2U_k[x_{k-1}, x_{k-2}, \ldots, x_0]$$
即，$x\ mod\ 2^k$能够被一个位级表示为$[x_{k-1}, x_{k-2}, \ldots, x_0]$
的无符号数表示。将其转换为补码数则有$x^{\prime} = U2T_k(x\ mod\ 2^k)$。
$\blacksquare$ ◻
:::

总结：

无符号数的截断结果：
$$B2U_k[x_{k-1}, x_{k-2}, \ldots, x_0] = B2U_w([x_{w-1}, x_{w-2}, \ldots, x_0])\ mod\ 2^k$$
补码数字的截断结果：
$$B2T_l[x_{k-1}, x_{k-2}, \ldots, x_0] = U2T_k(B2U_w([x_{w-1}, x_{w-2}, \ldots, x_0])\ mod\ 2^k)$$

## 关于有符号数和无符号数的建议

**有符号数到无符号数的隐式转换，会导致错误或者漏洞。**
避免这类错误的一种方法是绝不使用无符号数。(例如除C语言外，
少有语言支持无符号整数。)

但是**当我们想要把字仅仅看做是位的集合而没有任何
数字意义时，无符号数值是非常有用的。**

所以，见机行事。

# 整数运算

## 无符号加法

::: theorem
**原理 10**. *无符号数加法*

*对满足$0 \le x, y \le 2^w$的x和y有： $$x + ^u_wy = \left\{
                \begin{array}{lll}
                    x + y, & x + y < 2^w & Normal \\
                    x + y - 2^w, & 2^w \le x + y < 2^{w+1} & Overflow
                \end{array}
                \right.$$*
:::

::: proof
*Proof.* 无符号数加法

一般而言，我们可以看到，如果$x + y < 2^w$, 和的
$w + 1$位表示中最高位会等于0，因此丢弃它不会改变 这个数值。

另一方面，如果$2^w \le x + y < 2^{w+1}$，和的
$w + 1$位表示中的最高位会等于1，因此丢弃它就相当于 从和中减去了$2^w$。
$\blacksquare$ ◻
:::

形象表示见下图：

![无符号加法(4位字长，加法是模16的)](unsigned_add.png){#fig:unsigned_add-png
width="80%"}

整数加法和无符号加法着急拿的关系见下图：

![整数加法和无符号加法间的关系。当x+y大于$2^w-1$时，其和溢出](int_uni_relashiption.png){#fig:int_uni_relashiption-png
width="80%"}

::: theorem
**原理 11**. *检测无符号数加法中的溢出*

*对在范围$0 \le x, y \le UMax_w$中的x和y，令
$s \dot{=} x + ^u_wy$。则对计算s，当且仅当 $s < x$(或者等价的s \<
y)时，发生了溢出。*
:::

::: proof
*Proof.* 检测无符号数加法中的溢出

通过观察发现$x + y \ge x$，因此如果s没有溢出，我们 能够肯定$s \ge x$。

另一方面，如果s确实溢出了，我们就有$s = x + y - 2^w$。
假设$y < 2^w$，我们就有$y - 2^w < 0$,因此
$s = x + (y - 2^w) < x$。$\blacksquare$ ◻
:::

**模数加法形成了一种数学结构，称为阿贝尔群(Abelian goup),
它是[可交换的和可结合的]{.underline}。它有一个单位元0，并且
每个元素有一个加法逆元。**

::: theorem
**原理 12**. *无符号数求反*

*对满足$0 \le x < 2^w$的任意x，其w位的无符号逆元$-^u_wx$ 由下式给出：
$$-^u_wx = \left\{
            \begin{array}{ll}
                x, & x = 0 \\
                2^w - x, & x > 0
            \end{array}
                \right.$$*
:::

::: proof
*Proof.* 无符号数求反

当$x = 0$时，加法逆元显然是0。对于$x > 0$，考虑值
$2^w - x$。我们观察到这个数字在$0 < 2^w - x < 2^w$
范围之内，并且$(x + 2^w - x)\ mod\ 2^w = 2^w\ mod\ 2^w = 0$。
因此，它就是x在$+^u_w$下的逆元。$\blacksquare$ ◻
:::

## 补码加法

定义$x + ^t_wy$为整数和$x + y$被截断为$w$位的结果，
并将这个结果看做是**补码数**。

::: theorem
**原理 13**. *补码加法*

*对满足$-2^{w-1} \le x, y \le 2^{w-1} - 1$的整数x和y， 有：
$$x + ^t_wy = \left\{
                \begin{array}{lll}
                    x + y - 2^w, & 2^{w-1} \le x + y & Positive\ overflow \\
                    x + y, & -2^{w-1} \le x + y < 2^{w-1} & Normal \\
                    x + y + 2^w, & x + y < -2^{w-1} & Negative\ overflow
                \end{array}
                \right.$$*
:::

::: proof
*Proof.* 补码加法

由于补码加法和无符号数加法有相同的位级表示，故可以
按照如下步骤表示运算$+^t_w$：

1\. 将参数转换为无符号数

2\. 执行无符号数加法

3\. 将结果转换为补码

$$x + ^t_wy \dot{=} U2T_w(T2U_w(x) + ^u_wT2U_w(y))$$
由式[\[e\]](#e){reference-type="ref"
reference="e"},$T2U_w(x) \iff x_{w-1}2^w + x$,
$T2U_w(y) \iff y_{w-1}2^w + y$。使用属性【$+^u_w$是
模$2^w$的加法，以及模数加法的属性，我们得到： $$\begin{aligned}
            x + ^t_wy &= U2T_w(T2U_w(x) + ^u_wT2U_w(y) \\
                        &= U2T_w[(x_{w-1}2^w + x + y_{w-1}2^w + y)\ mod\ 2^w]\\
              &= U2T_w[(x + y)\ mod\ 2^w]
        
\end{aligned}$$

定义$z \dot{=} x + y$，$z^{\prime} \dot{=} z\ mod\ 2^w$,
$z^{\prime\prime} \dot{=} U2T_w(z^{\prime})$，
$z^{\prime\prime} = x + ^t_wy$。下面分4种情况讨论：

1\. $-2^w \le z < -2^{w-1}$, 则$z^{\prime} = z + 2^w$。
于是得出$0 \le z^{\prime} < -2^{w-1} + 2^w = 2^{w-1}$。
检查式[\[1\]](#1){reference-type="ref"
reference="1"}可以看到$z^{\prime}$在满足
$z^{\prime\prime} = z^{\prime}$的范围之内。
这种情况称作**负溢出(negative overflow)**。
将两个负数x和y相加(这是得到$z < -2^{w-1}$的唯一方式)，
得到一个非负的结果$z^{\prime\prime} = x + y + 2^w$。

2\. $-2^{w-1} \le z < 0$，则$z^\prime = z + 2^w$。
于是得出$-2^{w-1} + 2^w = 2^{w-1} \le z^\prime < 2^w$。
检查式[\[1\]](#1){reference-type="ref"
reference="1"}可以看到$z^\prime$在满足
$z^{\prime\prime} = z^\prime - 2^w$的范围之内。
因此$z^{\prime\prime} = z^\prime - 2^w = z + 2^w - 2^w = z$。
即，补码和$z^{\prime\prime}$等于整数和$x + y$。

3\. $0 \le z < 2^{w-1}$，则$z^\prime = z$。
于是得出$0 \le z^\prime < 2^{w-1}$，因此
$z^{\prime\prime} = z^\prime = z$。于是
补码和$z^{\prime\prime}$又等于整数和$x + y$。

4\. $2^{w-1} \le z < 2^w$，则$z^\prime = z$。
于是得出$2^{w-1} \le z^\prime < 2^w$。
在这个范围内，$z^{\prime\prime} = z^\prime - 2^w$。
因此得到$z^{\prime\prime} = x + y -2^w$。 这种情况称作**正溢出(positive
overflow)**。 将整数x和y相加(这是得到$z \ge 2^{w-1}$的唯一方式)，
得出一个负数结果$z^{\prime\prime} = x + y - 2^w$。 $\blacksquare$ ◻
:::

补码加法的形象表示见下图：

![补码加法(字长为4位的情况下，当
$x + y < -8$时，产生负溢出；$x + y \ge 8$时，产生正溢出)](two_comp_add.png){#fig:two_comp_add-png
width="80%"}

::: theorem
**原理 14**. *检测补码加法中的溢出*

*对满足$TMin_w \le x, y \le TMax_w$的x和y，令
$s \dot{=} x + ^t_wy$。当且仅当$x > 0, y > 0$，
但$s \le 0$时，计算s发生了正溢出。当且仅当
$x < 0, y < 0$，但$s \ge 0$时，计算s发生了负溢出。*
:::

::: proof
*Proof.* 检测补码加法中的溢出

1\. 分析正溢出：若$x > 0, y > 0$，而$s \le 0$，
那么显然发生了正溢出。反过来，正溢出的条件为 1)
$x > 0, y > 0$(或者$x + y < TMax_w$)， 2) $s \le 0$。

2\. 分析负溢出：若$x < 0, y < 0$，而$s \ge 0$,
那么显然发生了负溢出。反过来，负溢出的条件为 1)
$x < 0, y < 0$(或者$x + y > TMin_w$)， 2) $s \ge 0$。 $\blacksquare$ ◻
:::

## 补码的非

::: theorem
**原理 15**. *补码的非*

*对满足$TMin_w \le x \le TMax_w$的x，其补码的非 $-^t_w$由下式给出：
$$-^t_wx = \left\{
                \begin{array}{ll}
                    TMin_w, & x = TMin_w \\
                    -x, & x > TMin_w
                \end{array}
                \right.$$*
:::

即，对w位的补码加法来说，$TMin_w$是自己的加法的逆，其他
任何数值x都有-x作为其加法的逆。

::: proof
*Proof.* 补码的非

观察发现$TMin_w + TMin_w = -2^{w-1} + (-2^{w-1}) = -2^w$。
这将导致负溢出，因此$TMin_w + ^t_wTMin_w = -2^w + 2^w = 0$。
对满足$x > TMin_w$的x，数值-x可以表示为一个w位的补码，
它们的和$-x + x = 0$。 $\blacksquare$ ◻
:::

### 补码非的两种快速求法

1\. 执行位级补码非可以对每一位求补，再对结果加1. 即，$-x = {\sim}x + 1$.

2\. 将位向量分为两部分：假设k是最右边的1的位置，故
x可表示为$[x_{w-1}, x_{w-2}, \ldots, x_{k+1}, 1, 0, \ldots, 0]$。
这个值的非写成二进制格式就是$[{\sim}x_{w-1}, {\sim}x_{w-2}, \ldots, {\sim}x_{k+1}, 1, 0, \ldots, 0]$。即，对k左边的所有位取反。

## 无符号乘法

::: theorem
**原理 16**. *无符号数乘法*

*对满足$0 \le x, y \le UMax_w$的x和y有：
$$x * ^u_wy = (x \cdot y)\ mod\ 2^w \refstepcounter{equation}\tag{\theequation}\label{2.16}$$*
:::

## 补码乘法

::: theorem
**原理 17**. *补码乘法*

*对满足$TMin_w \le x, y \le TMax_w$的x和y有：
$$x * ^t_wy = U2T_w((x \cdot y)mod\ 2^w) \refstepcounter{equation}\tag{\theequation}\label{2.17}$$*
:::

::: theorem
**原理 18**. *无符号数和补码乘法的位级等价性*

*给定长度为w的位向量$\vec{x}$和$\vec{y}$，用补码形式
的位向量表示来定义整数x和y：$x = B2T_w(\vec{x})$,
$y = B2T_w(\vec{y})$。用无符号数形式的位向量表示来定义
非负整数$x^\prime$和$y^\prime$：
$x^\prime = B2U_w(\vec{x})$，$y^\prime = B2U_w(\vec{y})$。 则
$$T2B_w(x * ^t_wy) = U2B_w(x^\prime * ^u_wy^\prime)$$*
:::

::: proof
*Proof.* 无符号和补码乘法的位级等价性

据式[\[1\]](#1){reference-type="ref"
reference="1"}，我们有$x^\prime = x + x_{w-1}2^w$和
$y^\prime = y + y_{w-1}2^w$。这些值的乘积模$2^w$ 可得：
$$\begin{aligned}
            (x^\prime \cdot y^\prime)mod\ 2^w &= 
            [(x + x_{w-1}2^w) \cdot (y + y_{w-1}2^w)]mod\ 2^w \\
                                              &= [x \cdot y + (x_{w-1}y + y_{w-1}x)2^w + x_{w-1}y_{w-1}2^{2w}]mod\ 2^w \\
                                              &= (x \cdot y)\ mod\ 2^w \refstepcounter{equation}\tag{\theequation}\label{2.18}
        
\end{aligned}$$ 由于模运算符，所有带有权重$2^w$和$2^{2w}$的项都丢掉了。
根据等式[\[2.17\]](#2.17){reference-type="ref"
reference="2.17"}，我们有$x * ^t_wy = U2T_w((x \cdot y)\ mod\ 2^w)$。对等式两边应用操作$T2U_w$有：
$$T2U_w(x * ^t_wy) = T2U_w(U2T_w((x \cdot y)\ mod\ 2^w)) = (x \cdot y)\ mod\ 2^w$$
由该结果与式[\[2.16\]](#2.16){reference-type="ref"
reference="2.16"}和式[\[2.18\]](#2.18){reference-type="ref"
reference="2.18"}结合得到
$T2U_w(x * ^t_wy) = (x^\prime \cdot y^\prime)\ mod\ 2^w = x^\prime * ^t_wy^\prime$。对该式两边应用$U2B_w$，得：
$$U2B_w(T2U_w(x * ^t_wy)) = T2B_w(x * ^t_wy) = U2B_w(x^\prime * ^u_wy^\prime)$$
$\blacksquare$ ◻
:::

## 乘以常数

**编译器使用移位和加法运算组合来代替乘以常数因子的乘法。**

::: theorem
**原理 19**. *乘以2的幂*

*设x为位模式$[x_{w-1}, x_{w-2}, \ldots, x_0]$表示的
无符号整数。那么，对于任何$k \ge 0$，我们都认为
$[x_{w-1}, x_{w-2}, \ldots, x_0, \textcolor{blue}{0, \ldots, 0}]$给出了$x2^k$的$w + k$位的无符号表示，这里右边增加了k个0.*
:::

::: proof
*Proof.* 乘以2的幂

$$\begin{aligned}
            B2U_{w+k}([x_{w-1}, x_{w-2}, \ldots, x_0, \textcolor{blue}{0, \ldots, 0}]) &= \sum^{w-1}_{i=0}x_i2^{i+k} \\
                                                                                        &= \left[\sum^{w-1}_{i=0}x_i2^i \right] \cdot 2^k \\
                                                                                        &= x2^k
        
\end{aligned}$$ $\blacksquare$ ◻
:::

::: theorem
**原理 20**. *与2的幂相乘的无符号乘法*

*C变量x和k有无符号数值x和k，且$0 \le k < w$, 则C表达式
$x \ll k$产生数值$x * ^u_w2^k$。*
:::

::: theorem
**原理 21**. *与2的幂相乘的补码乘法*

*C变量x和k有补码值x和无符号数值k，且$0 \le k < w$，则C
表达式$x \ll k$产生数值$x * ^t_w2^k$。*
:::

## 除以2的幂

::: theorem
**原理 22**. *除以2的幂的无符号除法*

*C变量x和k有无符号数值x和k，且$0 \le k < w$，则
C表达式$x \gg k$产生数值$\left\lfloor x / 2^k \right\rfloor$*
:::

::: proof
*Proof.* 除以2的幂的无符号除法

设x为位模式$[x_{w-1}, x_{w-2}, \ldots, x_0]$表示的无符号
整数，而k的取值范围为$0 \le k < w$。设$x^\prime$为
$w - k$位位表示$[x_{w-1}, x_{w-2}, \ldots, x_k]$的无符号
数，而$x^{\prime\prime}$为k位位表示$[x_{k-1}, \ldots, x_0]$
的无符号数。由此，$x = 2^kx^\prime + x^{\prime\prime}$，
而$0 \le x^{\prime\prime} < 2^k$。因此可得
$\left\lfloor x / 2^k \right\rfloor = x^\prime$。

对位向量$[x_{w-1}, x_{w-2}, \ldots, x_0]$逻辑右移k位 会得到向量
$$[\textcolor{blue}{0, \ldots, 0}, x_{w-1}, x_{w-2}, \ldots, x_k]$$

这个位向量有数值$x^\prime$，该值可以通过计算$x \gg k$得到。
$\blacksquare$ ◻
:::

::: theorem
**原理 23**. *除以2的幂的补码除法，向下舍入*

*C变量x和k分别有补码值x和无符号数值k，且$0 \le k < w$，
则当执行算术移位时，C表达式$x \gg k$产生数值
$\left\lfloor x / 2^k \right\rfloor$。*
:::

::: proof
*Proof.* 除以2的幂的补码除法，向下舍入

设x的位模式$[x_{w-1}, x_{w-2}, \ldots, x_0]$表示的
补码整数，而k的取值范围为$0 \le k < w$。设
$x^\prime$为$w-k$位$[x_{w-1}, x_{w-2}, \ldots, x_k]$
表示的补码数，而$x^{\prime\prime}$为低k位
$[x_{k-1}, \ldots, x_0]$表示的无符号数。通过
与对无符号数情况类似的分析，可得$x = 2^kx^\prime + x^{\prime\prime}$，
而$0 \le x^{\prime\prime} < 2^k$，得到$x^\prime = \left\lfloor x / 2^k \right\rfloor$。算术右移位向量$[\textcolor{blue}{x_{w-1}}, x_{w-2}, \ldots, x_0]$k位，得
$$[\textcolor{blue}{x_{w-1}, \ldots, x_{w-1}, x_{w-1}}, x_{w-2}, \ldots, x_k]$$
它刚好就是将$[x_{w-1}, x_{w-2}, \ldots, x_k]$从$w-k$
位符号扩展到w位。故这个移位后的位向量就是
$\left\lfloor x / 2^k \right\rfloor$的补码表示。 $\blacksquare$ ◻
:::

::: theorem
**原理 24**. *除以2的幂的补码除法，向上舍入*

*C变量x和k分别有补码值x和无符号数值k，且$0 \le k < w$，
则当执行算术移位时，C表达式$(x+(1\ll k)-1)\gg k$产生
数值$\left\lceil x / 2^k \right\rceil$。*
:::

::: proof
*Proof.* 除以2的幂的补码除法，向上舍入

查看$\left\lceil x / y \right\rceil = \left\lfloor (x + y - 1) / y \right\rfloor$，
假设$x = qy + r$，其中$0 \le r < y$，得到$(x + y - 1) / y = q + (r + y - 1) / y$，因此$\left\lfloor (x + y - 1) / y \right\rfloor = q + \left\lfloor (r + y - 1) / y \right\rfloor$。当$r = 0$时，后面一项等于0，而当$r > 0$时，等于1.即，
通过给x增加一个偏量$y - 1$，然后再将除法向下舍入，
当y整除x时，我们得到q，否则得到$q + 1$。

故当$y = 2^k$，C表达式$x + (1\ll k) - 1$得到数值
$x + 2^k - 1$。将这个值算术右移k位即产生
$\left\lfloor x / 2^k \right\rfloor$。 $\blacksquare$ ◻
:::

# 浮点数

浮点表示对形如$V = x \times 2^y$的有理数进行编码。

## 二进制小数

十进制的小数表示法如下：
$$d_md_{m-1}\ldots d_1d_0.d_{-1}d_{-2}\ldots d_{-n}$$
每个十进制数$d_{i}$的取值范围是$0\sim 9$。故 该表达描述的数值d定义为：
$$d = \sum^m_{i=-n}10^i \times d_i$$ 类似的，一个形如
$$b_mb_{m-1}\ldots b_1b_0.b_{-1}b_{-2}\ldots b_{-n-1}b_{-n}$$
的表示法定义的0 1串数字定义如下： $$b = \sum^m_{i=-n}2^i \times b_i$$

::: wrapfigure
r0.85 ![image](fraction_binary.png){width="0.8\\linewidth"}
:::

符号`.`现在是二进制的点，点左边的位权是2的正幂，
点右边的位的权是2的负幂。

例如，$101.11_2$表示数字$1 \times 2^2 = 0 \times 2^1
     + 1 \times 2^0 + 1 \times 2^{-1} + 1 \times 2^{-2}
      = 4 + 0 + 1 + \frac{1}{2} + \frac{1}{4} = 5 \frac{3}{4}$

**$0.111\ldots1_2$这样的数刚好是小于1的数。
例如，$0.111111_2$表示$\frac{63}{64}$，
我们通常用简单的表达法$1.0 - \epsilon$ 来表示这样的数值。**

**小数的二进制表示法只能表示那些能够表示成
$x \times 2^y$的数。其他的值只能够被近似表示。**

例如，$\frac{1}{5} = 0.20_{10}$可以被十进制小数
0.20精确表示，却不能被精确地表示为一个二进制小数。
增加二进制表示的长度可以提高其表示精度：

  表示                     值         十进制
  ---------------- ------------------ -------------------
  $0.0_2$            $\frac{0}{2}$    $0.0_{10}$
  $0.01_2$           $\frac{1}{4}$    $0.25_{10}$
  $0.010_2$          $\frac{2}{8}$    $0.25_{10}$
  $0.0011_2$         $\frac{8}{16}$   $0.1875_{10}$
  $0.001101_2$      $\frac{13}{64}$   $0.203125_{10}$
  $0.00110011_2$    $\frac{51}{256}$  $0.19921875_{10}$

## IEEE浮点表示

IEEE浮点标准用$V = (-1)^s \times M \times 2^E$的形式来表示一个数：

$\bullet$

:   符号(sign) s决定数字的正(s=0)负(s=1),
    对于数值0的符号位作为特殊情况处理。

$\bullet$

:   尾数(significand) M是一个二进制小数，
    它的范围是$1 \sim 2 - \epsilon$，或者是$0 \sim 1 - \epsilon$。

$\bullet$

:   阶码(exponent) E的作用是对浮点数加权，
    这个权重是2的E次幂(可能为负数)。

对浮点数的位表示划分为三个字段，分别对这些值进行编码：

$\bullet$

:   一个单独的符号位s直接编码符号s。

$\bullet$

:   k位的阶码字段$exp=e_{k-1}\ldots e_1e_0$编码阶码E。

$\bullet$

:   n位小数字段$frac=f_{n-1}\ldots f_1f_0$编码尾数M，但是编码出来的值也依赖于编码字段的值是否等于0。

![标准浮点格式(浮点数由3个字段表示。两种最常见的格式是他们被封装到32位(单精度)和64位(双精度)的字中)](standard_float.png){#fig:Standard_float-png
width="80%"}

给定位表示，根据exp的值，被编码的值可以分成三种不同的情况
(最后一种情况有两个变种)。下图说明的是单精度格式的情况。

![单精度浮点数值的分类(阶码的值决定了这个数是规格化的、非规格化的或特殊值)](float_class.png){#fig:float_class-png
width="80%"}

**情况1:规格化的值**

最普遍的情况。当exp的位模式既不全为0(数值0)，也不全为1(单精度数值255，双精度数值2047)时的情况。

阶码字段被解释为以偏置(biased)形式表示的有符号整数。
阶码的值是$E = e - Bias$，其中e是无符号数，其位表示为
$e_{k-1}\ldots e_1e_0$，而$Bias$是一个等于$2^{k-1}-1$
(单精度是127，双精度是1023)的偏置值。

小数字段frac被解释为描述小数值f，其中$0 \le f < 1$，其
二进制表示为$0.f_{n-1}\ldots f_1f_0$, 即二进制小数点
在最高有效位的左边。尾数定义为$M = 1 + f$。有时，该方式
也被称为*隐含*的以1开头的(implied leading 1)表示，
因为M可看成是二进制表示为$1.f_{n-1}f_{n-2}\ldots f_0$的 数字。

**我们总是能够调整阶码E，使得尾数M在范围$1 \le M < 2$之中，
(假设没有溢出)，那么这种表示方法是一种轻松获得一个额外 精度位的技巧。**

既然第一位总是1，那么我们就不需要显式地表示它。

**情况2:非规格化的值**

当阶码域为全0时，所表示的数是*非规格化*形式。
阶码值是$E = 1 - Bias$，而尾数的值是$M = f$,
即小数字段的值，不包含隐含的开头的1。

非规格化数有两个用途。

1\. 提供了一种表示0的方法。$+0.0$的浮点表示位模式为全0.
然而当符号为为1，其他域全为0时，我们得到值$-0.0$。IEEE标准
认为它们在某些方面不同，其他方面相同。

2\. 表示非常接近于0.0的数。它们提供了被称为*逐渐溢出* (gradual
underflow)的属性，其中，可能的数值分布均匀地接近于0.0。

**情况3:特殊值**

当阶码全为1时的情况。

a\. 小数域全为0，得到的值表示无穷∞：$s = 0 \rightarrow +\infty$,
$s = 1 \rightarrow -\infty$。当我们把两个非常大的数相乘，
或者除以零，无穷能够表示*溢出*的结果。

b\. 小数域为非零时，结果值被称为\"NaN\"，即"不是一个数"
一些运算结果不能是实数或无穷，这种返回值很有用处。

## 数学示例

在这个示例中，假定用6位格式来表示，有$k=3$的阶码位和 $n=2$的尾数位。
偏置量是$2^{3-1} - 1 = 3$。图中a部分显示了所有可表示的值
(除了NaN)。两个无穷值在两个末端。最大数量值得规格化数是
$\pm 14$。非规格化数聚集在0的附近。
图中b部分，我们只展示了介于$-1.0$和$+1.0$之间的数值。两个
零是特殊的非规格化数。

**那些可表示的数并非均匀分布，越靠近原点处越稠密。**

![6位浮点格式可表示的值(k=3的阶码位和k=2的尾数位。偏置量是3)](float_number.png){#fig:float_number-png
width="80%"}

下图展示了假定的8位浮点格式的示例，有k=4的阶码位和n=3的
小数位。偏置量是$2^{4-1} = 7$。图分为3个区域，来描述3类 数字。

![8位浮点格式的非负值示例(k=4的阶码位的和n=3的小数位。偏置量是7)](8_bit.png){#fig:8_bit-png
width="80%"}

下图展示了一些重要单精度和双精度浮点数的表示和数字值。

![非负浮点数的示例](non_neg_float.png){#fig:non_neg_float-png
width="80%"}

$\bullet$

:   值$+0.0$总有一个全为0的位表示。

$\bullet$

:   最小的正非规格化值的位表示，是由最低
    有效位为1而其他所有位为0构成的。它具有
    小数(和尾数)值$M = f = 2^{-n}$和阶码值
    $E = -2^{k-1} + 2$。因此它的数字值是 $V = 2^{-n-2^{k-1}+2}$。

$\bullet$

:   最大的正规格化值的位模式是由全为0
    的阶码字段和全为1的小数字段组成的。它有
    小数(和尾数)值$M = f = 1 - 2^{-n}$和阶码值
    $E = -2^{k-1} + 2$。因此数值
    $V = (1 - 2^{-n}) \times 2^{-2^{k-1}+2}$，
    这仅比最小的规格化值小一点。

$\bullet$

:   最小的正规格化值的位模式的阶码字段的
    最低有效位为1，其他位全为0。它的尾数值
    $M = 1$，而阶码值$E = -2^{k-1} + 2$。 因此数值$V = 2^{-2^{k-1}+2}$。

$\bullet$

:   值1.0的位表示的阶码字段除了最高 有效位等于1以外，其他位都等于0.它的
    尾数值是$M=1$，而它的阶码值是$E=0$。

$\bullet$

:   最大的规格化值的位表示的符号 位为0，阶码的最低有效位等于0，其他位
    等于1.它的小数值$f = 1 - 2^{-n}$，尾数
    $M = 2 - 2^{-n}$。它的阶码值$E=2^{k-1}-1$，
    得到数值$V = (2 - 2^{-n}) \times 2^{2^{k-1}-1} = (1 - 2^{-n-1} \times 2^{2^{k-1}}$。

## 舍入
