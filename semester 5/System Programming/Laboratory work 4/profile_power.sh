#!/bin/bash

PROGRAM=${1:-matrix_O0}
OUTPUT=${2:-power_${PROGRAM}.txt}
DURATION=${3:-15}

echo "Вимірювання енерговитрат для: $PROGRAM"
echo "Вихідний файл: $OUTPUT"
echo ""

if [ ! -f "./$PROGRAM" ]; then
    echo "програма $PROGRAM не знайдена"
    exit 1
fi

if [ "$EUID" -ne 0 ]; then
    echo "Помилка. потрібні права root."
    exit 1
fi

echo "Запуск powermetrics..."

sudo powermetrics -i 1000 -n $DURATION --samplers tasks,cpu_power -a --hide-cpu-duty-cycle > "$OUTPUT" &
POWERMETRICS_PID=$!

sleep 0.5

echo "Запуск програми..."
./$PROGRAM &
PROGRAM_PID=$!

echo "Моніторинг енерговитрат..."

wait $PROGRAM_PID

sleep 1
kill $POWERMETRICS_PID 2>/dev/null
wait $POWERMETRICS_PID 2>/dev/null

echo ""
echo "Вимірювання завершено! Результати збережено в $OUTPUT"

