import numpy as np
import matplotlib.pyplot as plt
from typing import Tuple, List, Dict, Callable


# ============================================================================
# Функції рівняння
# ============================================================================


def f(x: float) -> float:
    """Функція f(x) = x³ - 3x² - 14x - 8"""
    return x**3 - 3 * x**2 - 14 * x - 8


def df(x: float) -> float:
    """Перша похідна f'(x) = 3x² - 6x - 14"""
    return 3 * x**2 - 6 * x - 14


def d2f(x: float) -> float:
    """Друга похідна f''(x) = 6x - 6"""
    return 6 * x - 6


# ============================================================================
# Допоміжні функції для аналізу
# ============================================================================


def find_roots_intervals(
    a: float = -10.0, b: float = 10.0, step: float = 0.1
) -> List[Tuple[float, float]]:
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


def find_smallest_root_interval() -> Tuple[float, float]:
    intervals = find_roots_intervals()

    if not intervals:
        discriminant = 36 + 168
        x1 = (6 - np.sqrt(discriminant)) / 6
        x2 = (6 + np.sqrt(discriminant)) / 6

        test_points = [-5, x1 - 1, x1, x1 + 1, x2 - 1, x2, x2 + 1, 5]
        test_points.sort()

        for i in range(len(test_points) - 1):
            if f(test_points[i]) * f(test_points[i + 1]) < 0:
                return (test_points[i], test_points[i + 1])

        return (-5.0, -1.0)

    intervals.sort()
    return intervals[0]


def choose_initial_approximation(interval: Tuple[float, float]) -> float:
    return (interval[0] + interval[1]) / 2.0


# ============================================================================
# Метод релаксації
# ============================================================================


def relaxation_phi1(x: float) -> float:
    return (x**3 - 3 * x**2 - 8) / 14


def relaxation_phi1_derivative(x: float) -> float:
    """Похідна від phi1: φ'(x) = (3x² - 6x) / 14"""
    return (3 * x**2 - 6 * x) / 14


def relaxation_phi2(x: float) -> float:
    return (x**3 - 3 * x**2 - 14 * x) / 8


def relaxation_phi2_derivative(x: float) -> float:
    """Похідна від phi2: φ'(x) = (3x² - 6x - 14) / 8"""
    return (3 * x**2 - 6 * x - 14) / 8


def relaxation_phi3(x: float, lambda_param: float) -> float:
    return x - lambda_param * f(x)


def relaxation_phi3_derivative(x: float, lambda_param: float) -> float:
    """Похідна від phi3: φ'(x) = 1 - λ * f'(x)"""
    return 1 - lambda_param * df(x)


def find_optimal_lambda(interval: Tuple[float, float]) -> float:
    a, b = interval
    x_points = np.linspace(a, b, 1000)
    max_df = max([abs(df(x)) for x in x_points])

    if max_df < 1e-10:
        return 0.01

    lambda_param = 0.9 / max_df
    return lambda_param


def check_convergence_condition(
    phi_derivative: Callable,
    interval: Tuple[float, float],
    num_points: int = 1000,
    lambda_param: float = None,
) -> Tuple[bool, float]:
    a, b = interval
    x_points = np.linspace(a, b, num_points)

    try:
        derivatives = [abs(phi_derivative(x)) for x in x_points]
    except TypeError:
        if lambda_param is not None:
            derivatives = [abs(phi_derivative(x, lambda_param)) for x in x_points]
        else:
            raise ValueError("Не вдалося викликати phi_derivative")

    max_deriv = max(derivatives)

    return max_deriv < 1.0, max_deriv


def choose_relaxation_form(
    interval: Tuple[float, float],
) -> Tuple[Callable, Callable, float]:
    lambda_param = find_optimal_lambda(interval)

    def phi_with_lambda(x):
        return relaxation_phi3(x, lambda_param)

    def phi_derivative_with_lambda(x):
        return relaxation_phi3_derivative(x, lambda_param)

    conv, max_deriv = check_convergence_condition(
        phi_derivative_with_lambda, interval, lambda_param=lambda_param
    )

    if conv:
        return phi_with_lambda, phi_derivative_with_lambda, lambda_param
    conv1, max_deriv1 = check_convergence_condition(
        relaxation_phi1_derivative, interval
    )
    if conv1:
        return relaxation_phi1, relaxation_phi1_derivative, None

    conv2, max_deriv2 = check_convergence_condition(
        relaxation_phi2_derivative, interval
    )
    if conv2:
        return relaxation_phi2, relaxation_phi2_derivative, None

    return phi_with_lambda, phi_derivative_with_lambda, lambda_param


def calculate_a_priori_estimate(
    x0: float, root_estimate: float, q: float, epsilon: float
) -> int:
    if q >= 1.0:
        return 1000

    if abs(x0 - root_estimate) < epsilon:
        return 1

    numerator = np.log(abs(x0 - root_estimate) / epsilon)
    denominator = abs(np.log(q))

    if denominator < 1e-10:
        return 1000

    n = int(np.ceil(numerator / denominator))
    return max(n, 1)


