//
// Created by xue on 19-1-3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_ALGORITHM_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_ALGORITHM_H


#include "Defines.h"
#include "GlobalVar.h"
#include "TargetUtils.h"
#include "SysUtils.h"

void expression(bool* fsys, int* ptx, int lev);

int position(char* id, int tx);

/*
 * 因子处理
 */
void factor(bool* fsys, int* ptx, int lev)
{
    int i;
    bool nxtlev[symnum];
    test(facbegsys, fsys, 24);	/* 检测因子的开始符号 */
    while(inset(sym, facbegsys)) 	/* 循环处理因子 */
    {
        if(sym == ident)	/* 因子为常量或变量 */
        {
            i = position(id, *ptx);	/* 查找标识符在符号表中的位置 */
            if (i == 0)
            {
                error(11);	/* 标识符未声明 */
            }
            else
            {
                switch (table[i].kind)
                {
                    case constant:	/* 标识符为常量 */
                        gen(lit, 0, table[i].val);	/* 直接把常量的值入栈 */
                        break;
                    case variable:	/* 标识符为变量 */
                        gen(lod, lev-table[i].level, table[i].adr);	/* 找到变量地址并将其值入栈 */
                        break;
                    case procedure:	/* 标识符为过程 */
                        error(21);	/* 不能为过程 */
                        break;
                }
            }
            getsym();
        }
        else
        {
            if(sym == number)	/* 因子为数 */
            {
                if (num > amax)
                {
                    error(31); /* 数越界 */
                    num = 0;
                }
                gen(lit, 0, num);
                getsym();
            }
            else
            {
                if (sym == lparen)	/* 因子为表达式 */
                {
                    getsym();
                    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                    nxtlev[rparen] = true;
                    expression(nxtlev, ptx, lev);
                    if (sym == rparen)
                    {
                        getsym();
                    }
                    else
                    {
                        error(22);	/* 缺少右括号 */
                    }
                }
            }
        }
        memset(nxtlev, 0, sizeof(bool) * symnum);
        nxtlev[lparen] = true;
        test(fsys, nxtlev, 23); /* 一个因子处理完毕，遇到的单词应在fsys集合中 */
        /* 如果不是，报错并找到下一个因子的开始，使语法分析可以继续运行下去 */
    }
}

/*
 * 项处理
 */
void term(bool* fsys, int* ptx, int lev)
{
    enum symbol mulop;	/* 用于保存乘除法符号 */
    bool nxtlev[symnum];

    memcpy(nxtlev, fsys, sizeof(bool) * symnum);
    nxtlev[times] = true;
    nxtlev[slash] = true;
    factor(nxtlev, ptx, lev);	/* 处理因子 */
    while(sym == times || sym == slash)
    {
        mulop = sym;
        getsym();
        factor(nxtlev, ptx, lev);
        if(mulop == times)
        {
            gen(opr, 0, 4);	/* 生成乘法指令 */
        }
        else
        {
            gen(opr, 0, 5);	/* 生成除法指令 */
        }
    }
}


/*
 * 表达式处理
 */
void expression(bool* fsys, int* ptx, int lev)
{
    enum symbol addop;	/* 用于保存正负号 */
    bool nxtlev[symnum];

    if(sym == plus || sym == minus)	/* 表达式开头有正负号，此时当前表达式被看作一个正的或负的项 */
    {
        addop = sym;	/* 保存开头的正负号 */
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* 处理项 */
        if (addop == minus)
        {
            gen(opr,0,1);	/* 如果开头为负号生成取负指令 */
        }
    }
    else	/* 此时表达式被看作项的加减 */
    {
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* 处理项 */
    }
    while (sym == plus || sym == minus)
    {
        addop = sym;
        getsym();
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[plus] = true;
        nxtlev[minus] = true;
        term(nxtlev, ptx, lev);	/* 处理项 */
        if (addop == plus)
        {
            gen(opr, 0, 2);	/* 生成加法指令 */
        }
        else
        {
            gen(opr, 0, 3);	/* 生成减法指令 */
        }
    }
}


/*
 * 查找标识符在符号表中的位置，从tx开始倒序查找标识符
 * 找到则返回在符号表中的位置，否则返回0
 *
 * id:    要查找的名字
 * tx:    当前符号表尾指针
 */
int position(char* id, int tx)
{
    int i;
    strcpy(table[0].name, id);
    i = tx;
    while (strcmp(table[i].name, id) != 0)
    {
        i--;
    }
    return i;
}



/*
 * 条件处理
 */
