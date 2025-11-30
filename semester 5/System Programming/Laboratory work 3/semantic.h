#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

enum class SymbolType {
    VARIABLE,
    FUNCTION,
    TYPE
};

struct Symbol {
    SymbolType type;
    std::string name;
    std::string dataType;
    bool isDefined;
    
    Symbol() : type(SymbolType::VARIABLE), isDefined(false) {}
    Symbol(SymbolType t, const std::string& n, const std::string& dt)
        : type(t), name(n), dataType(dt), isDefined(false) {}
};

class SymbolTable {
public:
    void enterScope();
    void exitScope();
    bool addSymbol(const std::string& name, SymbolType type, const std::string& dataType);
    Symbol* lookup(const std::string& name);
    bool isInCurrentScope(const std::string& name);
    
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
};

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    bool analyze(std::vector<std::unique_ptr<StatementNode>>& ast);
    std::string getErrors() const { return errors; }
    
private:
    SymbolTable symbolTable;
    std::string errors;
    
    void analyzeStatement(StatementNode* stmt);
    void analyzeExpression(ExpressionNode* expr);
    void analyzeFunction(FunctionNode* func);
    void analyzeVarDecl(VarDeclNode* decl);
    void analyzeAssign(AssignNode* assign);
    void analyzeIf(IfNode* ifNode);
    void analyzeWhile(WhileNode* whileNode);
    void analyzeFor(ForNode* forNode);
    void analyzeBlock(BlockNode* block);
    void analyzeCall(CallNode* call);
    
    std::string getExpressionType(ExpressionNode* expr);
    bool isConstantExpression(ExpressionNode* expr);
    int evaluateConstantExpression(ExpressionNode* expr);
};

#endif // SEMANTIC_H

