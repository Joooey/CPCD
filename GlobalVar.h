//
// Created by xue on 19-1-3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_GLOBALVAR_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_GLOBALVAR_H


#include <cstdio>
#include "Enums.h"
#include "defines.h"
#include "Structs.h"


bool listswitch;   /* 显示虚拟机代码与否 */
bool tableswitch;  /* 显示符号表与否 */
char ch;            /* 存放当前读取的字符，getch 使用 */
enum symbol sym;    /* 当前的符号 */
char id[al + 1];      /* 当前ident，多出的一个字节用于存放0 */
int num;            /* 当前number */
int cc, ll;         /* getch使用的计数器，cc表示当前字符(ch)的位置 */
int cx;             /* 虚拟机代码指针, 取值范围[0, cxmax-1]*/
char line[81];      /* 读取行缓冲区 */
char a[al + 1];       /* 临时符号，多出的一个字节用于存放0 */
struct instruction code[cxmax]; /* 存放虚拟机代码的数组 */
char word[norw][al];        /* 保留字 */
enum symbol wsym[norw];     /* 保留字对应的符号值 */
enum symbol ssym[256];      /* 单字符的符号值 */
char mnemonic[fctnum][5];   /* 虚拟机代码指令名称 */
bool declbegsys[symnum];    /* 表示声明开始的符号集合 */
bool statbegsys[symnum];    /* 表示语句开始的符号集合 */
bool facbegsys[symnum];     /* 表示因子开始的符号集合 */


FILE *fin;      /* 输入源文件 */
FILE *ftable;    /* 输出符号表 */
FILE *fcode;    /* 输出虚拟机代码 */
FILE *foutput;  /* 输出文件及出错示意（如有错）、各行对应的生成代码首地址（如无错） */
FILE *fresult;  /* 输出执行结果 */
char fname[al] = {'1', '.', 't', 'x', 't'};
int err;        /* 错误计数器 */


struct tablestruct table[txmax]; /* 符号表 */




#endif //COMPILATIONPRINCIPLECOURSEDESGIN_GLOBALVAR_H


