#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ast.h"
#include "semantic.h"
#include "codegen.h"
#include "parser.tab.hh"

extern std::vector<std::unique_ptr<StatementNode>> g_ast;
extern FILE* yyin;
int yylex(yy::parser::semantic_type* yylval, yy::parser::location_type* yylloc);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [options]" << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  --json <output.json>    Export AST to JSON" << std::endl;
        std::cerr << "  --code <output.c>      Generate C code" << std::endl;
        std::cerr << "  --semantic              Run semantic analysis" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string jsonFile;
    std::string codeFile;
    bool runSemantic = false;

    // Parse arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--json" && i + 1 < argc) {
            jsonFile = argv[++i];
        } else if (arg == "--code" && i + 1 < argc) {
            codeFile = argv[++i];
        } else if (arg == "--semantic") {
            runSemantic = true;
        }
    }

    // Open input file
    FILE* file = fopen(inputFile.c_str(), "r");
    if (!file) {
        std::cerr << "Error: Cannot open file " << inputFile << std::endl;
        return 1;
    }

    yyin = file;
    g_ast.clear();

    std::cout << "Parsing " << inputFile << "..." << std::endl;
    
    yy::parser parser;
    int result = parser.parse();
    fclose(file);

    if (result != 0) {
        std::cerr << "Parse error!" << std::endl;
        return 1;
    }

    std::cout << "Parse successful! Found " << g_ast.size() << " top-level statements." << std::endl;

    // Semantic analysis
    if (runSemantic) {
        std::cout << "\nRunning semantic analysis..." << std::endl;
        SemanticAnalyzer analyzer;
        bool success = analyzer.analyze(g_ast);
        
        if (!success || !analyzer.getErrors().empty()) {
            std::cerr << "Semantic errors:" << std::endl;
            std::cerr << analyzer.getErrors() << std::endl;
        } else {
            std::cout << "Semantic analysis passed!" << std::endl;
        }
    }

    // Export AST to JSON
    if (!jsonFile.empty()) {
        std::cout << "\nExporting AST to " << jsonFile << "..." << std::endl;
        std::ofstream jsonOut(jsonFile);
        
        if (jsonOut.is_open()) {
            jsonOut << "[" << std::endl;
            for (size_t i = 0; i < g_ast.size(); i++) {
                jsonOut << g_ast[i]->toJSON(1);
                if (i < g_ast.size() - 1) {
                    jsonOut << ",";
                }
                jsonOut << std::endl;
            }
            jsonOut << "]" << std::endl;
            jsonOut.close();
            std::cout << "AST exported successfully!" << std::endl;
        } else {
            std::cerr << "Error: Cannot write to " << jsonFile << std::endl;
        }
    }

    // Generate code
    if (!codeFile.empty()) {
        std::cout << "\nGenerating C code to " << codeFile << "..." << std::endl;
        CodeGenerator generator;
        std::string code = generator.generate(g_ast);
        
        std::ofstream codeOut(codeFile);
        if (codeOut.is_open()) {
            codeOut << code;
            codeOut.close();
            std::cout << "Code generated successfully!" << std::endl;
        } else {
            std::cerr << "Error: Cannot write to " << codeFile << std::endl;
        }
    }

    return 0;
}

