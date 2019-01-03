//
// Created by dmrfcoder on 2019/1/3.
//

#ifndef COMPILATIONPRINCIPLECOURSEDESGIN_ENUMS_H
#define COMPILATIONPRINCIPLECOURSEDESGIN_ENUMS_H



enum symbol {
    nul, ident, number, plus, minus,
    times, slash, oddsym, eql, neq,
    lss, leq, gtr, geq, lparen,
    rparen, comma, semicolon, period, becomes,
    beginsym, endsym, ifsym, thensym, whilesym,
    writesym, readsym, dosym, callsym, constsym,
    varsym, procsym,
};

/* 符号表中的类型 */
enum object {
    constant,
    variable,
    procedure,
};

/* 虚拟机代码指令 */
enum fct {
    lit, opr, lod,
    sto, cal, ini,
    jmp, jpc,
};

#endif //COMPILATIONPRINCIPLECOURSEDESGIN_ENUMS_H