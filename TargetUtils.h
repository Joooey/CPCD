//
// Created by xue on 19-1-3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_TARGETUTILS_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_TARGETUTILS_H

#include <cstdlib>
#include "Enums.h"
#include "GlobalVar.h"




/*
 * 生成虚拟机代码
 *
 * x: instruction.f;
 * y: instruction.l;
 * z: instruction.a;
 */
void gen(enum fct x, int y, int z )
{
    if (cx >= cxmax)
    {
        printf("Program is too long!\n");	/* 生成的虚拟机代码程序过长 */
        exit(1);
    }
    if ( z >= amax)
    {
        printf("Displacement address is too big!\n");	/* 地址偏移越界 */
        exit(1);
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
}



/*
 * 用数组实现集合的集合运算
 */
int inset(int e, bool* s)
{
    return s[e];
}


/*
 * 测试当前符号是否合法
 *
 * 在语法分析程序的入口和出口处调用测试函数test，
 * 检查当前单词进入和退出该语法单位的合法性
 *
 * s1:	需要的单词集合
 * s2:	如果不是需要的单词，在某一出错状态时，
 *      可恢复语法分析继续正常工作的补充单词符号集合
 * n:  	错误号
 */
void test(bool* s1, bool* s2, int n)
{
    if (!inset(sym, s1))
    {
        error(n);
        /* 当检测不通过时，不停获取符号，直到它属于需要的集合或补救的集合 */
        while ((!inset(sym,s1)) && (!inset(sym,s2)))
        {
            getsym();
        }
    }
}

#endif //COMPILATIONPRINCIPLECOURSEDESGIN_TARGETUTILS_H