void condition(bool* fsys, int* ptx, int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];

    if(sym == oddsym)	/* 准备按照odd运算处理 */
    {
        getsym();
        expression(fsys, ptx, lev);
        gen(opr, 0, 6);	/* 生成odd指令 */
    }
    else
    {
        /* 逻辑表达式处理 */
        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
        nxtlev[eql] = true;
        nxtlev[neq] = true;
        nxtlev[lss] = true;
        nxtlev[leq] = true;
        nxtlev[gtr] = true;
        nxtlev[geq] = true;
        expression(nxtlev, ptx, lev);
        if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
        {
            error(20); /* 应该为关系运算符 */
        }
        else
        {
            relop = sym;
            getsym();
            expression(fsys, ptx, lev);
            switch (relop)
            {
                case eql:
                    gen(opr, 0, 8);
                    break;
                case neq:
                    gen(opr, 0, 9);
                    break;
                case lss:
                    gen(opr, 0, 10);
                    break;
                case geq:
                    gen(opr, 0, 11);
                    break;
                case gtr:
                    gen(opr, 0, 12);
                    break;
                case leq:
                    gen(opr, 0, 13);
                    break;
            }
        }
    }
}



/*
 * 语句处理
 */
void statement(bool* fsys, int* ptx, int lev)
{
    int i, cx1, cx2;
    bool nxtlev[symnum];

    if (sym == ident)	/* 准备按照赋值语句处理 */
    {
        i = position(id, *ptx);/* 查找标识符在符号表中的位置 */
        if (i == 0)
        {
            error(11);	/* 标识符未声明 */
        }
        else
        {
            if(table[i].kind != variable)
            {
                error(12);	/* 赋值语句中，赋值号左部标识符应该是变量 */
                i = 0;
            }
            else
            {
                getsym();
                if(sym == becomes)
                {
                    getsym();
                }
                else
                {
                    error(13);	/* 没有检测到赋值符号 */
                }
                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                expression(nxtlev, ptx, lev);	/* 处理赋值符号右侧表达式 */
                if(i != 0)
                {
                    /* expression将执行一系列指令，但最终结果将会保存在栈顶，执行sto命令完成赋值 */
                    gen(sto, lev-table[i].level, table[i].adr);
                }
            }
        }
    }
    else
    {
        if (sym == readsym)	/* 准备按照read语句处理 */
        {
            getsym();
            if (sym != lparen)
            {
                error(34);	/* 格式错误，应是左括号 */
            }
            else
            {
                do {
                    getsym();
                    if (sym == ident)
                    {
                        i = position(id, *ptx);	/* 查找要读的变量 */
                    }
                    else
                    {
                        i = 0;
                    }

                    if (i == 0)
                    {
                        error(35);	/* read语句括号中的标识符应该是声明过的变量 */
                    }
                    else
                    {
                        gen(opr, 0, 16);	/* 生成输入指令，读取值到栈顶 */
                        gen(sto, lev-table[i].level, table[i].adr);	/* 将栈顶内容送入变量单元中 */
                    }
                    getsym();

                } while (sym == comma);	/* 一条read语句可读多个变量 */
            }
            if(sym != rparen)
            {
                error(33);	/* 格式错误，应是右括号 */
                while (!inset(sym, fsys))	/* 出错补救，直到遇到上层函数的后继符号 */
                {
                    getsym();
                }
            }
            else
            {
                getsym();
            }
        }
        else
        {
            if (sym == writesym)	/* 准备按照write语句处理 */
            {
                getsym();
                if (sym == lparen)
                {
                    do {
                        getsym();
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[rparen] = true;
                        nxtlev[comma] = true;
                        expression(nxtlev, ptx, lev);	/* 调用表达式处理 */
                        gen(opr, 0, 14);	/* 生成输出指令，输出栈顶的值 */
                        gen(opr, 0, 15);	/* 生成换行指令 */
                    } while (sym == comma);  /* 一条write可输出多个变量的值 */
                    if (sym != rparen)
                    {
                        error(33);	/* 格式错误，应是右括号 */
                    }
                    else
                    {
                        getsym();
                    }
                }

            }
            else
            {
                if (sym == callsym)	/* 准备按照call语句处理 */
                {
                    getsym();
                    if (sym != ident)
                    {
                        error(14);	/* call后应为标识符 */
                    }
                    else
                    {
                        i = position(id, *ptx);
                        if (i == 0)
                        {
                            error(11);	/* 过程名未找到 */
                        }
                        else
                        {
                            if (table[i].kind == procedure)
                            {
                                gen(cal, lev-table[i].level, table[i].adr);	/* 生成call指令 */
                            }
                            else
                            {
                                error(15);	/* call后标识符类型应为过程 */
                            }
                        }
                        getsym();
                    }
                }
                else
                {
                    if (sym == ifsym)	/* 准备按照if语句处理 */
                    {
                        getsym();
                        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                        nxtlev[thensym] = true;
                        nxtlev[dosym] = true;	/* 后继符号为then或do */
                        condition(nxtlev, ptx, lev); /* 调用条件处理 */
                        if (sym == thensym)
                        {
                            getsym();
                        }
                        else
                        {
                            error(16);	/* 缺少then */
                        }
                        cx1 = cx;	/* 保存当前指令地址 */
                        gen(jpc, 0, 0);	/* 生成条件跳转指令，跳转地址未知，暂时写0 */
                        statement(fsys, ptx, lev);	/* 处理then后的语句 */
                        code[cx1].a = cx;	/* 经statement处理后，cx为then后语句执行完的位置，它正是前面未定的跳转地址，此时进行回填 */
                    }
                    else
                    {
                        if (sym == beginsym)	/* 准备按照复合语句处理 */
                        {
                            getsym();
                            memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                            nxtlev[semicolon] = true;
                            nxtlev[endsym] = true;	/* 后继符号为分号或end */

                            statement(nxtlev, ptx, lev);  /* 对begin与end之间的语句进行分析处理 */
                            /* 如果分析完一句后遇到语句开始符或分号，则循环分析下一句语句 */
                            while (inset(sym, statbegsys) || sym == semicolon)
                            {
                                if (sym == semicolon)
                                {
                                    getsym();
                                }
                                else
                                {
                                    error(10);	/* 缺少分号 */
                                }
                                statement(nxtlev, ptx, lev);
                            }
                            if(sym == endsym)
                            {
                                getsym();
                            }
                            else
                            {
                                error(17);	/* 缺少end */
                            }
                        }
                        else
                        {
                            if (sym == whilesym)	/* 准备按照while语句处理 */
                            {
                                cx1 = cx;	/* 保存判断条件操作的位置 */
                                getsym();
                                memcpy(nxtlev, fsys, sizeof(bool) * symnum);
                                nxtlev[dosym] = true;	/* 后继符号为do */
                                condition(nxtlev, ptx, lev);	/* 调用条件处理 */
                                cx2 = cx;	/* 保存循环体的结束的下一个位置 */
                                gen(jpc, 0, 0);	/* 生成条件跳转，但跳出循环的地址未知，标记为0等待回填 */
                                if (sym == dosym)
                                {
                                    getsym();
                                }
                                else
                                {
                                    error(18);	/* 缺少do */
                                }
                                statement(fsys, ptx, lev);	/* 循环体 */
                                gen(jmp, 0, cx1);	/* 生成条件跳转指令，跳转到前面判断条件操作的位置 */
                                code[cx2].a = cx;	/* 回填跳出循环的地址 */
                            }
                        }
                    }
                }
            }
        }
    }
    memset(nxtlev, 0, sizeof(bool) * symnum);	/* 语句结束无补救集合 */
    test(fsys, nxtlev, 19);	/* 检测语句结束的正确性 */
}


