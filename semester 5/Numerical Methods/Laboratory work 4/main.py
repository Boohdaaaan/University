import numpy as np
import matplotlib.pyplot as plt
from typing import Callable
from scipy.optimize import fsolve


# ============================================================================
# Функції рівняння
# ============================================================================


def f(x: float) -> float:
    """Функція f(x) = x^4 + 4x - 2 cos x"""
    return x**4 + 4 * x - 2 * np.cos(x)


def find_root_intervals(
    a: float = -2.0, b: float = 2.0, step: float = 0.01
) -> list[tuple[float, float]]:
    intervals = []
    x = a
    prev_sign = np.sign(f(x))

    while x < b:
        x += step
        current_sign = np.sign(f(x))
        if current_sign != prev_sign and prev_sign != 0:
            intervals.append((x - step, x))
        prev_sign = current_sign

    return intervals


def find_largest_root_interval(
    a: float = -2.0, b: float = 2.0, step: float = 0.01
) -> tuple[float, float]:
    intervals = find_root_intervals(a, b, step)

    if not intervals:
        intervals = find_root_intervals(a - 1, b + 1, step)

    if not intervals:
        return (0.0, 2.0)

    intervals.sort()
    return intervals[-1]


# ============================================================================
# Поліном Лагранжа
# ============================================================================


def lagrange_basis_polynomial(x: float, nodes_x: np.ndarray, i: int) -> float:
    result = 1.0
    xi = nodes_x[i]

    for j in range(len(nodes_x)):
        if j != i:
            xj = nodes_x[j]
            if abs(xi - xj) < 1e-12:
                continue
            result *= (x - xj) / (xi - xj)

    return result


def lagrange_polynomial(x: float, nodes_x: np.ndarray, nodes_y: np.ndarray) -> float:
    result = 0.0
    for i in range(len(nodes_x)):
        result += nodes_y[i] * lagrange_basis_polynomial(x, nodes_x, i)
    return result


def build_lagrange_polynomial(nodes_x: np.ndarray, nodes_y: np.ndarray) -> Callable:
    def P(x):
        if isinstance(x, (int, float)):
            return lagrange_polynomial(x, nodes_x, nodes_y)
        else:
            return np.array([lagrange_polynomial(xi, nodes_x, nodes_y) for xi in x])

    return P


def find_roots_polynomial(
    nodes_x: np.ndarray,
    nodes_y: np.ndarray,
    interval: tuple[float, float],
    tol: float = 1e-6,
) -> list[float]:
    a, b = interval
    P = build_lagrange_polynomial(nodes_x, nodes_y)

    def P_zero(x):
        return P(x)

    roots = []
    n_points = 100
    x_test = np.linspace(a, b, n_points)
    y_test = P(x_test)

    for i in range(len(x_test) - 1):
        if y_test[i] * y_test[i + 1] < 0:
            try:
                root = fsolve(P_zero, (x_test[i] + x_test[i + 1]) / 2, xtol=tol)[0]
                if a <= root <= b:
                    if not any(abs(root - r) < tol for r in roots):
                        roots.append(root)
            except:
                continue

    return sorted(roots)


# ============================================================================
# Пряма інтерполяція
# ============================================================================


def direct_interpolation(interval: tuple[float, float], n_nodes: int = 10) -> dict:
    a, b = interval

    nodes_x = np.linspace(a, b, n_nodes)
    nodes_y = np.array([f(x) for x in nodes_x])

    polynomial = build_lagrange_polynomial(nodes_x, nodes_y)
    roots = find_roots_polynomial(nodes_x, nodes_y, interval)
    largest_root = max(roots) if roots else None

    return {
        "nodes_x": nodes_x,
        "nodes_y": nodes_y,
        "polynomial": polynomial,
        "roots": roots,
        "largest_root": largest_root,
    }


# ============================================================================
# Обернена інтерполяція
# ============================================================================


