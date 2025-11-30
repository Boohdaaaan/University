#include "codegen.h"
#include <iostream>

CodeGenerator::CodeGenerator() : indentLevel(0) {
}

std::string CodeGenerator::generate(std::vector<std::unique_ptr<StatementNode>>& ast) {
    output.str("");
    output.clear();
    indentLevel = 0;
    
    for (auto& stmt : ast) {
        generateStatement(stmt.get());
    }
    
    return output.str();
}

void CodeGenerator::indent() {
    for (int i = 0; i < indentLevel; i++) {
        output << "  ";
    }
}

void CodeGenerator::newline() {
    output << "\n";
}

void CodeGenerator::generateStatement(StatementNode* stmt) {
    if (!stmt) return;
    
    switch (stmt->getType()) {
        case ASTNode::NODE_FUNCTION:
            generateFunction(static_cast<FunctionNode*>(stmt));
            break;
        case ASTNode::NODE_VAR_DECL:
            generateVarDecl(static_cast<VarDeclNode*>(stmt));
            break;
        case ASTNode::NODE_ASSIGN:
            generateAssign(static_cast<AssignNode*>(stmt));
            break;
        case ASTNode::NODE_IF:
            generateIf(static_cast<IfNode*>(stmt));
            break;
        case ASTNode::NODE_WHILE:
            generateWhile(static_cast<WhileNode*>(stmt));
            break;
        case ASTNode::NODE_FOR:
            generateFor(static_cast<ForNode*>(stmt));
            break;
        case ASTNode::NODE_BLOCK:
            generateBlock(static_cast<BlockNode*>(stmt));
            break;
        case ASTNode::NODE_RETURN:
            generateReturn(static_cast<ReturnNode*>(stmt));
            break;
        case ASTNode::NODE_BREAK:
            generateBreak(static_cast<BreakNode*>(stmt));
            break;
        case ASTNode::NODE_CONTINUE:
            generateContinue(static_cast<ContinueNode*>(stmt));
            break;
        default:
            break;
    }
}

void CodeGenerator::generateExpression(ExpressionNode* expr) {
    if (!expr) return;
    
    switch (expr->getType()) {
        case ASTNode::NODE_LITERAL: {
            LiteralNode* lit = static_cast<LiteralNode*>(expr);
            output << lit->getValue();
            break;
        }
        case ASTNode::NODE_IDENTIFIER: {
            IdentifierNode* id = static_cast<IdentifierNode*>(expr);
            output << id->getName();
            break;
        }
        case ASTNode::NODE_BINARY_OP: {
            BinaryOpNode* bin = static_cast<BinaryOpNode*>(expr);
            output << "(";
            generateExpression(bin->getLeft());
            output << " " << bin->getOp() << " ";
            generateExpression(bin->getRight());
            output << ")";
            break;
        }
        case ASTNode::NODE_UNARY_OP: {
            UnaryOpNode* un = static_cast<UnaryOpNode*>(expr);
            output << un->getOp();
            generateExpression(un->getOperand());
            break;
        }
        case ASTNode::NODE_CALL:
            generateCall(static_cast<CallNode*>(expr));
            break;
        default:
            output << "/* unknown expression */";
            break;
    }
}

void CodeGenerator::generateFunction(FunctionNode* func) {
    if (!func) return;
    
    indent();
    output << func->getReturnType() << " " << func->getName() << "(";
    
    const auto& params = func->getParams();
    for (size_t i = 0; i < params.size(); i++) {
        output << params[i].first << " " << params[i].second;
        if (i < params.size() - 1) {
            output << ", ";
        }
    }
    
    output << ")";
    newline();
    
    if (func->getBody()) {
        generateBlock(func->getBody());
    } else {
        indent();
        output << "{";
        newline();
        indent();
        output << "}";
        newline();
    }
    newline();
}

void CodeGenerator::generateVarDecl(VarDeclNode* decl) {
    if (!decl) return;
    
    indent();
    output << decl->getVarType() << " " << decl->getName();
    
    if (decl->getInitializer()) {
        output << " = ";
        generateExpression(decl->getInitializer());
    }
    
    output << ";";
    newline();
}

void CodeGenerator::generateAssign(AssignNode* assign) {
    if (!assign) return;
    
    indent();
    output << assign->getName() << " = ";
    generateExpression(assign->getValue());
    output << ";";
    newline();
}

void CodeGenerator::generateIf(IfNode* ifNode) {
    if (!ifNode) return;
    
    indent();
    output << "if (";
    generateExpression(ifNode->getCondition());
    output << ")";
    newline();
    
    if (ifNode->getThenBlock()) {
        generateBlock(ifNode->getThenBlock());
    } else {
        indent();
        output << "{";
        newline();
        indent();
        output << "}";
        newline();
    }
    
    if (ifNode->getElseBlock()) {
        indent();
        output << "else";
        newline();
        generateBlock(ifNode->getElseBlock());
    }
}

void CodeGenerator::generateWhile(WhileNode* whileNode) {
    if (!whileNode) return;
    
    indent();
    output << "while (";
    generateExpression(whileNode->getCondition());
    output << ")";
    newline();
    
    if (whileNode->getBody()) {
        generateBlock(whileNode->getBody());
    } else {
        indent();
        output << "{";
        newline();
        indent();
        output << "}";
        newline();
    }
}

void CodeGenerator::generateFor(ForNode* forNode) {
    if (!forNode) return;
    
    indent();
    output << "for (";
    
    if (forNode->getInit()) {
        // Generate init statement without semicolon
        VarDeclNode* varDecl = dynamic_cast<VarDeclNode*>(forNode->getInit());
        if (varDecl) {
            output << varDecl->getVarType() << " " << varDecl->getName();
            if (varDecl->getInitializer()) {
                output << " = ";
                generateExpression(varDecl->getInitializer());
            }
        } else {
            // For other statement types, we'd need to handle them differently
            output << ";";
        }
    } else {
        output << ";";
    }
    
    output << " ";
    if (forNode->getCondition()) {
        generateExpression(forNode->getCondition());
    }
    output << "; ";
    
    if (forNode->getIncrement()) {
        generateExpression(forNode->getIncrement());
    }
    
    output << ")";
    newline();
    
    if (forNode->getBody()) {
        generateBlock(forNode->getBody());
    } else {
        indent();
        output << "{";
        newline();
        indent();
        output << "}";
        newline();
    }
}

void CodeGenerator::generateBlock(BlockNode* block) {
    if (!block) return;
    
    indent();
    output << "{";
    newline();
    indentLevel++;
    
    for (auto& stmt : block->getStatements()) {
        generateStatement(stmt.get());
    }
    
    indentLevel--;
    indent();
    output << "}";
    newline();
}

void CodeGenerator::generateReturn(ReturnNode* returnNode) {
    if (!returnNode) return;
    
    indent();
    output << "return";
    if (returnNode->getValue()) {
        output << " ";
        generateExpression(returnNode->getValue());
    }
    output << ";";
    newline();
}

void CodeGenerator::generateBreak(BreakNode* breakNode) {
    if (!breakNode) return;
    
    indent();
    output << "break;";
    newline();
}

void CodeGenerator::generateContinue(ContinueNode* continueNode) {
    if (!continueNode) return;
    
    indent();
    output << "continue;";
    newline();
}

void CodeGenerator::generateCall(CallNode* call) {
    if (!call) return;
    
    output << call->getName() << "(";
    
    const auto& args = call->getArgs();
    for (size_t i = 0; i < args.size(); i++) {
        generateExpression(args[i].get());
        if (i < args.size() - 1) {
            output << ", ";
        }
    }
    
    output << ")";
}

