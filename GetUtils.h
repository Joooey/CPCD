//
// Created by xue on 19-1-3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_GETUTILS_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_GETUTILS_H

#include <cstdlib>
#include <cstring>
#include "GlobalVar.h"
#include "SysUtils.h"


/*
 * 过滤空格，读取一个字符
 * 每次读一行，存入line缓冲区，line被getsym取空后再读一行
 * 被函数getsym调用
 */
void getch()
{
    if (cc == ll) /* 判断缓冲区中是否有字符，若无字符，则读入下一行字符到缓冲区中 */
    {
        if (feof(fin))
        {
            printf("Program is incomplete!\n");
            exit(1);
        }
        ll = 0;
        cc = 0;
        printf("%d ", cx);
        fprintf(foutput,"%d ", cx);
        ch = ' ';
        while (ch != 10)
        {
            if (EOF == fscanf(fin,"%c", &ch))
            {
                line[ll] = 0;
                break;
            }

            printf("%c", ch);
            fprintf(foutput, "%c", ch);
            line[ll] = ch;
            ll++;
        }
    }
    ch = line[cc];
    cc++;
}


void getsym() {
    int i, j, k;

    while (ch == ' ' || ch == 10 || ch == 9)    /* 过滤空格、换行和制表符 */
    {
        getch();
    }
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) /* 当前的单词是标识符或是保留字 */
    {
        k = 0;
        do {
            if (k < al) {
                a[k] = ch;
                k++;
            }
            getch();
        } while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
        a[k] = 0;
        strcpy(id, a);
        i = 0;
        j = norw - 1;
        do {    /* 搜索当前单词是否为保留字，使用二分法查找 */
            k = (i + j) / 2;
            if (strcmp(id, word[k]) <= 0) {
                j = k - 1;
            }
            if (strcmp(id, word[k]) >= 0) {
                i = k + 1;
            }
        } while (i <= j);
        if (i - 1 > j) /* 当前的单词是保留字 */
        {
            sym = wsym[k];
        } else /* 当前的单词是标识符 */
        {
            sym = ident;
        }
    } else {
        if (ch >= '0' && ch <= '9') /* 当前的单词是数字 */
        {
            k = 0;
            num = 0;
            sym = number;
            do {
                num = 10 * num + ch - '0';
                k++;
                getch();;
            } while (ch >= '0' && ch <= '9'); /* 获取数字的值 */
            k--;
            if (k > nmax) /* 数字位数太多 */
            {
                error(30);
            }
        } else {
            if (ch == ':')        /* 检测赋值符号 */
            {
                getch();
                if (ch == '=') {
                    sym = becomes;
                    getch();
                } else {
                    sym = nul;    /* 不能识别的符号 */
                }
            } else {
                if (ch == '<')        /* 检测小于或小于等于符号 */
                {
                    getch();
                    if (ch == '=') {
                        sym = leq;
                        getch();
                    } else {
                        sym = lss;
                    }
                } else {
                    if (ch == '>')        /* 检测大于或大于等于符号 */
                    {
                        getch();
                        if (ch == '=') {
                            sym = geq;
                            getch();
                        } else {
                            sym = gtr;
                        }
                    } else {
                        sym = ssym[ch];        /* 当符号不满足上述条件时，全部按照单字符符号处理 */
                        if (sym != period) {
                            getch();
                        }

                    }
                }
            }
        }
    }

}
#endif //COMPILATIONPRINCIPLECOURSEDESGIN_GETUTILS_H