def inverse_interpolation(interval: tuple[float, float], n_nodes: int = 10) -> dict:
    a, b = interval

    n_fine = max(100, n_nodes * 10)
    x_fine = np.linspace(a, b, n_fine)
    y_fine = np.array([f(x) for x in x_fine])

    indices_sorted = np.argsort(np.abs(y_fine))
    selected_indices = indices_sorted[:n_nodes]
    selected_indices = np.sort(selected_indices)

    nodes_x = x_fine[selected_indices]
    nodes_y = y_fine[selected_indices]

    if np.all(nodes_y >= 0) or np.all(nodes_y <= 0):
        root_intervals = find_root_intervals(a, b, step=0.01)
        if root_intervals:
            largest_interval = sorted(root_intervals)[-1]
            center = (largest_interval[0] + largest_interval[1]) / 2
            nodes_x = np.linspace(max(a, center - 0.1), min(b, center + 0.1), n_nodes)
            nodes_y = np.array([f(x) for x in nodes_x])

    polynomial = build_lagrange_polynomial(nodes_y, nodes_x)

    try:
        root = polynomial(0.0)
        if not (a <= root <= b):
            idx_closest = np.argmin(np.abs(nodes_y))
            root = nodes_x[idx_closest]
    except:
        idx_closest = np.argmin(np.abs(nodes_y))
        root = nodes_x[idx_closest]

    return {
        "nodes_y": nodes_y,
        "nodes_x": nodes_x,
        "polynomial": polynomial,
        "root": root,
    }


# ============================================================================
# Візуалізація
# ============================================================================


def plot_interpolation_results(
    interval: tuple[float, float],
    direct_result: dict,
    inverse_result: dict,
    save_path: str = "graph.png",
):
    a, b = interval

    x_range = np.linspace(a - 0.5, b + 0.5, 1000)
    y_range = [f(x) for x in x_range]

    P = direct_result["polynomial"]
    y_poly = P(x_range)

    plt.figure(figsize=(14, 10))

    plt.plot(x_range, y_range, "b-", linewidth=2, label="f(x) = x⁴ + 4x - 2cos(x)")
    plt.axhline(y=0, color="k", linestyle="--", linewidth=1, alpha=0.5)
    plt.axvline(x=0, color="k", linestyle="--", linewidth=1, alpha=0.5)

    plt.plot(
        x_range,
        y_poly,
        "r--",
        linewidth=2,
        alpha=0.7,
        label="Поліном Лагранжа (пряма інтерполяція)",
    )

    plt.plot(
        direct_result["nodes_x"],
        direct_result["nodes_y"],
        "ro",
        markersize=8,
        label="Вузли інтерполяції (пряма)",
    )

    if direct_result["largest_root"] is not None:
        plt.plot(
            direct_result["largest_root"],
            0,
            "rs",
            markersize=12,
            label=f"Корінь (пряма): x = {direct_result['largest_root']:.6f}",
        )

    plt.plot(
        inverse_result["root"],
        0,
        "g^",
        markersize=12,
        label=f"Корінь (обернена): x = {inverse_result['root']:.6f}",
    )

    for i, (x_node, y_node) in enumerate(
        zip(inverse_result["nodes_x"], inverse_result["nodes_y"])
    ):
        if i == 0:
            plt.plot(
                [x_node, x_node],
                [y_node, 0],
                "g:",
                linewidth=1,
                alpha=0.5,
                label="Вузли оберненої інтерполяції",
            )
        else:
            plt.plot([x_node, x_node], [y_node, 0], "g:", linewidth=1, alpha=0.5)

    plt.xlabel("x", fontsize=12)
    plt.ylabel("f(x)", fontsize=12)
    plt.title(
        "Інтерполяція та обернена інтерполяція для знаходження найбільшого кореня",
        fontsize=14,
        fontweight="bold",
    )
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=10, loc="best")
    plt.tight_layout()
    plt.savefig(save_path, dpi=300, bbox_inches="tight")
    print(f"\nГрафік збережено у файл: {save_path}")


# ============================================================================
# Виведення результатів
# ============================================================================


def print_interpolation_table(
    method_name: str, nodes_x: np.ndarray, nodes_y: np.ndarray
):
    print(f"\n{'=' * 80}")
    print(f"{method_name.upper()}: ТАБЛИЦЯ ВУЗЛІВ ІНТЕРПОЛЯЦІЇ")
    print(f"{'=' * 80}")
    print(f"{'№':<5} {'x':<20} {'f(x)':<20}")
    print(f"{'-' * 80}")

    for i, (x, y) in enumerate(zip(nodes_x, nodes_y)):
        print(f"{i + 1:<5} {x:<20.10f} {y:<20.10e}")

    print(f"{'-' * 80}")


# ============================================================================
# Головна функція
# ============================================================================


