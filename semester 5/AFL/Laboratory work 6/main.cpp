#include <clang-c/Index.h>
#include <iostream>
#include <string>
#include <cstring>

int lambda_count = 0;

bool hasReferenceCapture(CXCursor lambda_cursor) {
    CXTranslationUnit tu = clang_Cursor_getTranslationUnit(lambda_cursor);
    CXSourceRange range = clang_getCursorExtent(lambda_cursor);
    
    CXToken* tokens = nullptr;
    unsigned num_tokens = 0;
    clang_tokenize(tu, range, &tokens, &num_tokens);
    
    bool found_ref_capture = false;
    bool in_capture_list = false;
    
    for (unsigned i = 0; i < num_tokens; ++i) {
        CXString token_spelling = clang_getTokenSpelling(tu, tokens[i]);
        const char* spelling = clang_getCString(token_spelling);
        
        if (!spelling) {
            clang_disposeString(token_spelling);
            continue;
        }
        
        if (strcmp(spelling, "[") == 0) {
            in_capture_list = true;
            if (i + 1 < num_tokens) {
                CXString next_spelling = clang_getTokenSpelling(tu, tokens[i + 1]);
                const char* next = clang_getCString(next_spelling);
                if (next && strcmp(next, "&") == 0) {
                    found_ref_capture = true;
                }
                clang_disposeString(next_spelling);
            }
        } else if (strcmp(spelling, "]") == 0 && in_capture_list) {
            in_capture_list = false;
            if (found_ref_capture) {
                clang_disposeString(token_spelling);
                break;
            }
        }
        
        if (in_capture_list && strcmp(spelling, "&") == 0) {
            found_ref_capture = true;
        }
        
        clang_disposeString(token_spelling);
    }
    
    if (tokens) {
        clang_disposeTokens(tu, tokens, num_tokens);
    }
    
    return found_ref_capture;
}

enum CXChildVisitResult visitLambda(CXCursor cursor, CXCursor /* parent */, CXClientData /* client_data */) {
    if (clang_getCursorKind(cursor) == CXCursor_LambdaExpr) {
        if (hasReferenceCapture(cursor)) {
            lambda_count++;
        }
    }
    
    return CXChildVisit_Recurse;
}

int main() {
    const char* file_path = "test_file.cpp";
    
    CXIndex index = clang_createIndex(0, 0);
    
    const char* args[] = {
        "-std=c++11",
    };
    
    CXTranslationUnit tu = clang_parseTranslationUnit(
        index,
        file_path,
        args,
        sizeof(args) / sizeof(args[0]),
        nullptr,
        0,
        CXTranslationUnit_None
    );
    
    if (!tu) {
        std::cerr << "Помилка: не вдалося розпарсити файл " << file_path << std::endl;
        clang_disposeIndex(index);
        return 1;
    }
    
    CXCursor root_cursor = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(root_cursor, visitLambda, nullptr);
    
    std::cout << "Кількість анонімних функцій, що захоплюють контекст за посиланням: " 
              << lambda_count << std::endl;
    
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
    
    return 0;
}

