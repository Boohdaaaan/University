#!/bin/bash

PROGRAM=${1:-matrix_O0}
OUTPUT=${2:-time_${PROGRAM}.txt}

echo "Збір статистики для: $PROGRAM"
echo "Вихідний файл: $OUTPUT"
echo ""

if [ ! -f "./$PROGRAM" ]; then
    echo "програма $PROGRAM не знайдена"
    exit 1
fi

/usr/bin/time -l ./$PROGRAM > /dev/null 2> "$OUTPUT"

echo "Результати збережено в $OUTPUT"
echo ""
echo "Перші рядки результату:"
head -20 "$OUTPUT"
