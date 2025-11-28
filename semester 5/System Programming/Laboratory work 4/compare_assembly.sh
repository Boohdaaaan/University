#!/bin/bash

echo "=========================================="
echo "Порівняння асемблерного коду"
echo "=========================================="
echo ""

echo "Витягування асемблерного коду O0..."
./extract_assembly.sh matrix_O0 assembly_O0.txt

echo "Витягування асемблерного коду O1..."
./extract_assembly.sh matrix_O1 assembly_O1.txt

echo "Аналіз відмінностей..."
echo ""

echo "--- Функція multiply_matrix O0 ---"
grep -A 100 "<multiply_matrix>:" assembly_O0.txt | head -50
echo ""

echo "--- Функція multiply_matrix O1 ---"
grep -A 100 "<multiply_matrix>:" assembly_O1.txt | head -50
echo ""

echo "--- SIMD/NEON інструкції в O1 ---"
grep -iE "vpmul|vadd|vmul|vfmadd|vpxor|vpshuf|simd|neon|arm64|ldp|stp|fmla|fmul" assembly_O1.txt | head -20
echo ""

echo "--- Розміри функцій ---"
echo "multiply_matrix O0:"
grep -A 200 "<multiply_matrix>:" assembly_O0.txt | grep -E "^[[:space:]]*[0-9a-f]+:" | wc -l | xargs echo "  Інструкцій:"
echo "multiply_matrix O1:"
grep -A 200 "<multiply_matrix>:" assembly_O1.txt | grep -E "^[[:space:]]*[0-9a-f]+:" | wc -l | xargs echo "  Інструкцій:"
echo ""

echo "Порівняння завершено!"
