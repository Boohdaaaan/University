#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <string>
#include <sstream>

class CodeGenerator {
public:
    CodeGenerator();
    std::string generate(std::vector<std::unique_ptr<StatementNode>>& ast);
    
private:
    std::ostringstream output;
    int indentLevel;
    
    void generateStatement(StatementNode* stmt);
    void generateExpression(ExpressionNode* expr);
    void generateFunction(FunctionNode* func);
    void generateVarDecl(VarDeclNode* decl);
    void generateAssign(AssignNode* assign);
    void generateIf(IfNode* ifNode);
    void generateWhile(WhileNode* whileNode);
    void generateFor(ForNode* forNode);
    void generateBlock(BlockNode* block);
    void generateReturn(ReturnNode* returnNode);
    void generateBreak(BreakNode* breakNode);
    void generateContinue(ContinueNode* continueNode);
    void generateCall(CallNode* call);
    
    void indent();
    void newline();
};

#endif // CODEGEN_H

