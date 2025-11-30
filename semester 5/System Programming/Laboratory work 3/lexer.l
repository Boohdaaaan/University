%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.tab.hh"

extern int yylineno;

// Global variables for C++ Bison integration
yy::parser::semantic_type* yylval_ptr = nullptr;
yy::parser::location_type* yylloc_ptr = nullptr;

static char* alloc_token_text(const char* text) {
    size_t len = strlen(text);
    char* copy = (char*)malloc(len + 1);
    if (!copy) {
        fprintf(stderr, "Out of memory on lexer allocation at line %d\n", yylineno);
        exit(EXIT_FAILURE);
    }
    memcpy(copy, text, len + 1);
    return copy;
}

%}

%option noyywrap
%option yylineno

DIGIT       [0-9]
LETTER      [a-zA-Z_]
HEXDIGIT    [0-9a-fA-F]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
INTEGER     {DIGIT}+
FLOAT       {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?
HEX         (0[xX]{HEXDIGIT}+)
STRING      \"([^"\\]|\\.)*\"
CHAR        \'([^\'\\]|\\.)\'
WHITESPACE  [ \t\r]+
NEWLINE     \n

%%

"/*"            { /* C-style comment */
                  int c;
                  while ((c = yyinput()) != EOF) {
                      if (c == '*') {
                          c = yyinput();
                          if (c == '/') break;
                          if (c != EOF) unput(c);
                      }
                      if (c == '\n') yylineno++;
                  }
                }
"//"            { /* C++-style comment */
                  int c;
                  while ((c = yyinput()) != EOF && c != '\n');
                  if (c == '\n') yylineno++;
                }

"int"           { return yy::parser::token::INT; }
"char"          { return yy::parser::token::CHAR; }
"float"         { return yy::parser::token::FLOAT_TYPE; }
"double"        { return yy::parser::token::DOUBLE; }
"void"          { return yy::parser::token::VOID; }
"if"            { return yy::parser::token::IF; }
"else"          { return yy::parser::token::ELSE; }
"while"         { return yy::parser::token::WHILE; }
"for"           { return yy::parser::token::FOR; }
"return"        { return yy::parser::token::RETURN; }
"break"         { return yy::parser::token::BREAK; }
"continue"      { return yy::parser::token::CONTINUE; }
"do"            { return yy::parser::token::DO; }
"switch"        { return yy::parser::token::SWITCH; }
"case"          { return yy::parser::token::CASE; }
"default"       { return yy::parser::token::DEFAULT; }
"struct"        { return yy::parser::token::STRUCT; }
"typedef"       { return yy::parser::token::TYPEDEF; }
"const"         { return yy::parser::token::CONST; }
"static"        { return yy::parser::token::STATIC; }
"extern"        { return yy::parser::token::EXTERN; }
"sizeof"        { return yy::parser::token::SIZEOF; }

{HEX}           { if (yylval_ptr) yylval_ptr->emplace<int>(strtol(yytext, NULL, 0)); return yy::parser::token::INTEGER_LITERAL; }
{INTEGER}       { if (yylval_ptr) yylval_ptr->emplace<int>(atoi(yytext)); return yy::parser::token::INTEGER_LITERAL; }
{FLOAT}         { if (yylval_ptr) yylval_ptr->emplace<char*>(alloc_token_text(yytext)); return yy::parser::token::FLOAT_LITERAL; }
{STRING}        { if (yylval_ptr) yylval_ptr->emplace<char*>(alloc_token_text(yytext)); return yy::parser::token::STRING_LITERAL; }
{CHAR}          { if (yylval_ptr) yylval_ptr->emplace<char*>(alloc_token_text(yytext)); return yy::parser::token::CHAR_LITERAL; }
{IDENTIFIER}    { if (yylval_ptr) yylval_ptr->emplace<char*>(alloc_token_text(yytext)); return yy::parser::token::IDENTIFIER; }

"=="            { return yy::parser::token::EQ; }
"!="            { return yy::parser::token::NE; }
"<="            { return yy::parser::token::LE; }
">="            { return yy::parser::token::GE; }
"&&"            { return yy::parser::token::AND; }
"||"            { return yy::parser::token::OR; }
"++"            { return yy::parser::token::INC; }
"--"            { return yy::parser::token::DEC; }
"+="            { return yy::parser::token::ADD_ASSIGN; }
"-="            { return yy::parser::token::SUB_ASSIGN; }
"*="            { return yy::parser::token::MUL_ASSIGN; }
"/="            { return yy::parser::token::DIV_ASSIGN; }
"%="            { return yy::parser::token::MOD_ASSIGN; }
"<<="           { return yy::parser::token::LSHIFT_ASSIGN; }
">>="           { return yy::parser::token::RSHIFT_ASSIGN; }
"&="            { return yy::parser::token::AND_ASSIGN; }
"|="            { return yy::parser::token::OR_ASSIGN; }
"^="            { return yy::parser::token::XOR_ASSIGN; }
"<<"            { return yy::parser::token::LSHIFT; }
">>"            { return yy::parser::token::RSHIFT; }
"->"            { return yy::parser::token::ARROW; }

"<"             { return '<'; }
">"             { return '>'; }
"="             { return '='; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"%"             { return '%'; }
"!"             { return '!'; }
"&"             { return '&'; }
"|"             { return '|'; }
"^"             { return '^'; }
"~"             { return '~'; }
"?"             { return '?'; }
":"             { return ':'; }
";"             { return ';'; }
","             { return ','; }
"."             { return '.'; }
"("             { return '('; }
")"             { return ')'; }
"["             { return '['; }
"]"             { return ']'; }
"{"             { return '{'; }
"}"             { return '}'; }

{WHITESPACE}    { /* ignore whitespace */ }
{NEWLINE}       { yylineno++; /* ignore newlines */ }

.               { return yytext[0]; }

%%

// Wrapper function for C++ Bison
int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc) {
    yylval_ptr = yylval;
    yylloc_ptr = yylloc;
    return ::yylex();
}
