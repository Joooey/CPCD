#include <iostream>

#include "Defines.h"
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


    if ((fin = fopen("../1.txt", "r")) == NULL) {
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
        printf("Can't opedefinesn ftable.txt file!\n");
        exit(1);
    }




    listswitch = 1;


    tableswitch = true;

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







