CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
FLEX = flex
BISON = /opt/homebrew/Cellar/bison/3.8.2/bin/bison

TARGET = c_parser
LEXER_SRC = lexer.l
PARSER_SRC = parser.y
LEXER_OUT = lex.yy.c
PARSER_OUT = parser.tab.cc parser.tab.hh location.hh position.hh stack.hh

SOURCES = main.cpp ast.cpp semantic.cpp codegen.cpp
OBJECTS = $(SOURCES:.cpp=.o) $(LEXER_OUT:.c=.o) $(PARSER_OUT:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(filter %.o,$(OBJECTS))

# Generate parser files first
parser.tab.cc parser.tab.hh: $(PARSER_SRC)
	$(BISON) -d $(PARSER_SRC)

# Generate lexer after parser (needs parser.tab.hh)
$(LEXER_OUT): $(LEXER_SRC) parser.tab.hh
	$(FLEX) $(LEXER_SRC)

lex.yy.o: lex.yy.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main.cpp only after parser.tab.hh exists
main.o: main.cpp parser.tab.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(OBJECTS) $(TARGET) $(LEXER_OUT) $(PARSER_OUT) *.output

.PHONY: all clean

