# Makefile для лабораторної роботи 5: Природній кубічний сплайн

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = spline
SOURCE = main.cpp

# Правило за замовчуванням
all: $(TARGET)

# Компіляція програми
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Очищення згенерованих файлів
clean:
	rm -f $(TARGET) graph_data.txt

# Запуск програми
run: $(TARGET)
	./$(TARGET)

# Перекомпіляція
rebuild: clean all

.PHONY: all clean run rebuild

