#include "lexer.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename> [--pairs|--colored]" << std::endl;
        std::cerr << "  --pairs    : Output token pairs <lexeme, token_type>" << std::endl;
        std::cerr << "  --colored  : Output colored syntax highlighting (default)" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    bool outputPairs = false;

    // Check for output mode
    if (argc >= 3) {
        std::string mode = argv[2];
        if (mode == "--pairs") {
            outputPairs = true;
        } else if (mode == "--colored") {
            outputPairs = false;
        }
    }

    Lexer lexer(filename);
    std::vector<Token> tokens = lexer.tokenize();

    if (outputPairs) {
        lexer.printTokens();
    } else {
        lexer.printColored();
    }

    return 0;
}

