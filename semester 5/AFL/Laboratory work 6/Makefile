CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra

LLVM_PREFIX := $(shell brew --prefix llvm 2>/dev/null || echo "")
CLANG_LIB = $(LLVM_PREFIX)/lib/libclang.dylib
CLANG_INCLUDE = $(LLVM_PREFIX)/include

INCLUDES = -I$(CLANG_INCLUDE)
LIBS = $(CLANG_LIB)

TARGET = lab6
SOURCE = main.cpp

all: $(TARGET)
	@echo ""
	@echo "Збірка завершена! Запустіть: ./$(TARGET)"

$(TARGET): $(SOURCE)
	@echo "Збірка проєкту..."
	@if [ ! -f "$(CLANG_INCLUDE)/clang-c/Index.h" ]; then \
		echo "ПОМИЛКА"; \
		exit 1; \
	fi
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCE) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

