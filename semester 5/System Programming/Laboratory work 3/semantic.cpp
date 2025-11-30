#include "semantic.h"
#include <sstream>
#include <iostream>

void SymbolTable::enterScope() {
    scopes.push_back(std::unordered_map<std::string, Symbol>());
}

void SymbolTable::exitScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

bool SymbolTable::addSymbol(const std::string& name, SymbolType type, const std::string& dataType) {
    if (scopes.empty()) {
        enterScope();
    }
    
    if (isInCurrentScope(name)) {
        return false; // Symbol already exists in current scope
    }
    
    scopes.back()[name] = Symbol(type, name, dataType);
    return true;
}

Symbol* SymbolTable::lookup(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return &found->second;
        }
    }
    return nullptr;
}

bool SymbolTable::isInCurrentScope(const std::string& name) {
    if (scopes.empty()) {
        return false;
    }
    return scopes.back().find(name) != scopes.back().end();
}

SemanticAnalyzer::SemanticAnalyzer() {
    symbolTable.enterScope();
}

bool SemanticAnalyzer::analyze(std::vector<std::unique_ptr<StatementNode>>& ast) {
    errors.clear();
    bool hasErrors = false;
    
    for (auto& stmt : ast) {
        analyzeStatement(stmt.get());
    }
    
    return errors.empty();
}

void SemanticAnalyzer::analyzeStatement(StatementNode* stmt) {
    if (!stmt) return;
    
    switch (stmt->getType()) {
        case ASTNode::NODE_FUNCTION:
            analyzeFunction(static_cast<FunctionNode*>(stmt));
            break;
        case ASTNode::NODE_VAR_DECL:
            analyzeVarDecl(static_cast<VarDeclNode*>(stmt));
            break;
        case ASTNode::NODE_ASSIGN:
            analyzeAssign(static_cast<AssignNode*>(stmt));
            break;
        case ASTNode::NODE_IF:
            analyzeIf(static_cast<IfNode*>(stmt));
            break;
        case ASTNode::NODE_WHILE:
            analyzeWhile(static_cast<WhileNode*>(stmt));
            break;
        case ASTNode::NODE_FOR:
            analyzeFor(static_cast<ForNode*>(stmt));
            break;
        case ASTNode::NODE_BLOCK:
            analyzeBlock(static_cast<BlockNode*>(stmt));
            break;
        case ASTNode::NODE_RETURN:
        case ASTNode::NODE_BREAK:
        case ASTNode::NODE_CONTINUE:
            // No semantic analysis needed
            break;
        default:
            break;
    }
}

void SemanticAnalyzer::analyzeExpression(ExpressionNode* expr) {
    if (!expr) return;
    
    switch (expr->getType()) {
        case ASTNode::NODE_IDENTIFIER: {
            IdentifierNode* id = static_cast<IdentifierNode*>(expr);
            Symbol* sym = symbolTable.lookup(id->getName());
            if (!sym) {
                std::ostringstream oss;
                oss << "Line " << expr->getLine() << ": Undefined identifier '" << id->getName() << "'\n";
                errors += oss.str();
            }
            break;
        }
        case ASTNode::NODE_CALL:
            analyzeCall(static_cast<CallNode*>(expr));
            break;
        case ASTNode::NODE_BINARY_OP:
        case ASTNode::NODE_UNARY_OP:
        case ASTNode::NODE_LITERAL:
            // Type checking can be added here
            break;
        default:
            break;
    }
}

void SemanticAnalyzer::analyzeFunction(FunctionNode* func) {
    if (!func) return;
    
    symbolTable.enterScope();
    
    // Add parameters to symbol table
    for (const auto& param : func->getParams()) {
        symbolTable.addSymbol(param.second, SymbolType::VARIABLE, param.first);
    }
    
    // Analyze function body
    if (func->getBody()) {
        analyzeBlock(func->getBody());
    }
    
    symbolTable.exitScope();
}

