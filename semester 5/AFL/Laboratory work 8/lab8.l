%{
#include <stdio.h>
#include <string.h>

int col_num = 1;
%}

%option yylineno
%option noyywrap

%x IN_SINGLE_STRING
%x IN_DOUBLE_STRING
%x IN_TRIPLE_SINGLE_STRING
%x IN_TRIPLE_DOUBLE_STRING
%x IN_RAW_SINGLE_STRING
%x IN_RAW_DOUBLE_STRING
%x IN_RAW_TRIPLE_SINGLE_STRING
%x IN_RAW_TRIPLE_DOUBLE_STRING
%x IN_F_SINGLE_STRING
%x IN_F_DOUBLE_STRING
%x IN_F_TRIPLE_SINGLE_STRING
%x IN_F_TRIPLE_DOUBLE_STRING

%%

<INITIAL>["'] {
    if (yytext[0] == '"') {
        BEGIN(IN_DOUBLE_STRING);
    } else {
        BEGIN(IN_SINGLE_STRING);
    }
    col_num += yyleng;
}

<INITIAL>r["']|R["'] {
    if (yytext[1] == '"') {
        BEGIN(IN_RAW_DOUBLE_STRING);
    } else {
        BEGIN(IN_RAW_SINGLE_STRING);
    }
    col_num += yyleng;
}

<INITIAL>f["']|F["'] {
    if (yytext[1] == '"') {
        BEGIN(IN_F_DOUBLE_STRING);
    } else {
        BEGIN(IN_F_SINGLE_STRING);
    }
    col_num += yyleng;
}

<INITIAL>r\"\"\"|R\"\"\" {
    BEGIN(IN_RAW_TRIPLE_DOUBLE_STRING);
    col_num += yyleng;
}

<INITIAL>r'''|R''' {
    BEGIN(IN_RAW_TRIPLE_SINGLE_STRING);
    col_num += yyleng;
}

<INITIAL>f\"\"\"|F\"\"\" {
    BEGIN(IN_F_TRIPLE_DOUBLE_STRING);
    col_num += yyleng;
}

<INITIAL>f'''|F''' {
    BEGIN(IN_F_TRIPLE_SINGLE_STRING);
    col_num += yyleng;
}

<INITIAL>\"\"\" {
    BEGIN(IN_TRIPLE_DOUBLE_STRING);
    col_num += yyleng;
}

<INITIAL>''' {
    BEGIN(IN_TRIPLE_SINGLE_STRING);
    col_num += yyleng;
}

<IN_SINGLE_STRING>['] {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_DOUBLE_STRING>["] {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_TRIPLE_SINGLE_STRING>''' {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_TRIPLE_DOUBLE_STRING>\"\"\" {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_RAW_SINGLE_STRING>['] {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_RAW_DOUBLE_STRING>["] {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_RAW_TRIPLE_SINGLE_STRING>''' {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_RAW_TRIPLE_DOUBLE_STRING>\"\"\" {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_F_SINGLE_STRING>['] {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_F_DOUBLE_STRING>["] {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_F_TRIPLE_SINGLE_STRING>''' {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_F_TRIPLE_DOUBLE_STRING>\"\"\" {
    BEGIN(INITIAL);
    col_num += yyleng;
}

<IN_RAW_SINGLE_STRING,IN_RAW_DOUBLE_STRING,IN_RAW_TRIPLE_SINGLE_STRING,IN_RAW_TRIPLE_DOUBLE_STRING>\\\\[^\\\n\r\t"']+(\\[^\\\n\r\t"']+)* {
    printf("[%d:%d] %s\n", yylineno, col_num, yytext);
    col_num += yyleng;
}

<IN_RAW_SINGLE_STRING,IN_RAW_DOUBLE_STRING,IN_RAW_TRIPLE_SINGLE_STRING,IN_RAW_TRIPLE_DOUBLE_STRING>([A-Za-z]:)?(\\[^\\\n\r\t"']+)+ {
    printf("[%d:%d] %s\n", yylineno, col_num, yytext);
    col_num += yyleng;
}

<IN_SINGLE_STRING,IN_DOUBLE_STRING,IN_TRIPLE_SINGLE_STRING,IN_TRIPLE_DOUBLE_STRING,IN_F_SINGLE_STRING,IN_F_DOUBLE_STRING,IN_F_TRIPLE_SINGLE_STRING,IN_F_TRIPLE_DOUBLE_STRING>\\\\\\\\[^\\\n\r\t"']+(\\\\[^\\\n\r\t"']+)* {
    printf("[%d:%d] %s\n", yylineno, col_num, yytext);
    col_num += yyleng;
}

<IN_SINGLE_STRING,IN_DOUBLE_STRING,IN_TRIPLE_SINGLE_STRING,IN_TRIPLE_DOUBLE_STRING,IN_F_SINGLE_STRING,IN_F_DOUBLE_STRING,IN_F_TRIPLE_SINGLE_STRING,IN_F_TRIPLE_DOUBLE_STRING>([A-Za-z]:)?(\\\\[^\\\n\r\t"']+)+ {
    printf("[%d:%d] %s\n", yylineno, col_num, yytext);
    col_num += yyleng;
}

<IN_SINGLE_STRING,IN_DOUBLE_STRING,IN_TRIPLE_SINGLE_STRING,IN_TRIPLE_DOUBLE_STRING,IN_RAW_SINGLE_STRING,IN_RAW_DOUBLE_STRING,IN_RAW_TRIPLE_SINGLE_STRING,IN_RAW_TRIPLE_DOUBLE_STRING,IN_F_SINGLE_STRING,IN_F_DOUBLE_STRING,IN_F_TRIPLE_SINGLE_STRING,IN_F_TRIPLE_DOUBLE_STRING>\\\\. {
    col_num += yyleng;
}

<IN_SINGLE_STRING,IN_DOUBLE_STRING,IN_TRIPLE_SINGLE_STRING,IN_TRIPLE_DOUBLE_STRING,IN_RAW_SINGLE_STRING,IN_RAW_DOUBLE_STRING,IN_RAW_TRIPLE_SINGLE_STRING,IN_RAW_TRIPLE_DOUBLE_STRING,IN_F_SINGLE_STRING,IN_F_DOUBLE_STRING,IN_F_TRIPLE_SINGLE_STRING,IN_F_TRIPLE_DOUBLE_STRING>. {
    col_num += yyleng;
}

<IN_SINGLE_STRING,IN_DOUBLE_STRING,IN_TRIPLE_SINGLE_STRING,IN_TRIPLE_DOUBLE_STRING,IN_RAW_SINGLE_STRING,IN_RAW_DOUBLE_STRING,IN_RAW_TRIPLE_SINGLE_STRING,IN_RAW_TRIPLE_DOUBLE_STRING,IN_F_SINGLE_STRING,IN_F_DOUBLE_STRING,IN_F_TRIPLE_SINGLE_STRING,IN_F_TRIPLE_DOUBLE_STRING>\n {
    col_num = 1;
}

<INITIAL>\n {
    col_num = 1;
}

<INITIAL>. {
    col_num += yyleng;
}

%%

int main(int argc, char *argv[]) {
    FILE *file;
    
    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Помилка: не вдалося відкрити файл %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    } else {
        fprintf(stderr, "Використання: %s <файл.py>\n", argv[0]);
        return 1;
    }
    
    yylex();
    
    if (file != stdin) {
        fclose(file);
    }
    
    return 0;
}