/*
 * 常量声明处理
 */
void constdeclaration(int* ptx, int lev, int* pdx)
{
    if (sym == ident)
    {
        getsym();
        if (sym == eql || sym == becomes)
        {
            if (sym == becomes)
            {
                error(1);	/* 把=写成了:= */
            }
            getsym();
            if (sym == number)
            {
                enter(constant, ptx, lev, pdx);
                getsym();
            }
            else
            {
                error(2);	/* 常量声明中的=后应是数字 */
            }
        }
        else
        {
            error(3);	/* 常量声明中的标识符后应是= */
        }
    }
    else
    {
        error(4);	/* const后应是标识符 */
    }
}

/*
 * 变量声明处理
 */
void vardeclaration(int* ptx,int lev,int* pdx)
{
    if (sym == ident)
    {
        enter(variable, ptx, lev, pdx);	// 填写符号表
        getsym();
    }
    else
    {
        error(4);	/* var后面应是标识符 */
    }
}



/*
 * 输出目标代码清单
 */
void listcode(int cx0)
{
    int i;
    if (listswitch)
    {
        printf("\n");
        for (i = cx0; i < cx; i++)
        {
            printf("%d %s %d %d\n", i, mnemonic[code[i].f], code[i].l, code[i].a);
        }
    }
}




int addset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        sr[i] = s1[i]||s2[i];
    }
    return 0;
}

#endif //COMPILATIONPRINCIPLECOURSEDESGIN_ALGORITHM_H
