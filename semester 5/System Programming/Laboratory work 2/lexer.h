#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <fstream>

enum class TokenType {
    // Numbers
    DECIMAL_INTEGER,
    FLOATING_POINT,
    HEXADECIMAL,
    
    // Constants
    STRING_CONSTANT,
    CHARACTER_CONSTANT,
    
    // Preprocessor
    PREPROCESSOR_DIRECTIVE,
    
    // Comments
    COMMENT,
    
    // Reserved words
    RESERVED_WORD,
    
    // Operators
    OPERATOR,
    
    // Delimiters
    DELIMITER,
    
    // Identifiers
    IDENTIFIER,
    
    // Other
    UNRECOGNIZED,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    size_t position;  // Character position in source string
    
    Token(TokenType t, const std::string& v, size_t l, size_t c, size_t p = 0)
        : type(t), value(v), line(l), column(c), position(p) {}
};

class Lexer {
private:
    std::string source;
    size_t position;
    size_t line;
    size_t column;
    std::vector<Token> tokens;
    
    // Helper methods
    bool isWhitespace(char c);
    bool isDigit(char c);
    bool isHexDigit(char c);
    bool isLetter(char c);
    bool isIdentifierChar(char c);
    char peek();
    char advance();
    void skipWhitespace();
    
    // Token recognition methods
    Token scanNumber();
    Token scanString();
    Token scanComment();
    Token scanPreprocessor();
    Token scanIdentifierOrKeyword();
    Token scanOperator();
    Token scanDelimiter();
    
    // Reserved words check
    bool isReservedWord(const std::string& word);
    
public:
    Lexer(const std::string& filename);
    std::vector<Token> tokenize();
    void printTokens();
    void printColored();
};

#endif // LEXER_H

