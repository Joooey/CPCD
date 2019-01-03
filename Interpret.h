//
// Created by xue on 19-1-3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_INTERPRET_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_INTERPRET_H

#include <cstdio>
#include "defines.h"
#include "GlobalVar.h"




/* 通过过程基址求上l层过程的基址 */
int base(int l, int* s, int b)
{
    int b1;
    b1 = b;
    while (l > 0)
    {
        b1 = s[b1];
        l--;
    }
    return b1;
}



/*
 * 解释程序
 */
void interpret()
{
    int p = 0; /* 指令指针 */
    int b = 1; /* 指令基址 */
    int t = 0; /* 栈顶指针 */
    struct instruction i;	/* 存放当前指令 */
    int s[stacksize];	/* 栈 */

    printf("Start pl0\n");
    fprintf(fresult,"Start pl0\n");
    s[0] = 0; /* s[0]不用 */
    s[1] = 0; /* 主程序的三个联系单元均置为0 */
    s[2] = 0;
    s[3] = 0;
    do {
        i = code[p];	/* 读当前指令 */
        p = p + 1;
        switch (i.f)
        {
            case lit:	/* 将常量a的值取到栈顶 */
                t = t + 1;
                s[t] = i.a;
                break;
            case opr:	/* 数学、逻辑运算 */
                switch (i.a)
                {
                    case 0:  /* 函数调用结束后返回 */
                        t = b - 1;
                        p = s[t + 3];
                        b = s[t + 2];
                        break;
                    case 1: /* 栈顶元素取反 */
                        s[t] = - s[t];
                        break;
                    case 2: /* 次栈顶项加上栈顶项，退两个栈元素，相加值进栈 */
                        t = t - 1;
                        s[t] = s[t] + s[t + 1];
                        break;
                    case 3:/* 次栈顶项减去栈顶项 */
                        t = t - 1;
                        s[t] = s[t] - s[t + 1];
                        break;
                    case 4:/* 次栈顶项乘以栈顶项 */
                        t = t - 1;
                        s[t] = s[t] * s[t + 1];
                        break;
                    case 5:/* 次栈顶项除以栈顶项 */
                        t = t - 1;
                        s[t] = s[t] / s[t + 1];
                        break;
                    case 6:/* 栈顶元素的奇偶判断 */
                        s[t] = s[t] % 2;
                        break;
                    case 8:/* 次栈顶项与栈顶项是否相等 */
                        t = t - 1;
                        s[t] = (s[t] == s[t + 1]);
                        break;
                    case 9:/* 次栈顶项与栈顶项是否不等 */
                        t = t - 1;
                        s[t] = (s[t] != s[t + 1]);
                        break;
                    case 10:/* 次栈顶项是否小于栈顶项 */
                        t = t - 1;
                        s[t] = (s[t] < s[t + 1]);
                        break;
                    case 11:/* 次栈顶项是否大于等于栈顶项 */
                        t = t - 1;
                        s[t] = (s[t] >= s[t + 1]);
                        break;
                    case 12:/* 次栈顶项是否大于栈顶项 */
                        t = t - 1;
                        s[t] = (s[t] > s[t + 1]);
                        break;
                    case 13: /* 次栈顶项是否小于等于栈顶项 */
                        t = t - 1;
                        s[t] = (s[t] <= s[t + 1]);
                        break;
                    case 14:/* 栈顶值输出 */
                        printf("%d", s[t]);
                        fprintf(fresult, "%d", s[t]);
                        t = t - 1;
                        break;
                    case 15:/* 输出换行符 */
                        printf("\n");
                        fprintf(fresult,"\n");
                        break;
                    case 16:/* 读入一个输入置于栈顶 */
                        t = t + 1;
                        printf("?");
                        fprintf(fresult, "?");
                        scanf("%d", &(s[t]));
                        fprintf(fresult, "%d\n", s[t]);
                        break;
                }
                break;
            case lod:	/* 取相对当前过程的数据基地址为a的内存的值到栈顶 */
                t = t + 1;
                s[t] = s[base(i.l,s,b) + i.a];
                break;
            case sto:	/* 栈顶的值存到相对当前过程的数据基地址为a的内存 */
                s[base(i.l, s, b) + i.a] = s[t];
                t = t - 1;
                break;
            case cal:	/* 调用子过程 */
                s[t + 1] = base(i.l, s, b);	/* 将父过程基地址入栈，即建立静态链 */
                s[t + 2] = b;	/* 将本过程基地址入栈，即建立动态链 */
                s[t + 3] = p;	/* 将当前指令指针入栈，即保存返回地址 */
                b = t + 1;	/* 改变基地址指针值为新过程的基地址 */
                p = i.a;	/* 跳转 */
                break;
            case ini:	/* 在数据栈中为被调用的过程开辟a个单元的数据区 */
                t = t + i.a;
                break;
            case jmp:	/* 直接跳转 */
                p = i.a;
                break;
            case jpc:	/* 条件跳转 */
                if (s[t] == 0)
                    p = i.a;
                t = t - 1;
                break;
        }
    } while (p != 0);
    printf("End pl0\n");
    fprintf(fresult,"End pl0\n");
}



#endif //COMPILATIONPRINCIPLECOURSEDESGIN_INTERPRET_H
