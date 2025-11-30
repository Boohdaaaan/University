%language "c++"
%define api.value.type variant
%define parse.error verbose
%locations

%code requires {
#include "ast.h"
#include <memory>
#include <vector>

// Global AST root - exported for main.cpp
extern std::vector<std::unique_ptr<StatementNode>> g_ast;
}

%code provides {
int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

extern int yylineno;
extern FILE* yyin;

static std::string take_owned_text(char* text) {
    if (!text) {
        return std::string();
    }
    std::string result(text);
    free(text);
    return result;
}

%}

%token <int> INTEGER_LITERAL
%token <char*> IDENTIFIER STRING_LITERAL CHAR_LITERAL FLOAT_LITERAL
%token INT CHAR FLOAT_TYPE DOUBLE VOID
%token IF ELSE WHILE FOR RETURN BREAK CONTINUE DO SWITCH CASE DEFAULT
%token STRUCT TYPEDEF CONST STATIC EXTERN SIZEOF
%token EQ NE LE GE AND OR INC DEC
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN OR_ASSIGN XOR_ASSIGN
%token LSHIFT RSHIFT ARROW

%nterm <std::unique_ptr<ExpressionNode>> expression
%nterm <std::unique_ptr<StatementNode>> statement
%nterm <std::unique_ptr<BlockNode>> block
%nterm <std::vector<std::unique_ptr<StatementNode>>> statement_list
%nterm <std::vector<std::unique_ptr<ExpressionNode>>> expression_list
%nterm <std::pair<std::string, std::string>> parameter
%nterm <std::vector<std::pair<std::string, std::string>>> parameter_list
%nterm <std::string> type_specifier

%left OR
%left AND
%left EQ NE '<' '>' LE GE
%left LSHIFT RSHIFT
%left '+' '-'
%left '*' '/' '%'
%left '!' '~' INC DEC
%right '='
%left ARROW '.'
%left '(' '['

%%

program:
    translation_unit { }
    ;

translation_unit:
    /* empty */ { }
    | translation_unit function_definition { }
    | translation_unit declaration { }
    ;

function_definition:
    type_specifier IDENTIFIER '(' parameter_list ')' block {
        g_ast.push_back(std::make_unique<FunctionNode>(take_owned_text($2), $1, std::move($4), std::move($6), yylineno));
    }
    | type_specifier IDENTIFIER '(' ')' block {
        std::vector<std::pair<std::string, std::string>> params;
        g_ast.push_back(std::make_unique<FunctionNode>(take_owned_text($2), $1, std::move(params), std::move($5), yylineno));
    }
    ;

parameter_list:
    parameter {
        $$ = std::vector<std::pair<std::string, std::string>>();
        $$.push_back($1);
    }
    | parameter_list ',' parameter {
        $$ = std::move($1);
        $$.push_back($3);
    }
    ;

parameter:
    type_specifier IDENTIFIER {
        $$ = std::make_pair($1, take_owned_text($2));
    }
    ;

declaration:
    type_specifier IDENTIFIER ';' {
        g_ast.push_back(std::make_unique<VarDeclNode>($1, take_owned_text($2), nullptr, yylineno));
    }
    | type_specifier IDENTIFIER '=' expression ';' {
        g_ast.push_back(std::make_unique<VarDeclNode>($1, take_owned_text($2), std::move($4), yylineno));
    }
    ;

type_specifier:
    INT { $$ = "int"; }
    | CHAR { $$ = "char"; }
    | FLOAT_TYPE { $$ = "float"; }
    | DOUBLE { $$ = "double"; }
    | VOID { $$ = "void"; }
    ;

statement:
    expression ';' {
        // Expression statement - ignore result
        $$ = nullptr;
    }
    | block {
        $$ = std::move($1);
    }
    | IF '(' expression ')' statement {
        std::unique_ptr<BlockNode> thenBlock = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        if ($5) {
            thenBlock->getStatements().push_back(std::move($5));
        }
        $$ = std::make_unique<IfNode>(std::move($3), std::move(thenBlock), nullptr, yylineno);
    }
    | IF '(' expression ')' statement ELSE statement {
        std::unique_ptr<BlockNode> thenBlock = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        std::unique_ptr<BlockNode> elseBlock = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        if ($5) {
            thenBlock->getStatements().push_back(std::move($5));
        }
        if ($7) {
            elseBlock->getStatements().push_back(std::move($7));
        }
        $$ = std::make_unique<IfNode>(std::move($3), std::move(thenBlock), std::move(elseBlock), yylineno);
    }
    | WHILE '(' expression ')' statement {
        std::unique_ptr<BlockNode> body = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        if ($5) {
            body->getStatements().push_back(std::move($5));
        }
        $$ = std::make_unique<WhileNode>(std::move($3), std::move(body), yylineno);
    }
    | FOR '(' expression ';' expression ';' expression ')' statement {
        std::unique_ptr<BlockNode> body = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>());
        if ($9) {
            body->getStatements().push_back(std::move($9));
        }
        $$ = std::make_unique<ForNode>(nullptr, std::move($5), std::move($7), std::move(body), yylineno);
    }
    | RETURN ';' {
        $$ = std::make_unique<ReturnNode>(nullptr, yylineno);
    }
    | RETURN expression ';' {
        $$ = std::make_unique<ReturnNode>(std::move($2), yylineno);
    }
    | BREAK ';' {
        $$ = std::make_unique<BreakNode>(yylineno);
    }
    | CONTINUE ';' {
        $$ = std::make_unique<ContinueNode>(yylineno);
    }
    | type_specifier IDENTIFIER ';' {
        $$ = std::make_unique<VarDeclNode>($1, take_owned_text($2), nullptr, yylineno);
    }
    | type_specifier IDENTIFIER '=' expression ';' {
        $$ = std::make_unique<VarDeclNode>($1, take_owned_text($2), std::move($4), yylineno);
    }
    | IDENTIFIER '=' expression ';' {
        $$ = std::make_unique<AssignNode>(take_owned_text($1), std::move($3), yylineno);
    }
    ;

