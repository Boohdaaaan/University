#include "ast.h"
#include <sstream>
#include <iomanip>

std::vector<std::unique_ptr<StatementNode>> g_ast;

std::string indentString(int level) {
    return std::string(level * 2, ' ');
}

std::string BinaryOpNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"BinaryOp\",\n";
    oss << indentStr << "  \"operator\": \"" << op << "\",\n";
    oss << indentStr << "  \"left\": " << (left ? left->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"right\": " << (right ? right->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string UnaryOpNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"UnaryOp\",\n";
    oss << indentStr << "  \"operator\": \"" << op << "\",\n";
    oss << indentStr << "  \"operand\": " << (operand ? operand->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string LiteralNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Literal\",\n";
    oss << indentStr << "  \"value\": \"" << value << "\",\n";
    oss << indentStr << "  \"literalType\": \"" << literalType << "\",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string IdentifierNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Identifier\",\n";
    oss << indentStr << "  \"name\": \"" << name << "\",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string IfNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"If\",\n";
    oss << indentStr << "  \"condition\": " << (condition ? condition->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"thenBlock\": " << (thenBlock ? thenBlock->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"elseBlock\": " << (elseBlock ? elseBlock->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string WhileNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"While\",\n";
    oss << indentStr << "  \"condition\": " << (condition ? condition->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"body\": " << (body ? body->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string ForNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"For\",\n";
    oss << indentStr << "  \"init\": " << (init ? init->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"condition\": " << (condition ? condition->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"increment\": " << (increment ? increment->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"body\": " << (body ? body->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string FunctionNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Function\",\n";
    oss << indentStr << "  \"name\": \"" << name << "\",\n";
    oss << indentStr << "  \"returnType\": \"" << returnType << "\",\n";
    oss << indentStr << "  \"params\": [\n";
    for (size_t i = 0; i < params.size(); i++) {
        oss << indentStr << "    {\"type\": \"" << params[i].first << "\", \"name\": \"" << params[i].second << "\"}";
        if (i < params.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << indentStr << "  ],\n";
    oss << indentStr << "  \"body\": " << (body ? body->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string CallNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Call\",\n";
    oss << indentStr << "  \"name\": \"" << name << "\",\n";
    oss << indentStr << "  \"args\": [\n";
    for (size_t i = 0; i < args.size(); i++) {
        oss << args[i]->toJSON(indent + 2);
        if (i < args.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << indentStr << "  ],\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string VarDeclNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"VarDecl\",\n";
    oss << indentStr << "  \"varType\": \"" << varType << "\",\n";
    oss << indentStr << "  \"name\": \"" << name << "\",\n";
    oss << indentStr << "  \"initializer\": " << (initializer ? initializer->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string AssignNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Assign\",\n";
    oss << indentStr << "  \"name\": \"" << name << "\",\n";
    oss << indentStr << "  \"value\": " << (value ? value->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string BlockNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Block\",\n";
    oss << indentStr << "  \"statements\": [\n";
    for (size_t i = 0; i < statements.size(); i++) {
        oss << statements[i]->toJSON(indent + 2);
        if (i < statements.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << indentStr << "  ],\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string ReturnNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Return\",\n";
    oss << indentStr << "  \"value\": " << (value ? value->toJSON(indent + 1) : "null") << ",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string BreakNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Break\",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

std::string ContinueNode::toJSON(int indent) const {
    std::ostringstream oss;
    std::string indentStr = indentString(indent);
    oss << indentStr << "{\n";
    oss << indentStr << "  \"type\": \"Continue\",\n";
    oss << indentStr << "  \"line\": " << lineNumber << "\n";
    oss << indentStr << "}";
    return oss.str();
}

