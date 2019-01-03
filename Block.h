//
// Created by xue on 19-1-3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_BLOCK_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_BLOCK_H



#include "Defines.h"
#include "GlobalVar.h"
#include "TargetUtils.h"
#include "Algorithm.h"
#include "GetUtils.h"

void block(int lev, int tx, bool* fsys)
{
    int i;

    int dx;                 /* 记录数据分配的相对地址 */
    int tx0;                /* 保留初始tx */
    int cx0;                /* 保留初始cx */
    bool nxtlev[symnum];    /* 在下级函数的参数中，符号集合均为值参，但由于使用数组实现，
	                           传递进来的是指针，为防止下级函数改变上级函数的集合，开辟新的空间
	                           传递给下级函数*/

    dx = 3;                 /* 三个空间用于存放静态链SL、动态链DL和返回地址RA  */
    tx0 = tx;		        /* 记录本层标识符的初始位置 */
    table[tx].adr = cx;	    /* 记录当前层代码的开始位置 */
    gen(jmp, 0, 0);         /* 产生跳转指令，跳转位置未知填0 */

    if (lev > levmax)		/* 嵌套层数过多 */
    {
        error(32);
    }

    do {

        if (sym == constsym)	/* 遇到常量声明符号，开始处理常量声明 */
        {
            getsym();

            do {
                constdeclaration(&tx, lev, &dx);	/* dx的值会被constdeclaration改变，使用指针 */
                while (sym == comma)  /* 遇到逗号继续定义常量 */
                {
                    getsym();
                    constdeclaration(&tx, lev, &dx);
                }
                if (sym == semicolon) /* 遇到分号结束定义常量 */
                {
                    getsym();
                }
                else
                {
                    error(5);   /* 漏掉了分号 */
                }
            } while (sym == ident);
        }

        if (sym == varsym)		/* 遇到变量声明符号，开始处理变量声明 */
        {
            getsym();

            do {
                vardeclaration(&tx, lev, &dx);
                while (sym == comma)
                {
                    getsym();
                    vardeclaration(&tx, lev, &dx);
                }
                if (sym == semicolon)
                {
                    getsym();
                }
                else
                {
                    error(5); /* 漏掉了分号 */
                }
            } while (sym == ident);
        }

        while (sym == procsym) /* 遇到过程声明符号，开始处理过程声明 */
        {
            getsym();

            if (sym == ident)
            {
                enter(procedure, &tx, lev, &dx);	/* 填写符号表 */
                getsym();
            }
            else
            {
                error(4);	/* procedure后应为标识符 */
            }

            if (sym == semicolon)
            {
                getsym();
            }
            else
            {
                error(5);	/* 漏掉了分号 */
            }

            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
            nxtlev[semicolon] = true;
            block(lev + 1, tx, nxtlev); /* 递归调用 */

            if(sym == semicolon)
            {
                getsym();
                memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
                nxtlev[ident] = true;
                nxtlev[procsym] = true;
                test(nxtlev, fsys, 6);
            }
            else
            {
                error(5);	/* 漏掉了分号 */
            }
        }
        memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
        nxtlev[ident] = true;
        test(nxtlev, declbegsys, 7);
    } while (inset(sym, declbegsys));	/* 直到没有声明符号 */

    code[table[tx0].adr].a = cx;	/* 把前面生成的跳转语句的跳转位置改成当前位置 */
    table[tx0].adr = cx;	        /* 记录当前过程代码地址 */
    table[tx0].size = dx;	        /* 声明部分中每增加一条声明都会给dx增加1，声明部分已经结束，dx就是当前过程数据的size */
    cx0 = cx;
    gen(ini, 0, dx);	            /* 生成指令，此指令执行时在数据栈中为被调用的过程开辟dx个单元的数据区 */

    if (tableswitch)		/* 输出符号表 */
    {
        for (i = 1; i <= tx; i++)
        {
            switch (table[i].kind)
            {
                case constant:
                    printf("    %d const %s ", i, table[i].name);
                    printf("val=%d\n", table[i].val);
                    fprintf(ftable, "    %d const %s ", i, table[i].name);
                    fprintf(ftable, "val=%d\n", table[i].val);
                    break;
                case variable:
                    printf("    %d var   %s ", i, table[i].name);
                    printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
                    fprintf(ftable, "    %d var   %s ", i, table[i].name);
                    fprintf(ftable, "lev=%d addr=%d\n", table[i].level, table[i].adr);
                    break;
                case procedure:
                    printf("    %d proc  %s ", i, table[i].name);
                    printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                    fprintf(ftable,"    %d proc  %s ", i, table[i].name);
                    fprintf(ftable,"lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
                    break;
            }
        }
        printf("\n");
        fprintf(ftable,"\n");
    }

    /* 语句后继符号为分号或end */
    memcpy(nxtlev, fsys, sizeof(bool) * symnum);	/* 每个后继符号集合都包含上层后继符号集合，以便补救 */
    nxtlev[semicolon] = true;
    nxtlev[endsym] = true;
    statement(nxtlev, &tx, lev);
    gen(opr, 0, 0);	                    /* 每个过程出口都要使用的释放数据段指令 */
    memset(nxtlev, 0, sizeof(bool) * symnum);	/* 分程序没有补救集合 */
    test(fsys, nxtlev, 8);            	/* 检测后继符号正确性 */
    listcode(cx0);                      /* 输出本分程序生成的代码 */
}

#endif //COMPILATIONPRINCIPLECOURSEDESGIN_BLOCK_H
