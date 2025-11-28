# Лабораторна робота 4: Профілювання та оптимізація

## Компіляція програми без оптимізації (O0) та з оптимізацією (O1)
```bash
gcc -O0 -g main.cpp -o matrix_O0
gcc -O1 -g main.cpp -o matrix_O1
```

## Скрипти для профілювання

### Основні скрипти:

#### 1. `profile_sample.sh` - Профілювання за допомогою sample
```bash
./profile_sample.sh [matrix_O0|matrix_O1] [output_file] [duration]
```
Приклад:
```bash
./profile_sample.sh matrix_O0 sample_O0.txt 15
```

#### 2. `profile_time.sh` - Збір статистики time --verbose
```bash
./profile_time.sh [matrix_O0|matrix_O1] [output_file]
```
Приклад:
```bash
./profile_time.sh matrix_O0 time_O0.txt
```

#### 3. `profile_power.sh` - Вимірювання енерговитрат
```bash
sudo ./profile_power.sh [matrix_O0|matrix_O1] [output_file] [duration]
```
Приклад:
```bash
sudo ./profile_power.sh matrix_O0 power_O0.txt 15
```

#### 4. `sample_to_flamegraph.sh` - Конвертація sample у формат FlameGraph
```bash
./sample_to_flamegraph.sh [sample_file] [output_file]
```
Приклад:
```bash
./sample_to_flamegraph.sh sample_O0.txt flamegraph_O0.txt
```

#### 5. `extract_assembly.sh` - Витягування асемблерного коду
```bash
./extract_assembly.sh [matrix_O0|matrix_O1] [output_file]
```
Приклад:
```bash
./extract_assembly.sh matrix_O0 assembly_O0.txt
```

#### 6. `compare_assembly.sh` - Порівняння асемблерного коду O0 та O1
```bash
./compare_assembly.sh
```

#### 7. `compare_versions.sh` - Порівняння версій O0 та O1
```bash
./compare_versions.sh
```

## Побудова FlameGraph

### 1. Зібрати дані sample:
```bash
./profile_sample.sh matrix_O0 sample_matrix_O0.txt 15
./profile_sample.sh matrix_O1 sample_matrix_O1.txt 15
```

### 2. Конвертувати у формат FlameGraph:
```bash
./sample_to_flamegraph.sh sample_matrix_O0.txt flamegraph_O0.txt
./sample_to_flamegraph.sh sample_matrix_O1.txt flamegraph_O1.txt
```

### 3. Побудувати SVG графіки:
```bash
./FlameGraph/flamegraph.pl flamegraph_O0.txt > flamegraph_O0.svg
./FlameGraph/flamegraph.pl flamegraph_O1.txt > flamegraph_O1.svg
```

## Порівняння асемблерного коду

### Витягування асемблерного коду:
```bash
objdump -d matrix_O0 > assembly_O0.txt
objdump -d matrix_O1 > assembly_O1.txt
```

### Пошук SIMD/AVX інструкцій:
```bash
grep -iE "vpmul|vadd|vmul|vfmadd|neon|arm64" assembly_O1.txt
```

### Порівняння функцій:
```bash
# Функція multiply_matrix
grep -A 100 "<multiply_matrix>:" assembly_O0.txt
grep -A 100 "<multiply_matrix>:" assembly_O1.txt
```

## Структура результатів

Після виконання всіх скриптів будуть створені файли:
- `sample_matrix_O0.txt`, `sample_matrix_O1.txt` - профілі sample
- `flamegraph_O0.txt`, `flamegraph_O1.txt` - дані для FlameGraph
- `flamegraph_O0.svg`, `flamegraph_O1.svg` - візуалізації FlameGraph
- `time_matrix_O0.txt`, `time_matrix_O1.txt` - статистика time
- `power_matrix_O0.txt`, `power_matrix_O1.txt` - енерговитрати
- `assembly_O0.txt`, `assembly_O1.txt` - асемблерний код

## Швидкий старт

Для автоматичного виконання всіх кроків:
```bash
./run_all.sh
```

Потім встановіть FlameGraph та побудуйте графіки (див. вище).
