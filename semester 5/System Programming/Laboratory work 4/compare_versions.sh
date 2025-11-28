#!/bin/bash

echo "=========================================="
echo "Порівняння версій matrix_O0 та matrix_O1"
echo "=========================================="
echo ""

# Перевірка наявності програм
if [ ! -f "./matrix_O0" ] || [ ! -f "./matrix_O1" ]; then
    echo "Помилка: не знайдено matrix_O0 або matrix_O1!"
    echo "Скомпілюйте програми спочатку:"
    echo "  gcc -O0 -g main.cpp -o matrix_O0"
    echo "  gcc -O1 -g main.cpp -o matrix_O1"
    exit 1
fi

echo "[1/4] Збір статистики time для O0..."
./profile_time.sh matrix_O0 time_matrix_O0.txt
echo ""

echo "[2/4] Збір статистики time для O1..."
./profile_time.sh matrix_O1 time_matrix_O1.txt
echo ""

echo "[3/4] Профілювання sample для O0..."
./profile_sample.sh matrix_O0 sample_matrix_O0.txt 15
echo ""

echo "[4/4] Профілювання sample для O1..."
./profile_sample.sh matrix_O1 sample_matrix_O1.txt 15
echo ""

echo "=========================================="
echo "Порівняння результатів time:"
echo "=========================================="
echo ""
echo "--- matrix_O0 ---"
grep -E "(User time|System time|Elapsed|Maximum resident)" time_matrix_O0.txt
echo ""
echo "--- matrix_O1 ---"
grep -E "(User time|System time|Elapsed|Maximum resident)" time_matrix_O1.txt
echo ""

echo "=========================================="
echo "Всі результати збережено!"
echo "=========================================="