block:
    '{' statement_list '}' {
        $$ = std::make_unique<BlockNode>(std::move($2), yylineno);
    }
    | '{' '}' {
        $$ = std::make_unique<BlockNode>(std::vector<std::unique_ptr<StatementNode>>(), yylineno);
    }
    ;

statement_list:
    statement {
        $$ = std::vector<std::unique_ptr<StatementNode>>();
        if ($1) {
            $$.push_back(std::move($1));
        }
    }
    | statement_list statement {
        $$ = std::move($1);
        if ($2) {
            $$.push_back(std::move($2));
        }
    }
    ;

expression:
    INTEGER_LITERAL {
        $$ = std::make_unique<LiteralNode>(std::to_string($1), "int", yylineno);
    }
    | FLOAT_LITERAL {
        $$ = std::make_unique<LiteralNode>(take_owned_text($1), "float", yylineno);
    }
    | STRING_LITERAL {
        $$ = std::make_unique<LiteralNode>(take_owned_text($1), "string", yylineno);
    }
    | CHAR_LITERAL {
        $$ = std::make_unique<LiteralNode>(take_owned_text($1), "char", yylineno);
    }
    | IDENTIFIER {
        $$ = std::make_unique<IdentifierNode>(take_owned_text($1), yylineno);
    }
    | IDENTIFIER '(' expression_list ')' {
        $$ = std::make_unique<CallNode>(take_owned_text($1), std::move($3), yylineno);
    }
    | IDENTIFIER '(' ')' {
        std::vector<std::unique_ptr<ExpressionNode>> args;
        $$ = std::make_unique<CallNode>(take_owned_text($1), std::move(args), yylineno);
    }
    | '(' expression ')' {
        $$ = std::move($2);
    }
    | expression '+' expression {
        $$ = std::make_unique<BinaryOpNode>("+", std::move($1), std::move($3), yylineno);
    }
    | expression '-' expression {
        $$ = std::make_unique<BinaryOpNode>("-", std::move($1), std::move($3), yylineno);
    }
    | expression '*' expression {
        $$ = std::make_unique<BinaryOpNode>("*", std::move($1), std::move($3), yylineno);
    }
    | expression '/' expression {
        $$ = std::make_unique<BinaryOpNode>("/", std::move($1), std::move($3), yylineno);
    }
    | expression '%' expression {
        $$ = std::make_unique<BinaryOpNode>("%", std::move($1), std::move($3), yylineno);
    }
    | expression EQ expression {
        $$ = std::make_unique<BinaryOpNode>("==", std::move($1), std::move($3), yylineno);
    }
    | expression NE expression {
        $$ = std::make_unique<BinaryOpNode>("!=", std::move($1), std::move($3), yylineno);
    }
    | expression '<' expression {
        $$ = std::make_unique<BinaryOpNode>("<", std::move($1), std::move($3), yylineno);
    }
    | expression '>' expression {
        $$ = std::make_unique<BinaryOpNode>(">", std::move($1), std::move($3), yylineno);
    }
    | expression LE expression {
        $$ = std::make_unique<BinaryOpNode>("<=", std::move($1), std::move($3), yylineno);
    }
    | expression GE expression {
        $$ = std::make_unique<BinaryOpNode>(">=", std::move($1), std::move($3), yylineno);
    }
    | expression AND expression {
        $$ = std::make_unique<BinaryOpNode>("&&", std::move($1), std::move($3), yylineno);
    }
    | expression OR expression {
        $$ = std::make_unique<BinaryOpNode>("||", std::move($1), std::move($3), yylineno);
    }
    | '!' expression {
        $$ = std::make_unique<UnaryOpNode>("!", std::move($2), yylineno);
    }
    | '-' expression {
        $$ = std::make_unique<UnaryOpNode>("-", std::move($2), yylineno);
    }
    | '+' expression {
        $$ = std::make_unique<UnaryOpNode>("+", std::move($2), yylineno);
    }
    | expression '=' expression {
        $$ = std::make_unique<BinaryOpNode>("=", std::move($1), std::move($3), yylineno);
    }
    ;

expression_list:
    expression {
        $$ = std::vector<std::unique_ptr<ExpressionNode>>();
        $$.push_back(std::move($1));
    }
    | expression_list ',' expression {
        $$ = std::move($1);
        $$.push_back(std::move($3));
    }
    ;

%%

void yy::parser::error(const location_type& loc, const std::string& msg) {
    std::cerr << "Error at line " << loc.begin.line << ": " << msg << std::endl;
}

