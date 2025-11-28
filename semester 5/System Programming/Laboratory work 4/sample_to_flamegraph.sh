#!/bin/bash

INPUT=${1:-sample_O0.txt}
OUTPUT=${2:-flamegraph_${INPUT%.txt}.txt}

if [ ! -f "$INPUT" ]; then
    echo "файл $INPUT не знайдено"
    exit 1
fi

echo "Конвертація $INPUT у формат FlameGraph..."
echo "Вихідний файл: $OUTPUT"
echo ""

python3 - "$INPUT" << 'PYTHON_SCRIPT' > "$OUTPUT"
import sys
import re

input_file = sys.argv[1]
in_call_graph = False
current_stack = []  # поточний стек функцій
stack_data = []  # список стеків з кількістю

with open(input_file, 'r') as f:
    for line in f:
        line = line.rstrip('\n')
        
        if 'Call graph:' in line:
            in_call_graph = True
            current_stack = []
            continue
        
        if 'Total number in stack' in line:
            in_call_graph = False
            continue
        
        if not in_call_graph:
            continue
        
        # Пропускаємо Thread та DispatchQueue
        if 'Thread_' in line or 'DispatchQueue_' in line:
            current_stack = []
            continue
        
        # Визначаємо рівень вкладеності по кількості пробілів
        leading_spaces = len(line) - len(line.lstrip())
        
        # Шукаємо рядки з викликами функцій
        # Формат 1: "    6262 start  (in dyld) + 6000"
        # Формат 2: "        2121 main  (in matrix_O0) + 148"
        # Формат 3: "        + 1496 multiply_matrix(int*, int*, int*, int)  (in matrix_O0)"
        match = None
        
        # Шукаємо "(in бібліотека)" і витягуємо все перед ним
        in_match = re.search(r'\(in\s+([^)]+)\)', line)
        if not in_match:
            continue
        
        lib = in_match.group(1)
        
        # Шукаємо число перед "(in ...)"
        num_match = re.search(r'(\d+)', line[:in_match.start()])
        if not num_match:
            continue
        
        count = int(num_match.group(1))
        
        # Витягуємо назву функції (все між числом і "(in ...)", видаляємо "+" якщо є)
        func_part = line[num_match.end():in_match.start()].strip()
        func_part = re.sub(r'^\+\s*', '', func_part)  # Видаляємо "+" на початку
        func_name = func_part.strip()
        
        # Визначаємо чи це рядок з "+" (продовження попереднього рівня)
        if re.match(r'^\s+\+', line):
            level = len(current_stack)
        else:
            # Визначаємо рівень: 6 пробілів = рівень 0, 8 пробілів = рівень 1, тощо
            if leading_spaces >= 6:
                level = (leading_spaces - 6) // 2
            else:
                level = 0
        
        match = True  # Встановлюємо match для продовження обробки

        if 'matrix_O' in lib:
            full_name = func_name + ' [program]'
        elif lib == 'dyld':
            full_name = func_name + ' [dyld]'
        elif 'libsystem' in lib:
            full_name = func_name + ' [system]'
        else:
            full_name = func_name + ' [' + lib + ']'
        
        while len(current_stack) > level:
            current_stack.pop()
        
        # Додаємо поточну функцію
        if len(current_stack) == level:
            current_stack.append(full_name)
        else:
            # Якщо рівень більший, заповнюємо проміжні рівні
            while len(current_stack) < level:
                current_stack.append('unknown')
            current_stack.append(full_name)
        
        # Будуємо повний стек
        if current_stack:
            stack_str = ';'.join(current_stack)
            stack_data.append((stack_str, count))

stack_counts = {}
for stack, count in stack_data:
    if stack in stack_counts:
        stack_counts[stack] += count
    else:
        stack_counts[stack] = count

for stack, count in sorted(stack_counts.items(), key=lambda x: x[1], reverse=True):
    print(f"{stack} {count}")
PYTHON_SCRIPT

# Перевірка результату
if [ ! -s "$OUTPUT" ]; then
    echo "конвертація не вдалася, файл порожній"
    exit 1
fi

echo ""
echo "Конвертація завершена"
echo "Рядків у вихідному файлі: $(wc -l < "$OUTPUT")"
echo ""
echo "Перші рядки результату:"
head -10 "$OUTPUT"
echo ""