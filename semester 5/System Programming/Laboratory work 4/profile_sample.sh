#!/bin/bash

PROGRAM=${1:-matrix_O0}
OUTPUT=${2:-sample_${PROGRAM}.txt}
DURATION=${3:-15}

echo "Профілювання програми: $PROGRAM"
echo "Вихідний файл: $OUTPUT"
echo ""

# Перевірка наявності програми
if [ ! -f "./$PROGRAM" ]; then
    echo "Помилка: програма $PROGRAM не знайдена!"
    exit 1
fi

# Запускаємо в фоні
./$PROGRAM &
PID=$!

echo "Програма запущена з PID: $PID"

# sleep 0.009

echo "Запуск sample на $DURATION секунд..."
sample $PID $DURATION -f "$OUTPUT"

wait $PID

echo ""
echo "Профілювання завершено! Результати збережено в $OUTPUT"