void SemanticAnalyzer::analyzeVarDecl(VarDeclNode* decl) {
    if (!decl) return;
    
    // Add variable to symbol table
    if (!symbolTable.addSymbol(decl->getName(), SymbolType::VARIABLE, decl->getVarType())) {
        std::ostringstream oss;
        oss << "Line " << decl->getLine() << ": Variable '" << decl->getName() << "' already declared in this scope\n";
        errors += oss.str();
    }
    
    if (decl->getInitializer()) {
        analyzeExpression(decl->getInitializer());
    }
}

void SemanticAnalyzer::analyzeAssign(AssignNode* assign) {
    if (!assign) return;
    
    // Check if variable exists
    Symbol* sym = symbolTable.lookup(assign->getName());
    if (!sym) {
        std::ostringstream oss;
        oss << "Line " << assign->getLine() << ": Assignment to undefined variable '" << assign->getName() << "'\n";
        errors += oss.str();
    }
    
    if (assign->getValue()) {
        analyzeExpression(assign->getValue());
    }
}

void SemanticAnalyzer::analyzeIf(IfNode* ifNode) {
    if (!ifNode) return;
    
    analyzeExpression(ifNode->getCondition());
    symbolTable.enterScope();
    if (ifNode->getThenBlock()) {
        analyzeBlock(ifNode->getThenBlock());
    }
    symbolTable.exitScope();
    
    if (ifNode->getElseBlock()) {
        symbolTable.enterScope();
        analyzeBlock(ifNode->getElseBlock());
        symbolTable.exitScope();
    }
}

void SemanticAnalyzer::analyzeWhile(WhileNode* whileNode) {
    if (!whileNode) return;
    
    analyzeExpression(whileNode->getCondition());
    symbolTable.enterScope();
    if (whileNode->getBody()) {
        analyzeBlock(whileNode->getBody());
    }
    symbolTable.exitScope();
}

void SemanticAnalyzer::analyzeFor(ForNode* forNode) {
    if (!forNode) return;
    
    symbolTable.enterScope();
    
    if (forNode->getInit()) {
        analyzeStatement(forNode->getInit());
    }
    if (forNode->getCondition()) {
        analyzeExpression(forNode->getCondition());
    }
    if (forNode->getIncrement()) {
        analyzeExpression(forNode->getIncrement());
    }
    if (forNode->getBody()) {
        analyzeBlock(forNode->getBody());
    }
    
    symbolTable.exitScope();
}

void SemanticAnalyzer::analyzeBlock(BlockNode* block) {
    if (!block) return;
    
    symbolTable.enterScope();
    
    for (auto& stmt : block->getStatements()) {
        analyzeStatement(stmt.get());
    }
    
    symbolTable.exitScope();
}

void SemanticAnalyzer::analyzeCall(CallNode* call) {
    if (!call) return;
    
    Symbol* sym = symbolTable.lookup(call->getName());
    if (!sym || sym->type != SymbolType::FUNCTION) {
        std::ostringstream oss;
        oss << "Line " << call->getLine() << ": Call to undefined function '" << call->getName() << "'\n";
        errors += oss.str();
    }
    
    // Analyze arguments
    for (const auto& arg : call->getArgs()) {
        analyzeExpression(arg.get());
    }
}

std::string SemanticAnalyzer::getExpressionType(ExpressionNode* expr) {
    if (!expr) return "unknown";
    
    switch (expr->getType()) {
        case ASTNode::NODE_LITERAL: {
            LiteralNode* lit = static_cast<LiteralNode*>(expr);
            // Extract type from literal node - would need access to private members
            return "int"; // Simplified
        }
        case ASTNode::NODE_IDENTIFIER: {
            IdentifierNode* id = static_cast<IdentifierNode*>(expr);
            Symbol* sym = symbolTable.lookup(id->getName());
            return sym ? sym->dataType : "unknown";
        }
        default:
            return "unknown";
    }
}

bool SemanticAnalyzer::isConstantExpression(ExpressionNode* expr) {
    if (!expr) return false;
    
    return expr->getType() == ASTNode::NODE_LITERAL;
}

int SemanticAnalyzer::evaluateConstantExpression(ExpressionNode* expr) {
    if (!isConstantExpression(expr)) {
        return 0;
    }
    
    LiteralNode* lit = static_cast<LiteralNode*>(expr);
    // Would need to parse the literal value
    return 0; // Simplified
}

