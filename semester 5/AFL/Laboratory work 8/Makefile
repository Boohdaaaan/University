CC = gcc
CFLAGS = -Wall -Wextra
FLEX = flex
TARGET = lab8
SOURCE = lab8.l

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	# macOS - спробуємо -ll або без бібліотеки
	FLEX_LIB = -ll
else
	# Linux та інші системи
	FLEX_LIB = -lfl
endif

all: $(TARGET)
	@echo ""
	@echo "Збірка завершена! Запустіть: ./$(TARGET) <файл.py>"

$(TARGET): $(SOURCE)
	@echo "Генерація лексера з Lex-специфікації..."
	$(FLEX) -o $(TARGET).c $(SOURCE)
	@echo "Компіляція програми..."
	@if $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(FLEX_LIB) 2>/dev/null; then \
		echo "Компіляція успішна з $(FLEX_LIB)"; \
	elif $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -lfl 2>/dev/null; then \
		echo "Компіляція успішна з -lfl"; \
	elif $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -ll 2>/dev/null; then \
		echo "Компіляція успішна з -ll"; \
	elif $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c 2>/dev/null; then \
		echo "Компіляція успішна без бібліотеки Flex"; \
	else \
		echo "Помилка компіляції. Спробуйте вручну:"; \
		echo "  $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -lfl"; \
		echo "  або"; \
		echo "  $(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -ll"; \
		exit 1; \
	fi
	@echo "Очищення проміжних файлів..."
	@rm -f $(TARGET).c

clean:
	rm -f $(TARGET) $(TARGET).c

.PHONY: all clean