def relaxation_method(
    phi: Callable, x0: float, epsilon: float, max_iterations: int = 1000
) -> Tuple[float, List[Dict], int]:
    iterations = []
    x_prev = x0

    for i in range(max_iterations):
        x_next = phi(x_prev)
        fx = f(x_next)
        error = abs(x_next - x_prev)

        iterations.append({"iteration": i + 1, "x": x_next, "f(x)": fx, "error": error})

        if error < epsilon:
            return x_next, iterations, i + 1

        x_prev = x_next

    return x_prev, iterations, max_iterations


# ============================================================================
# Модифікований метод Ньютона
# ============================================================================


def modified_newton_method(
    x0: float, epsilon: float, max_iterations: int = 1000
) -> Tuple[float, List[Dict], int]:
    iterations = []
    x_prev = x0
    df_x0 = df(x0)

    if abs(df_x0) < 1e-10:
        raise ValueError("Похідна в початковій точці близька до нуля")

    for i in range(max_iterations):
        fx = f(x_prev)
        x_next = x_prev - fx / df_x0
        error = abs(x_next - x_prev)

        iterations.append({"iteration": i + 1, "x": x_next, "f(x)": fx, "error": error})

        if error < epsilon or abs(fx) < epsilon:
            return x_next, iterations, i + 1

        x_prev = x_next

    return x_prev, iterations, max_iterations


# ============================================================================
# Виведення результатів
# ============================================================================


def print_iteration_table(
    method_name: str, iterations: List[Dict], show_all: bool = True, interval: int = 10
):
    print(f"\n{'=' * 80}")
    print(f"МЕТОД: {method_name}")
    print(f"{'=' * 80}")
    print(f"{'Крок':<8} {'Наближення x':<20} {'f(x)':<20} {'|xₙ - xₙ₋₁|':<20}")
    print(f"{'-' * 80}")

    if show_all or len(iterations) <= 20:
        for it in iterations:
            print(
                f"{it['iteration']:<8} {it['x']:<20.10f} {it['f(x)']:<20.10e} {it['error']:<20.10e}"
            )
    else:
        last_n = 5
        shown_indices = set()

        for i in range(min(5, len(iterations))):
            shown_indices.add(i)

        for i in range(interval - 1, len(iterations) - last_n, interval):
            shown_indices.add(i)

        for i in range(max(0, len(iterations) - last_n), len(iterations)):
            shown_indices.add(i)

        for idx in sorted(shown_indices):
            it = iterations[idx]
            print(
                f"{it['iteration']:<8} {it['x']:<20.10f} {it['f(x)']:<20.10e} {it['error']:<20.10e}"
            )

    print(f"{'-' * 80}")
    if iterations:
        last = iterations[-1]
        print(f"Результат: x = {last['x']:.10f}, f(x) = {last['f(x)']:.10e}")
        print(f"Кількість ітерацій: {len(iterations)}")


def plot_function_and_root(
    interval: Tuple[float, float], roots: Dict[str, float], save_path: str = "graph.png"
):
    a, b = interval
    x_range = np.linspace(a - 2, b + 2, 1000)
    y_range = [f(x) for x in x_range]

    plt.figure(figsize=(12, 8))
    plt.plot(x_range, y_range, "b-", linewidth=2, label="f(x) = x³ - 3x² - 14x - 8")
    plt.axhline(y=0, color="k", linestyle="--", linewidth=1)
    plt.axvline(x=0, color="k", linestyle="--", linewidth=1)

    colors = ["r", "g"]
    markers = ["o", "s"]
    for i, (method_name, root) in enumerate(roots.items()):
        plt.plot(
            root,
            f(root),
            marker=markers[i],
            markersize=10,
            color=colors[i],
            label=f"{method_name}: x = {root:.6f}",
        )

    plt.xlabel("x", fontsize=12)
    plt.ylabel("f(x)", fontsize=12)
    plt.title(
        "Графік функції f(x) = x³ - 3x² - 14x - 8", fontsize=14, fontweight="bold"
    )
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=10)
    plt.tight_layout()
    plt.savefig(save_path, dpi=300, bbox_inches="tight")
    print(f"\nГрафік збережено у файл: {save_path}")


# ============================================================================
# Головна функція
# ============================================================================


