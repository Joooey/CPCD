//
// Created by xue on 19-1-3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_SYSUTILS_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_SYSUTILS_H

#include <cstring>
#include <cstdlib>
#include "GlobalVar.h"

/*
 *	出错处理，打印出错位置和错误编码
 */
void error(int n)
{
    char space[81];
    memset(space,32,81);

    space[cc-1]=0; /* 出错时当前符号已经读完，所以cc-1 */

    printf("**%s^%d\n", space, n);
    fprintf(foutput,"**%s^%d\n", space, n);

    err = err + 1;
    if (err > maxerr)
    {
        exit(1);
    }
}

void listall() {
    int i;
    if (listswitch)
    {
        for (i = 0; i < cx; i++)
        {
            printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
            fprintf(fcode,"%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
        }
    }

}


/*
 * 在符号表中加入一项
 *
 * k:      标识符的种类为const，var或procedure
 * ptx:    符号表尾指针的指针，为了可以改变符号表尾指针的值
 * lev:    标识符所在的层次
 * pdx:    dx为当前应分配的变量的相对地址，分配后要增加1
 *
 */
void enter(enum object k, int* ptx,	int lev, int* pdx)
{
    (*ptx)++;
    strcpy(table[(*ptx)].name, id); /* 符号表的name域记录标识符的名字 */
    table[(*ptx)].kind = k;
    switch (k)
    {
        case constant:	/* 常量 */
            if (num > amax)
            {
                error(31);	/* 常数越界 */
                num = 0;
            }
            table[(*ptx)].val = num; /* 登记常数的值 */
            break;
        case variable:	/* 变量 */
            table[(*ptx)].level = lev;
            table[(*ptx)].adr = (*pdx);
            (*pdx)++;
            break;
        case procedure:	/* 过程 */
            table[(*ptx)].level = lev;
            break;
    }
}

#endif //COMPILATIONPRINCIPLECOURSEDESGIN_SYSUTILS_H
