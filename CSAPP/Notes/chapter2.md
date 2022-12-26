---
author:
- yinxuhao \[xuhao_yin@163.com\]
date: 2022-12-26
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

1\. $-2^w \le z < -2^{w-1}$

2\. $-2^{w-1} \le z < 0$

3\. $0 \le z < 2^{w-1}$

4\. $2^{w-1} \le z < 2^w$ ◻
:::
