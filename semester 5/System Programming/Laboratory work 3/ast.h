#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <cstdint>

// Forward declarations
class ASTNode;
class ExpressionNode;
class StatementNode;
class BlockNode;

// Base AST node class
class ASTNode {
public:
    enum NodeType {
        NODE_EXPRESSION,
        NODE_STATEMENT,
        NODE_BINARY_OP,
        NODE_UNARY_OP,
        NODE_LITERAL,
        NODE_IDENTIFIER,
        NODE_IF,
        NODE_WHILE,
        NODE_FOR,
        NODE_FUNCTION,
        NODE_CALL,
        NODE_VAR_DECL,
        NODE_ASSIGN,
        NODE_BLOCK,
        NODE_RETURN,
        NODE_BREAK,
        NODE_CONTINUE
    };

    ASTNode(NodeType type, int line = 0) : nodeType(type), lineNumber(line) {}
    virtual ~ASTNode() = default;
    
    NodeType getType() const { return nodeType; }
    int getLine() const { return lineNumber; }
    
    virtual std::string toJSON(int indent = 0) const = 0;

protected:
    NodeType nodeType;
    int lineNumber;
};

// Expression base class
class ExpressionNode : public ASTNode {
public:
    ExpressionNode(NodeType type, int line = 0) : ASTNode(type, line) {}
};

// Binary operation node
class BinaryOpNode : public ExpressionNode {
public:
    BinaryOpNode(const std::string& op, std::unique_ptr<ExpressionNode> left,
                 std::unique_ptr<ExpressionNode> right, int line = 0)
        : ExpressionNode(NODE_BINARY_OP, line), op(op),
          left(std::move(left)), right(std::move(right)) {}
    
    std::string toJSON(int indent = 0) const override;
    const std::string& getOp() const { return op; }
    ExpressionNode* getLeft() const { return left.get(); }
    ExpressionNode* getRight() const { return right.get(); }

private:
    std::string op;
    std::unique_ptr<ExpressionNode> left;
    std::unique_ptr<ExpressionNode> right;
};

// Unary operation node
class UnaryOpNode : public ExpressionNode {
public:
    UnaryOpNode(const std::string& op, std::unique_ptr<ExpressionNode> operand, int line = 0)
        : ExpressionNode(NODE_UNARY_OP, line), op(op), operand(std::move(operand)) {}
    
    std::string toJSON(int indent = 0) const override;
    const std::string& getOp() const { return op; }
    ExpressionNode* getOperand() const { return operand.get(); }

private:
    std::string op;
    std::unique_ptr<ExpressionNode> operand;
};

// Literal node
class LiteralNode : public ExpressionNode {
public:
    LiteralNode(const std::string& value, const std::string& type, int line = 0)
        : ExpressionNode(NODE_LITERAL, line), value(value), literalType(type) {}
    
    std::string toJSON(int indent = 0) const override;
    const std::string& getValue() const { return value; }
    const std::string& getLiteralType() const { return literalType; }

private:
    std::string value;
    std::string literalType;
};

// Identifier node
class IdentifierNode : public ExpressionNode {
public:
    IdentifierNode(const std::string& name, int line = 0)
        : ExpressionNode(NODE_IDENTIFIER, line), name(name) {}
    
    std::string toJSON(int indent = 0) const override;
    const std::string& getName() const { return name; }

private:
    std::string name;
};

// Statement base class
class StatementNode : public ASTNode {
public:
    StatementNode(NodeType type, int line = 0) : ASTNode(type, line) {}
};

// If statement node
class IfNode : public StatementNode {
public:
    IfNode(std::unique_ptr<ExpressionNode> condition,
           std::unique_ptr<BlockNode> thenBlock,
           std::unique_ptr<BlockNode> elseBlock,
           int line = 0)
        : StatementNode(NODE_IF, line),
          condition(std::move(condition)),
          thenBlock(std::move(thenBlock)),
          elseBlock(std::move(elseBlock)) {}
    
    std::string toJSON(int indent = 0) const override;
    ExpressionNode* getCondition() const { return condition.get(); }
    BlockNode* getThenBlock() const { return thenBlock.get(); }
    BlockNode* getElseBlock() const { return elseBlock.get(); }

private:
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockNode> thenBlock;
    std::unique_ptr<BlockNode> elseBlock;
};

// While loop node
class WhileNode : public StatementNode {
public:
    WhileNode(std::unique_ptr<ExpressionNode> condition,
              std::unique_ptr<BlockNode> body,
              int line = 0)
        : StatementNode(NODE_WHILE, line),
          condition(std::move(condition)),
          body(std::move(body)) {}
    
    std::string toJSON(int indent = 0) const override;
    ExpressionNode* getCondition() const { return condition.get(); }
    BlockNode* getBody() const { return body.get(); }

private:
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockNode> body;
};

