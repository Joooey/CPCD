#include <iostream>
#include "defines.h"
#include "Enums.h"
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


struct tablestruct table[txmax]; /* 符号表 */

void init();

void getsym();

void listall();

FILE *fin;      /* 输入源文件 */
FILE *ftable;    /* 输出符号表 */
FILE *fcode;    /* 输出虚拟机代码 */
FILE *foutput;  /* 输出文件及出错示意（如有错）、各行对应的生成代码首地址（如无错） */
FILE *fresult;  /* 输出执行结果 */
char fname[al] = {'1', '.', 't', 'x', 't'};
int err;        /* 错误计数器 */


int main() {
    bool nxtlev[symnum];

    //printf("Input pl/0 sourcefile name：   ");
    //scanf("%s", fname);		/* 输入文件名 */


    if ((fin = fopen("../in.txt", "r")) == NULL) {
        printf("Can't open the input file!\n");
        exit(1);
    }

    ch = fgetc(fin);
    if (ch == EOF) {
        printf("The input file is empty!\n");
        fclose(fin);
        exit(1);
    }
    rewind(fin);

    if ((foutput = fopen("output.txt", "w")) == NULL) {
        printf("Can't open the output file!\n");
        exit(1);
    }

    if ((ftable = fopen("table.txt", "w")) == NULL) {
        printf("Can't open ftable.txt file!\n");
        exit(1);
    }

    printf("List object codes?(Y/N)");    /* 是否输出虚拟机代码 */
    scanf("%s", fname);
    listswitch = (fname[0] == 'y' || fname[0] == 'Y');

    printf("List symbol table?(Y/N)");    /* 是否输出符号表 */
    scanf("%s", fname);
    tableswitch = (fname[0] == 'y' || fname[0] == 'Y');

    init();        /* 初始化 */
    err = 0;
    cc = ll = cx = 0;
    ch = ' ';

    getsym();

    addset(nxtlev, declbegsys, statbegsys, symnum);
    nxtlev[period] = true;
    block(0, 0, nxtlev);    /* 处理分程序 */

    if (sym != period) {
        error(9);
    }

    if (err == 0) {
        printf("\n===Parsing success!===\n");
        fprintf(foutput, "\n===Parsing success!===\n");

        if ((fcode = fopen("code.txt", "w")) == NULL) {
            printf("Can't open fcode.txt file!\n");
            exit(1);
        }

        if ((fresult = fopen("result.txt", "w")) == NULL) {
            printf("Can't open fresult.txt file!\n");
            exit(1);
        }

        listall();     /* 输出所有代码 */
        fclose(fcode);

        interpret();    /* 调用解释执行程序 */
        fclose(fresult);
    } else {
        printf("\n%d errors in pl/0 program!\n", err);
        fprintf(foutput, "\n%d errors in pl/0 program!\n", err);
    }

    fclose(ftable);
    fclose(foutput);
    fclose(fin);

    return 0;
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

void init() {
    int i;

    /* 设置单字符符号 */
    for (i = 0; i <= 255; i++) {
        ssym[i] = nul;
    }
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = slash;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['='] = eql;
    ssym[','] = comma;
    ssym['.'] = period;
    ssym['#'] = neq;
    ssym[';'] = semicolon;

    /* 设置保留字名字,按照字母顺序，便于二分查找 */
    strcpy(&(word[0][0]), "begin");
    strcpy(&(word[1][0]), "call");
    strcpy(&(word[2][0]), "const");
    strcpy(&(word[3][0]), "do");
    strcpy(&(word[4][0]), "end");
    strcpy(&(word[5][0]), "if");
    strcpy(&(word[6][0]), "odd");
    strcpy(&(word[7][0]), "procedure");
    strcpy(&(word[8][0]), "read");
    strcpy(&(word[9][0]), "then");
    strcpy(&(word[10][0]), "var");
    strcpy(&(word[11][0]), "while");
    strcpy(&(word[12][0]), "write");

    /* 设置保留字符号 */
    wsym[0] = beginsym;
    wsym[1] = callsym;
    wsym[2] = constsym;
    wsym[3] = dosym;
    wsym[4] = endsym;
    wsym[5] = ifsym;
    wsym[6] = oddsym;
    wsym[7] = procsym;
    wsym[8] = readsym;
    wsym[9] = thensym;
    wsym[10] = varsym;
    wsym[11] = whilesym;
    wsym[12] = writesym;

    /* 设置指令名称 */
    strcpy(&(mnemonic[lit][0]), "lit");
    strcpy(&(mnemonic[opr][0]), "opr");
    strcpy(&(mnemonic[lod][0]), "lod");
    strcpy(&(mnemonic[sto][0]), "sto");
    strcpy(&(mnemonic[cal][0]), "cal");
    strcpy(&(mnemonic[ini][0]), "int");
    strcpy(&(mnemonic[jmp][0]), "jmp");
    strcpy(&(mnemonic[jpc][0]), "jpc");

    /* 设置符号集 */
    for (i = 0; i < symnum; i++) {
        declbegsys[i] = false;
        statbegsys[i] = false;
        facbegsys[i] = false;
    }

    /* 设置声明开始符号集 */
    declbegsys[constsym] = true;
    declbegsys[varsym] = true;
    declbegsys[procsym] = true;

    /* 设置语句开始符号集 */
    statbegsys[beginsym] = true;
    statbegsys[callsym] = true;
    statbegsys[ifsym] = true;
    statbegsys[whilesym] = true;

    /* 设置因子开始符号集 */
    facbegsys[ident] = true;
    facbegsys[number] = true;
    facbegsys[lparen] = true;

}