def main():
    print("=" * 80)
    print("Лабораторна робота 1: Знаходження найменшого кореня нелінійного рівняння")
    print("Рівняння: x³ - 3x² - 14x - 8 = 0")
    print("Автор: Кузнецов Богдан, група ТК-31")
    print("=" * 80)

    # Введення точності
    default_epsilon = 1e-4
    print(f"\nТочність за замовчуванням: ε = {default_epsilon}")
    user_input = input(
        "Введіть нову точність (або натисніть Enter для використання за замовчуванням): "
    ).strip()

    if user_input:
        try:
            epsilon = float(user_input)
            if epsilon <= 0:
                print(
                    "Помилка: точність повинна бути додатньою. Використовується значення за замовчуванням."
                )
                epsilon = default_epsilon
        except ValueError:
            print(
                "Помилка: некоректне значення. Використовується значення за замовчуванням."
            )
            epsilon = default_epsilon
    else:
        epsilon = default_epsilon

    print(f"\nВикористовується точність: ε = {epsilon}")
    print("=" * 80)

    # Знаходження інтервалу для найменшого кореня
    print("\n1. ВИЗНАЧЕННЯ ІНТЕРВАЛУ ДЛЯ НАЙМЕНШОГО КОРЕНЯ")
    print("-" * 80)
    interval = find_smallest_root_interval()
    print(f"Знайдено інтервал: [{interval[0]:.6f}, {interval[1]:.6f}]")
    print(f"f({interval[0]:.6f}) = {f(interval[0]):.10e}")
    print(f"f({interval[1]:.6f}) = {f(interval[1]):.10e}")

    # Вибір початкового наближення
    x0 = choose_initial_approximation(interval)
    print(f"\nПочаткове наближення: x₀ = {x0:.10f}")
    print(f"f(x₀) = {f(x0):.10e}")

    # Вибір форми методу релаксації
    print("\n2. ВИБІР ФОРМИ МЕТОДУ РЕЛАКСАЦІЇ")
    print("-" * 80)
    phi, phi_derivative, lambda_param = choose_relaxation_form(interval)
    conv_check, max_deriv = check_convergence_condition(
        phi_derivative, interval, lambda_param=lambda_param
    )
    print(f"Максимальне значення |φ'(x)| на інтервалі: {max_deriv:.10f}")
    print(
        f"Умова збіжності (|φ'(x)| < 1): {'Виконується' if conv_check else 'Не виконується'}"
    )
    if lambda_param is not None:
        print(f"Параметр релаксації λ = {lambda_param:.10f}")

    # Апріорна оцінка для методу релаксації
    print("\n3. АПРІОРНА ОЦІНКА КІЛЬКОСТІ ІТЕРАЦІЙ (метод релаксації)")
    print("-" * 80)
    # Використовуємо кінець інтервалу як оцінку кореня для апріорної оцінки
    root_estimate = (
        interval[1] if abs(f(interval[1])) < abs(f(interval[0])) else interval[0]
    )
    a_priori_n = calculate_a_priori_estimate(x0, root_estimate, max_deriv, epsilon)
    print(f"Апріорна оцінка кількості ітерацій: n ≥ {a_priori_n}")
    print(f"Формула: n ≥ ln(|x₀ - x*| / ε) / ln(q), де q = {max_deriv:.10f}")

    # Метод релаксації
    print("\n4. МЕТОД РЕЛАКСАЦІЇ")
    print("-" * 80)
    root_relaxation, iterations_relaxation, n_relaxation = relaxation_method(
        phi, x0, epsilon, max_iterations=max(1000, a_priori_n * 2)
    )

    print_iteration_table(
        "МЕТОД РЕЛАКСАЦІЇ", iterations_relaxation, show_all=(n_relaxation <= 50)
    )

    print(f"\nАпостеріорна оцінка: виконано {n_relaxation} ітерацій")
    if a_priori_n > 0:
        print(
            f"Порівняння з апріорною оцінкою: {a_priori_n} (апріорна) vs {n_relaxation} (фактична)"
        )

    # Модифікований метод Ньютона
    print("\n5. МОДИФІКОВАНИЙ МЕТОД НЬЮТОНА")
    print("-" * 80)
    root_newton, iterations_newton, n_newton = modified_newton_method(
        x0, epsilon, max_iterations=1000
    )

    print_iteration_table(
        "МОДИФІКОВАНИЙ МЕТОД НЬЮТОНА", iterations_newton, show_all=(n_newton <= 50)
    )

    print(f"\nАпостеріорна оцінка: виконано {n_newton} ітерацій")

    # Порівняння результатів
    print("\n6. ПОРІВНЯННЯ РЕЗУЛЬТАТІВ")
    print("=" * 80)
    print(f"{'Метод':<30} {'Корінь x':<20} {'f(x)':<20} {'Ітерацій':<10}")
    print("-" * 80)
    print(
        f"{'Метод релаксації':<30} {root_relaxation:<20.10f} {f(root_relaxation):<20.10e} {n_relaxation:<10}"
    )
    print(
        f"{'Модифікований Ньютон':<30} {root_newton:<20.10f} {f(root_newton):<20.10e} {n_newton:<10}"
    )
    print("-" * 80)

    diff = abs(root_relaxation - root_newton)
    print(f"\nРізниця між методами: |x_relaxation - x_newton| = {diff:.10e}")

    if diff < epsilon:
        print("Результати методів збігаються в межах заданої точності.")
    else:
        print("Результати методів відрізняються більше ніж на задану точність.")

    # Побудова графіка
    print("\n7. ПОБУДОВА ГРАФІКА")
    print("-" * 80)
    roots_dict = {"Релаксація": root_relaxation, "Модифікований Ньютон": root_newton}
    plot_function_and_root(interval, roots_dict)

    print("\n" + "=" * 80)
    print("РОБОТУ ЗАВЕРШЕНО")
    print("=" * 80)


if __name__ == "__main__":
    main()