// For loop node
class ForNode : public StatementNode {
public:
    ForNode(std::unique_ptr<StatementNode> init,
            std::unique_ptr<ExpressionNode> condition,
            std::unique_ptr<ExpressionNode> increment,
            std::unique_ptr<BlockNode> body,
            int line = 0)
        : StatementNode(NODE_FOR, line),
          init(std::move(init)),
          condition(std::move(condition)),
          increment(std::move(increment)),
          body(std::move(body)) {}
    
    std::string toJSON(int indent = 0) const override;
    StatementNode* getInit() const { return init.get(); }
    ExpressionNode* getCondition() const { return condition.get(); }
    ExpressionNode* getIncrement() const { return increment.get(); }
    BlockNode* getBody() const { return body.get(); }

private:
    std::unique_ptr<StatementNode> init;
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<ExpressionNode> increment;
    std::unique_ptr<BlockNode> body;
};

// Function definition node
class FunctionNode : public StatementNode {
public:
    FunctionNode(const std::string& name,
                 const std::string& returnType,
                 std::vector<std::pair<std::string, std::string>> params,
                 std::unique_ptr<BlockNode> body,
                 int line = 0)
        : StatementNode(NODE_FUNCTION, line),
          name(name),
          returnType(returnType),
          params(std::move(params)),
          body(std::move(body)) {}
    
    std::string toJSON(int indent = 0) const override;
    const std::string& getName() const { return name; }
    const std::string& getReturnType() const { return returnType; }
    const std::vector<std::pair<std::string, std::string>>& getParams() const { return params; }
    BlockNode* getBody() const { return body.get(); }

private:
    std::string name;
    std::string returnType;
    std::vector<std::pair<std::string, std::string>> params;
    std::unique_ptr<BlockNode> body;
};

// Function call node
class CallNode : public ExpressionNode {
public:
    CallNode(const std::string& name,
             std::vector<std::unique_ptr<ExpressionNode>> args,
             int line = 0)
        : ExpressionNode(NODE_CALL, line),
          name(name),
          args(std::move(args)) {}
    
    std::string toJSON(int indent = 0) const override;
    const std::string& getName() const { return name; }
    const std::vector<std::unique_ptr<ExpressionNode>>& getArgs() const { return args; }

private:
    std::string name;
    std::vector<std::unique_ptr<ExpressionNode>> args;
};

// Variable declaration node
class VarDeclNode : public StatementNode {
public:
    VarDeclNode(const std::string& type,
                const std::string& name,
                std::unique_ptr<ExpressionNode> initializer,
                int line = 0)
        : StatementNode(NODE_VAR_DECL, line),
          varType(type),
          name(name),
          initializer(std::move(initializer)) {}
    
    std::string toJSON(int indent = 0) const override;
    const std::string& getVarType() const { return varType; }
    const std::string& getName() const { return name; }
    ExpressionNode* getInitializer() const { return initializer.get(); }

private:
    std::string varType;
    std::string name;
    std::unique_ptr<ExpressionNode> initializer;
};

// Assignment node
class AssignNode : public StatementNode {
public:
    AssignNode(const std::string& name,
               std::unique_ptr<ExpressionNode> value,
               int line = 0)
        : StatementNode(NODE_ASSIGN, line),
          name(name),
          value(std::move(value)) {}
    
    std::string toJSON(int indent = 0) const override;
    const std::string& getName() const { return name; }
    ExpressionNode* getValue() const { return value.get(); }

private:
    std::string name;
    std::unique_ptr<ExpressionNode> value;
};

// Block node
class BlockNode : public StatementNode {
public:
    BlockNode(std::vector<std::unique_ptr<StatementNode>> statements, int line = 0)
        : StatementNode(NODE_BLOCK, line),
          statements(std::move(statements)) {}
    
    std::string toJSON(int indent = 0) const override;
    std::vector<std::unique_ptr<StatementNode>>& getStatements() {
        return statements;
    }
    const std::vector<std::unique_ptr<StatementNode>>& getStatements() const {
        return statements;
    }

private:
    std::vector<std::unique_ptr<StatementNode>> statements;
};

// Return statement node
class ReturnNode : public StatementNode {
public:
    ReturnNode(std::unique_ptr<ExpressionNode> value, int line = 0)
        : StatementNode(NODE_RETURN, line),
          value(std::move(value)) {}
    
    std::string toJSON(int indent = 0) const override;
    ExpressionNode* getValue() const { return value.get(); }

private:
    std::unique_ptr<ExpressionNode> value;
};

// Break statement node
class BreakNode : public StatementNode {
public:
    BreakNode(int line = 0) : StatementNode(NODE_BREAK, line) {}
    std::string toJSON(int indent = 0) const override;
};

// Continue statement node
class ContinueNode : public StatementNode {
public:
    ContinueNode(int line = 0) : StatementNode(NODE_CONTINUE, line) {}
    std::string toJSON(int indent = 0) const override;
};

// Helper function for JSON indentation
std::string indentString(int level);

#endif // AST_H

