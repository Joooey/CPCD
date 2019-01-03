#include <iostream>
#include "defines.h"
#include "Enums.h"
#include "Structs.h"
#include "GlobalVar.h"
#include "Interpret.h"
#include "GetUtils.h"
#include "Block.h"


void init();



int addset(bool* sr, bool* s1, bool* s2, int n);



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




int addset(bool* sr, bool* s1, bool* s2, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        sr[i] = s1[i]||s2[i];
    }
    return 0;
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