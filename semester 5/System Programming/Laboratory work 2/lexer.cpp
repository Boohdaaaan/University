#include "lexer.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>

// ANSI color codes
#define COLOR_RESET "\033[0m"
#define COLOR_NUMBER "\033[33m"      // Yellow
#define COLOR_STRING "\033[32m"      // Green
#define COLOR_COMMENT "\033[36m"      // Cyan
#define COLOR_KEYWORD "\033[35m"      // Magenta
#define COLOR_OPERATOR "\033[31m"     // Red
#define COLOR_DELIMITER "\033[37m"    // White
#define COLOR_IDENTIFIER "\033[0m"    // Default
#define COLOR_PREPROCESSOR "\033[34m" // Blue
#define COLOR_ERROR "\033[91m"         // Bright Red

Lexer::Lexer(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        source = "";
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    source = buffer.str();
    file.close();
    
    position = 0;
    line = 1;
    column = 1;
}

bool Lexer::isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r';
}

bool Lexer::isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool Lexer::isHexDigit(char c) {
    return std::isxdigit(static_cast<unsigned char>(c));
}

bool Lexer::isLetter(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Lexer::isIdentifierChar(char c) {
    return isLetter(c) || isDigit(c) || c == '_';
}

char Lexer::peek() {
    if (position >= source.length()) {
        return '\0';
    }
    return source[position];
}

char Lexer::advance() {
    if (position >= source.length()) {
        return '\0';
    }
    
    char c = source[position++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

void Lexer::skipWhitespace() {
    while (position < source.length() && isWhitespace(source[position])) {
        advance();
    }
}

Token Lexer::scanNumber() {
    size_t startLine = line;
    size_t startCol = column;
    size_t startPos = position;
    std::string value;
    
    // Check for hexadecimal
    if (peek() == '&' && position + 1 < source.length()) {
        char next = source[position + 1];
        if (next == 'H' || next == 'h') {
            advance(); // consume '&'
            advance(); // consume 'H' or 'h'
            value = "&H";
            
            while (position < source.length() && isHexDigit(peek())) {
                value += advance();
            }
            
            // Check for type suffix
            if (peek() == '&' || peek() == '!' || peek() == '#') {
                value += advance();
            }
            
            return Token(TokenType::HEXADECIMAL, value, startLine, startCol, startPos);
        }
    }
    
    // Decimal integer or floating point
    while (position < source.length() && isDigit(peek())) {
        value += advance();
    }
    
    // Check for floating point
    if (peek() == '.' && position + 1 < source.length() && isDigit(source[position + 1])) {
        value += advance(); // consume '.'
        while (position < source.length() && isDigit(peek())) {
            value += advance();
        }
        
        // Check for exponent
        if (peek() == 'E' || peek() == 'e') {
            value += advance();
            if (peek() == '+' || peek() == '-') {
                value += advance();
            }
            while (position < source.length() && isDigit(peek())) {
                value += advance();
            }
        }
        
        // Type suffix
        if (peek() == '!' || peek() == '#' || peek() == '@') {
            value += advance();
        }
        
        return Token(TokenType::FLOATING_POINT, value, startLine, startCol, startPos);
    }
    
    // Type suffix for integer
    if (peek() == '&' || peek() == '!' || peek() == '#' || peek() == '@' || peek() == '%') {
        value += advance();
    }
    
    return Token(TokenType::DECIMAL_INTEGER, value, startLine, startCol, startPos);
}

Token Lexer::scanString() {
    size_t startLine = line;
    size_t startCol = column;
    size_t startPos = position;
    std::string value;
    
    char quote = advance(); // consume opening quote
    value += quote;
    
    bool isChar = false;
    
    while (position < source.length()) {
        char c = peek();
        if (c == '\0') {
            break;
        }
        
        if (c == quote) {
            value += advance();
            // Check for character constant suffix
            if (peek() == 'C' || peek() == 'c') {
                value += advance();
                isChar = true;
            }
            break;
        }
        
        if (c == '\n') {
            // Unterminated string
            break;
        }
        
        // Handle escape sequences
        if (c == '\\') {
            value += advance();
            if (position < source.length()) {
                value += advance();
            }
        } else {
            value += advance();
        }
    }
    
    TokenType type = isChar ? TokenType::CHARACTER_CONSTANT : TokenType::STRING_CONSTANT;
    return Token(type, value, startLine, startCol, startPos);
}

Token Lexer::scanComment() {
    size_t startLine = line;
    size_t startCol = column;
    size_t startPos = position;
    std::string value;
    
    // Check for REM keyword
    if (position + 2 < source.length() && 
        (source[position] == 'R' || source[position] == 'r') &&
        (source[position + 1] == 'E' || source[position + 1] == 'e') &&
        (source[position + 2] == 'M' || source[position + 2] == 'm') &&
        (position + 3 >= source.length() || isWhitespace(source[position + 3]) || source[position + 3] == '\n')) {
        value += advance(); // R
        value += advance(); // E
        value += advance(); // M
        skipWhitespace();
    } else if (peek() == '\'') {
        value += advance(); // consume '
    }
    
    // Read until end of line
    while (position < source.length() && peek() != '\n' && peek() != '\0') {
        value += advance();
    }
    
    return Token(TokenType::COMMENT, value, startLine, startCol, startPos);
}

Token Lexer::scanPreprocessor() {
    size_t startLine = line;
    size_t startCol = column;
    size_t startPos = position;
    std::string value;
    
    value += advance(); // consume '#'
    
    // Read directive name
    while (position < source.length() && isLetter(peek())) {
        value += advance();
    }
    
    // Read rest of the line (including whitespace)
    while (position < source.length() && peek() != '\n' && peek() != '\0') {
        value += advance();
    }
    
    return Token(TokenType::PREPROCESSOR_DIRECTIVE, value, startLine, startCol, startPos);
}

bool Lexer::isReservedWord(const std::string& word) {
    // Visual Basic reserved words (case-insensitive)
    static const std::vector<std::string> reservedWords = {
        "If", "Then", "Else", "ElseIf", "End", "Sub", "Function", "Dim", "As",
        "Integer", "String", "Boolean", "Double", "Single", "Long", "Short",
        "Byte", "Char", "Date", "Object", "Variant", "Void",
        "For", "Next", "While", "Wend", "Do", "Loop", "Until", "Step",
        "Select", "Case", "Is", "To",
        "Public", "Private", "Protected", "Friend", "Static",
        "Class", "Module", "Namespace", "Imports", "Inherits",
        "Return", "Exit", "Continue", "GoTo", "On", "Error", "Resume",
        "Try", "Catch", "Finally", "Throw",
        "And", "Or", "Not", "Xor", "Mod",
        "True", "False", "Nothing", "Me", "MyBase", "MyClass",
        "ByVal", "ByRef", "Optional", "ParamArray",
        "Const", "ReadOnly", "WriteOnly", "Property", "Get", "Set",
        "AddHandler", "RemoveHandler", "RaiseEvent", "Event",
        "With", "End", "Structure", "Enum", "Interface",
        "Inherits", "Implements", "Overloads", "Overrides", "Overridable",
        "MustOverride", "NotInheritable", "NotOverridable", "Shadows",
        "Shared", "MustInherit", "Partial", "Async", "Await",
        "Let", "Set", "Call", "New", "TypeOf", "IsNot",
        "Option", "Explicit", "Strict", "Compare", "On", "Off",
        "ReDim", "Preserve", "Erase", "LBound", "UBound",
        "On", "Error", "Resume", "Next", "GoSub", "Return",
        "Stop", "End", "ChDir", "ChDrive", "Close", "Reset",
        "Lock", "Unlock", "Open", "Input", "Output", "Append",
        "Binary", "Random", "Access", "Read", "Write", "ReadWrite",
        "Seek", "Loc", "LOF", "EOF", "FreeFile", "FileAttr",
        "Get", "Put", "Width", "Print", "PrintLine", "Write", "WriteLine",
        "Input", "InputString", "LineInput", "Lock", "Unlock",
        "Name", "Kill", "MkDir", "RmDir", "FileCopy", "FileDateTime",
        "FileLen", "GetAttr", "SetAttr", "Dir", "CurDir", "ChDir", "ChDrive"
    };
    
    std::string upperWord = word;
    std::transform(upperWord.begin(), upperWord.end(), upperWord.begin(), ::toupper);
    
    for (const auto& reserved : reservedWords) {
        std::string upperReserved = reserved;
        std::transform(upperReserved.begin(), upperReserved.end(), upperReserved.begin(), ::toupper);
        if (upperWord == upperReserved) {
            return true;
        }
    }
    
    return false;
}

Token Lexer::scanIdentifierOrKeyword() {
    size_t startLine = line;
    size_t startCol = column;
    size_t startPos = position;
    std::string value;
    
    while (position < source.length() && isIdentifierChar(peek())) {
        value += advance();
    }
    
    TokenType type = isReservedWord(value) ? TokenType::RESERVED_WORD : TokenType::IDENTIFIER;
    return Token(type, value, startLine, startCol, startPos);
}

Token Lexer::scanOperator() {
    size_t startLine = line;
    size_t startCol = column;
    size_t startPos = position;
    std::string value;
    
    char c = peek();
    
    // Multi-character operators
    if (c == '<') {
        value += advance();
        if (peek() == '=') {
            value += advance(); // <=
        } else if (peek() == '>') {
            value += advance(); // <>
        } else {
            // <
        }
    } else if (c == '>') {
        value += advance();
        if (peek() == '=') {
            value += advance(); // >=
        }
    } else if (c == '=') {
        value += advance();
    } else if (c == '+') {
        value += advance();
    } else if (c == '-') {
        value += advance();
    } else if (c == '*') {
        value += advance();
    } else if (c == '/') {
        value += advance();
        if (peek() == '=') {
            value += advance(); // /=
        }
    } else if (c == '\\') {
        value += advance(); // Integer division
    } else if (c == '^') {
        value += advance(); // Exponentiation
    } else if (c == '&') {
        value += advance(); // Concatenation
    } else {
        value += advance();
    }
    
    return Token(TokenType::OPERATOR, value, startLine, startCol, startPos);
}

Token Lexer::scanDelimiter() {
    size_t startLine = line;
    size_t startCol = column;
    size_t startPos = position;
    std::string value;
    
    value += advance();
    
    return Token(TokenType::DELIMITER, value, startLine, startCol, startPos);
}

std::vector<Token> Lexer::tokenize() {
    tokens.clear();
    position = 0;
    line = 1;
    column = 1;
    
    while (position < source.length()) {
        skipWhitespace();
        
        if (position >= source.length()) {
            break;
        }
        
        char c = peek();
        
        if (c == '\0') {
            break;
        }
        
        if (c == '\n') {
            advance();
            continue;
        }
        
        // Numbers
        if (isDigit(c) || (c == '&' && position + 1 < source.length() && 
            (source[position + 1] == 'H' || source[position + 1] == 'h'))) {
            tokens.push_back(scanNumber());
        }
        // Strings
        else if (c == '"') {
            tokens.push_back(scanString());
        }
        // Comments
        else if (c == '\'' || (position + 2 < source.length() && 
                 (c == 'R' || c == 'r') && 
                 (source[position + 1] == 'E' || source[position + 1] == 'e') &&
                 (source[position + 2] == 'M' || source[position + 2] == 'm') &&
                 (position + 3 >= source.length() || isWhitespace(source[position + 3]) || source[position + 3] == '\n'))) {
            tokens.push_back(scanComment());
        }
        // Preprocessor
        else if (c == '#') {
            tokens.push_back(scanPreprocessor());
        }
        // Identifiers and keywords
        else if (isLetter(c)) {
            tokens.push_back(scanIdentifierOrKeyword());
        }
        // Operators
        else if (c == '<' || c == '>' || c == '=' || c == '+' || c == '-' || 
                 c == '*' || c == '/' || c == '\\' || c == '^' || c == '&') {
            tokens.push_back(scanOperator());
        }
        // Delimiters
        else if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || 
                 c == '}' || c == ',' || c == ';' || c == ':' || c == '.') {
            tokens.push_back(scanDelimiter());
        }
        // Unrecognized
        else {
            size_t startLine = line;
            size_t startCol = column;
            size_t startPos = position;
            std::string value;
            value += advance();
            tokens.push_back(Token(TokenType::UNRECOGNIZED, value, startLine, startCol, startPos));
        }
    }
    
    tokens.push_back(Token(TokenType::END_OF_FILE, "", line, column, position));
    return tokens;
}

void Lexer::printTokens() {
    for (const auto& token : tokens) {
        std::string typeStr;
        switch (token.type) {
            case TokenType::DECIMAL_INTEGER:
                typeStr = "DECIMAL_INTEGER";
                break;
            case TokenType::FLOATING_POINT:
                typeStr = "FLOATING_POINT";
                break;
            case TokenType::HEXADECIMAL:
                typeStr = "HEXADECIMAL";
                break;
            case TokenType::STRING_CONSTANT:
                typeStr = "STRING_CONSTANT";
                break;
            case TokenType::CHARACTER_CONSTANT:
                typeStr = "CHARACTER_CONSTANT";
                break;
            case TokenType::PREPROCESSOR_DIRECTIVE:
                typeStr = "PREPROCESSOR_DIRECTIVE";
                break;
            case TokenType::COMMENT:
                typeStr = "COMMENT";
                break;
            case TokenType::RESERVED_WORD:
                typeStr = "RESERVED_WORD";
                break;
            case TokenType::OPERATOR:
                typeStr = "OPERATOR";
                break;
            case TokenType::DELIMITER:
                typeStr = "DELIMITER";
                break;
            case TokenType::IDENTIFIER:
                typeStr = "IDENTIFIER";
                break;
            case TokenType::UNRECOGNIZED:
                typeStr = "UNRECOGNIZED";
                break;
            case TokenType::END_OF_FILE:
                typeStr = "END_OF_FILE";
                break;
        }
        
        std::cout << "<" << token.value << ", " << typeStr << ">";
        if (token.type == TokenType::UNRECOGNIZED) {
            std::cout << " [ERROR at line " << token.line << ", column " << token.column << "]";
        }
        std::cout << std::endl;
    }
}

void Lexer::printColored() {
    // Sort tokens by position for easier processing
    std::vector<Token> sortedTokens = tokens;
    std::sort(sortedTokens.begin(), sortedTokens.end(), 
              [](const Token& a, const Token& b) { return a.position < b.position; });
    
    size_t sourcePos = 0;
    size_t tokenIndex = 0;
    
    while (sourcePos < source.length() && tokenIndex < sortedTokens.size()) {
        const Token& token = sortedTokens[tokenIndex];
        
        // Skip END_OF_FILE token
        if (token.type == TokenType::END_OF_FILE) {
            tokenIndex++;
            continue;
        }
        
        // Print everything before this token
        while (sourcePos < token.position && sourcePos < source.length()) {
            std::cout << source[sourcePos++];
        }
        
        // Print colored token
        std::string color;
        switch (token.type) {
            case TokenType::DECIMAL_INTEGER:
            case TokenType::FLOATING_POINT:
            case TokenType::HEXADECIMAL:
                color = COLOR_NUMBER;
                break;
            case TokenType::STRING_CONSTANT:
            case TokenType::CHARACTER_CONSTANT:
                color = COLOR_STRING;
                break;
            case TokenType::COMMENT:
                color = COLOR_COMMENT;
                break;
            case TokenType::RESERVED_WORD:
                color = COLOR_KEYWORD;
                break;
            case TokenType::OPERATOR:
                color = COLOR_OPERATOR;
                break;
            case TokenType::DELIMITER:
                color = COLOR_DELIMITER;
                break;
            case TokenType::PREPROCESSOR_DIRECTIVE:
                color = COLOR_PREPROCESSOR;
                break;
            case TokenType::IDENTIFIER:
                color = COLOR_IDENTIFIER;
                break;
            case TokenType::UNRECOGNIZED:
                color = COLOR_ERROR;
                break;
            default:
                color = COLOR_RESET;
        }
        
        std::cout << color << token.value << COLOR_RESET;
        sourcePos += token.value.length();
        tokenIndex++;
    }
    
    // Print any remaining source
    while (sourcePos < source.length()) {
        std::cout << source[sourcePos++];
    }
    std::cout << std::endl;
}

