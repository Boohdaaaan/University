#!/bin/bash

PROGRAM=${1:-matrix_O0}
OUTPUT=${2:-assembly_${PROGRAM}.txt}

echo "Витягування асемблерного коду з $PROGRAM..."
echo "Вихідний файл: $OUTPUT"
echo ""

if [ ! -f "./$PROGRAM" ]; then
    echo "Помилка: програма $PROGRAM не знайдена!"
    exit 1
fi

# Використовуємо objdump для витягування асемблерного коду
objdump -d "$PROGRAM" > "$OUTPUT"

echo "Асемблерний код збережено в $OUTPUT"
echo ""
echo "Розмір файлу: $(wc -l < "$OUTPUT") рядків"
echo ""
echo "Основні функції:"
grep -E "^[0-9a-f]+ <.*>:" "$OUTPUT" | head -10