def main():
    print("=" * 80)
    print("Лабораторна робота 4: Інтерполяція та обернена інтерполяція")
    print("Знаходження найбільшого кореня рівняння x^4 + 4x - 2 cos x = 0")
    print("Автор: Кузнецов Богдан, група ТК-31")
    print("=" * 80)

    print("\n1. ВИЗНАЧЕННЯ ІНТЕРВАЛУ ДЛЯ НАЙБІЛЬШОГО КОРЕНЯ")
    print("-" * 80)
    interval = find_largest_root_interval()
    print(f"Знайдено інтервал: [{interval[0]:.6f}, {interval[1]:.6f}]")
    print(f"f({interval[0]:.6f}) = {f(interval[0]):.10e}")
    print(f"f({interval[1]:.6f}) = {f(interval[1]):.10e}")

    print("\n2. ПРЯМА ІНТЕРПОЛЯЦІЯ")
    print("-" * 80)
    print("Побудова полінома Лагранжа для f(x) на 10 рівновіддалених вузлах")

    direct_result = direct_interpolation(interval, n_nodes=10)

    print_interpolation_table(
        "ПРЯМА ІНТЕРПОЛЯЦІЯ", direct_result["nodes_x"], direct_result["nodes_y"]
    )

    print(f"\nЗнайдені корені полінома Лагранжа:")
    if direct_result["roots"]:
        for i, root in enumerate(direct_result["roots"]):
            print(f"  x_{i + 1} = {root:.10f}, f(x_{i + 1}) = {f(root):.10e}")
        print(
            f"\nНайбільший корінь (пряма інтерполяція): x = {direct_result['largest_root']:.10f}"
        )
        print(f"f(x) = {f(direct_result['largest_root']):.10e}")
    else:
        print("  Корені не знайдено")

    print("\n3. ОБЕРНЕНА ІНТЕРПОЛЯЦІЯ")
    print("-" * 80)
    print("Побудова полінома Лагранжа для x як функції від y = f(x)")
    print("Використовуються вузли навколо y = 0")

    inverse_result = inverse_interpolation(interval, n_nodes=10)

    print_interpolation_table(
        "ОБЕРНЕНА ІНТЕРПОЛЯЦІЯ", inverse_result["nodes_x"], inverse_result["nodes_y"]
    )

    print(
        f"\nЗнайдений корінь (обернена інтерполяція): x = {inverse_result['root']:.10f}"
    )
    print(f"f(x) = {f(inverse_result['root']):.10e}")

    print("\n4. ПОРІВНЯННЯ РЕЗУЛЬТАТІВ")
    print("=" * 80)
    print(f"{'Метод':<30} {'Корінь x':<20} {'f(x)':<20}")
    print("-" * 80)

    if direct_result["largest_root"] is not None:
        print(
            f"{'Пряма інтерполяція':<30} {direct_result['largest_root']:<20.10f} {f(direct_result['largest_root']):<20.10e}"
        )

    print(
        f"{'Обернена інтерполяція':<30} {inverse_result['root']:<20.10f} {f(inverse_result['root']):<20.10e}"
    )
    print("-" * 80)

    if direct_result["largest_root"] is not None:
        diff = abs(direct_result["largest_root"] - inverse_result["root"])
        print(f"\nРізниця між методами: |x_direct - x_inverse| = {diff:.10e}")

    print("\n5. ПЕРЕВІРКА ТОЧНОСТІ")
    print("-" * 80)
    print("Знаходження точного кореня через scipy.optimize.fsolve:")

    try:
        exact_root = fsolve(f, interval[1], xtol=1e-10)[0]
        print(f"Точний корінь: x = {exact_root:.10f}")
        print(f"f(x) = {f(exact_root):.10e}")

        if direct_result["largest_root"] is not None:
            error_direct = abs(direct_result["largest_root"] - exact_root)
            print(f"\nПохибка прямої інтерполяції: {error_direct:.10e}")

        error_inverse = abs(inverse_result["root"] - exact_root)
        print(f"Похибка оберненої інтерполяції: {error_inverse:.10e}")
    except Exception as e:
        print(f"Не вдалося знайти точний корінь: {e}")

    print("\n6. ПОБУДОВА ГРАФІКА")
    print("-" * 80)
    plot_interpolation_results(interval, direct_result, inverse_result)

    print("\n" + "=" * 80)
    print("РОБОТУ ЗАВЕРШЕНО")
    print("=" * 80)


if __name__ == "__main__":
    main()
