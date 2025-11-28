#!/usr/bin/env python3

import re
import matplotlib.pyplot as plt
import numpy as np

def extract_power_data(filename):
    """Витягує дані про енергію з файлу powermetrics"""
    with open(filename, 'r') as f:
        content = f.read()
    
    # Знаходимо останні значення (найбільш актуальні)
    cpu_power = re.findall(r'CPU Power:\s+(\d+)\s+mW', content)
    gpu_power = re.findall(r'GPU Power:\s+(\d+)\s+mW', content)
    combined_power = re.findall(r'Combined Power.*?:\s+(\d+)\s+mW', content)
    
    # Беремо останнє значення
    cpu = int(cpu_power[-1]) if cpu_power else 0
    gpu = int(gpu_power[-1]) if gpu_power else 0
    combined = int(combined_power[-1]) if combined_power else 0
    
    # Витягуємо дані про програму
    program_name = 'matrix_O0' if 'O0' in filename else 'matrix_O1'
    program_lines = re.findall(rf'{program_name}\s+\d+\s+(\d+\.\d+)\s+(\d+\.\d+)', content)
    
    # Середнє значення CPU usage
    avg_cpu_ms = np.mean([float(x[0]) for x in program_lines]) if program_lines else 0
    avg_user_pct = np.mean([float(x[1]) for x in program_lines]) if program_lines else 0
    
    return {
        'cpu_power': cpu,
        'gpu_power': gpu,
        'combined_power': combined,
        'cpu_ms_per_sec': avg_cpu_ms,
        'user_percent': avg_user_pct
    }

def create_visualization():
    """Створює візуалізацію даних"""
    # Витягуємо дані
    data_o0 = extract_power_data('power_matrix_O0.txt')
    data_o1 = extract_power_data('power_matrix_O1.txt')
    
    # Створюємо фігуру з підграфіками
    fig, axes = plt.subplots(2, 2, figsize=(14, 10))
    fig.suptitle('Порівняння енерговитрат: matrix_O0 vs matrix_O1', fontsize=16, fontweight='bold')
    
    # 1. Загальна енергія системи
    ax1 = axes[0, 0]
    categories = ['CPU Power', 'GPU Power', 'Combined Power']
    o0_values = [data_o0['cpu_power'], data_o0['gpu_power'], data_o0['combined_power']]
    o1_values = [data_o1['cpu_power'], data_o1['gpu_power'], data_o1['combined_power']]
    
    x = np.arange(len(categories))
    width = 0.35
    
    bars1 = ax1.bar(x - width/2, o0_values, width, label='O0 (без оптимізації)', color='#ff6b6b', alpha=0.8)
    bars2 = ax1.bar(x + width/2, o1_values, width, label='O1 (з оптимізацією)', color='#4ecdc4', alpha=0.8)
    
    ax1.set_ylabel('Енергія (mW)', fontsize=12)
    ax1.set_title('Загальна енергія системи', fontsize=13, fontweight='bold')
    ax1.set_xticks(x)
    ax1.set_xticklabels(categories)
    ax1.legend()
    ax1.grid(axis='y', alpha=0.3)
    
    # Додаємо значення на стовпчики
    for bars in [bars1, bars2]:
        for bar in bars:
            height = bar.get_height()
            ax1.text(bar.get_x() + bar.get_width()/2., height,
                    f'{int(height)} mW',
                    ha='center', va='bottom', fontsize=9)
    
    # 2. CPU використання програми
    ax2 = axes[0, 1]
    cpu_usage = [data_o0['cpu_ms_per_sec'], data_o1['cpu_ms_per_sec']]
    colors = ['#ff6b6b', '#4ecdc4']
    bars = ax2.bar(['O0', 'O1'], cpu_usage, color=colors, alpha=0.8)
    ax2.set_ylabel('CPU ms/сек', fontsize=12)
    ax2.set_title('CPU використання програми', fontsize=13, fontweight='bold')
    ax2.grid(axis='y', alpha=0.3)
    
    for bar in bars:
        height = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.1f} ms/s',
                ha='center', va='bottom', fontsize=10)
    
    # 3. User% використання CPU
    ax3 = axes[1, 0]
    user_pct = [data_o0['user_percent'], data_o1['user_percent']]
    bars = ax3.bar(['O0', 'O1'], user_pct, color=colors, alpha=0.8)
    ax3.set_ylabel('User%', fontsize=12)
    ax3.set_title('Відсоток часу в user mode', fontsize=13, fontweight='bold')
    ax3.set_ylim([0, 100])
    ax3.grid(axis='y', alpha=0.3)
    
    for bar in bars:
        height = bar.get_height()
        ax3.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.1f}%',
                ha='center', va='bottom', fontsize=10)
    
    # 4. Порівняння ефективності
    ax4 = axes[1, 1]
    
    # Розраховуємо зменшення
    power_reduction = ((data_o0['combined_power'] - data_o1['combined_power']) / data_o0['combined_power']) * 100
    cpu_reduction = ((data_o0['cpu_ms_per_sec'] - data_o1['cpu_ms_per_sec']) / data_o0['cpu_ms_per_sec']) * 100
    
    metrics = ['Зменшення\nенергії', 'Зменшення\nCPU usage']
    reductions = [power_reduction, cpu_reduction]
    colors_reduction = ['#95e1d3', '#f38181']
    
    bars = ax4.bar(metrics, reductions, color=colors_reduction, alpha=0.8)
    ax4.set_ylabel('Відсоток зменшення (%)', fontsize=12)
    ax4.set_title('Ефективність оптимізації', fontsize=13, fontweight='bold')
    ax4.axhline(y=0, color='black', linestyle='-', linewidth=0.5)
    ax4.grid(axis='y', alpha=0.3)
    
    for bar in bars:
        height = bar.get_height()
        ax4.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.1f}%',
                ha='center', va='bottom' if height > 0 else 'top', fontsize=10, fontweight='bold')
    
    plt.tight_layout()
    plt.savefig('power_comparison.png', dpi=300, bbox_inches='tight')
    print("графік збережено як 'power_comparison.png'")

if __name__ == '__main__':
    try:
        create_visualization()
    except FileNotFoundError as e:
        print(f"файл не знайдено - {e}")
    except Exception as e:
        print(f"Помилка: {e}")
        import traceback
        traceback.print_exc()

