//
// Created by dmrfcoder on 2019/1/3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_STRUCTS_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_STRUCTS_H



#include "Enums.h"
#include "defines.h"


/* 虚拟机代码结构 */
struct instruction {
    enum fct f; /* 虚拟机代码指令 */
    int l;      /* 引用层与声明层的层次差 */
    int a;      /* 根据f的不同而不同 */
};


/* 符号表结构 */
struct tablestruct
{
    char name[al];	    /* 名字 */
    enum object kind;	/* 类型：const，var或procedure */
    int val;            /* 数值，仅const使用 */
    int level;          /* 所处层，仅const不使用 */
    int adr;            /* 地址，仅const不使用 */
    int size;           /* 需要分配的数据区空间, 仅procedure使用 */
};

#endif //COMPILATIONPRINCIPLECOURSEDESGIN_STRUCTS_H