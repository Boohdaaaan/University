import numpy as np


# ============================================================================
# Метод Гаусса для розв'язання системи лінійних рівнянь
# ============================================================================


def gauss_elimination(A: np.ndarray, b: np.ndarray) -> np.ndarray:
    n = len(A)
    A = A.copy().astype(float)
    b = b.copy().astype(float)

    for i in range(n):
        max_row = i
        for k in range(i + 1, n):
            if abs(A[k, i]) > abs(A[max_row, i]):
                max_row = k

        if max_row != i:
            A[[i, max_row]] = A[[max_row, i]]
            b[[i, max_row]] = b[[max_row, i]]

        if abs(A[i, i]) < 1e-10:
            raise ValueError("Матриця вироджена або близька до виродженої")

        for k in range(i + 1, n):
            factor = A[k, i] / A[i, i]
            A[k, i:] -= factor * A[i, i:]
            b[k] -= factor * b[i]

    x = np.zeros(n)
    for i in range(n - 1, -1, -1):
        x[i] = (b[i] - np.dot(A[i, i + 1 :], x[i + 1 :])) / A[i, i]

    return x


# ============================================================================
# Степеневій метод для найменшого власного значення
# ============================================================================


def power_method_inverse(
    A: np.ndarray, x0: np.ndarray = None, tol: float = 1e-6, max_iter: int = 1000
) -> tuple[float, np.ndarray, list[dict]]:
    n = len(A)

    if x0 is None:
        x0 = np.random.rand(n)
        x0 = x0 / np.linalg.norm(x0)
    else:
        x0 = x0.copy()
        x0 = x0 / np.linalg.norm(x0)

    x = x0.copy()
    iterations = []
    lambda_prev = 0.0

    for iteration in range(max_iter):
        try:
            y = gauss_elimination(A, x)
        except ValueError as e:
            raise ValueError(
                f"Помилка при розв'язанні системи на ітерації {iteration + 1}: {e}"
            )

        y_norm = np.linalg.norm(y)
        if y_norm < 1e-10:
            raise ValueError("Вектор став нульовим, матриця може бути виродженою")

        lambda_inv = y_norm
        y = y / y_norm

        if abs(lambda_inv) < 1e-12:
            raise ValueError("Власне значення оберненої матриці близьке до нуля")

        lambda_min = 1.0 / lambda_inv
        error = abs(lambda_min - lambda_prev)

        iterations.append(
            {
                "iteration": iteration + 1,
                "lambda_min": lambda_min,
                "lambda_inv": lambda_inv,
                "error": error,
                "vector_norm": np.linalg.norm(y),
            }
        )

        if iteration > 0 and error < tol:
            return lambda_min, y, iterations

        x = y.copy()
        lambda_prev = lambda_min

    return lambda_min, x, iterations


# ============================================================================
# Виведення результатів
# ============================================================================


def print_iteration_table(
    iterations: list[dict], show_all: bool = True, interval: int = 10
):
    print(f"\n{'=' * 80}")
    print(f"ТАБЛИЦЯ ІТЕРАЦІЙ СТЕПЕНЕВОГО МЕТОДУ")
    print(f"{'=' * 80}")
    print(f"{'Крок':<8} {'λ_min (наближення)':<25} {'λ(A⁻¹)':<20} {'|λₙ - λₙ₋₁|':<20}")
    print(f"{'-' * 80}")

    if show_all or len(iterations) <= 20:
        for it in iterations:
            print(
                f"{it['iteration']:<8} {it['lambda_min']:<25.10f} {it['lambda_inv']:<20.10f} {it['error']:<20.10e}"
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
                f"{it['iteration']:<8} {it['lambda_min']:<25.10f} {it['lambda_inv']:<20.10f} {it['error']:<20.10e}"
            )

    print(f"{'-' * 80}")
    if iterations:
        last = iterations[-1]
        print(f"Результат: λ_min = {last['lambda_min']:.10f}")
        print(f"Кількість ітерацій: {len(iterations)}")


def compare_with_exact(A: np.ndarray) -> dict:
    eigenvalues, eigenvectors = np.linalg.eig(A)
    eigenvalues = np.real(eigenvalues)

    lambda_min_exact = np.min(eigenvalues)
    min_idx = np.argmin(eigenvalues)
    eigenvector_exact = np.real(eigenvectors[:, min_idx])
    eigenvector_exact = eigenvector_exact / np.linalg.norm(eigenvector_exact)

    return {
        "eigenvalues": eigenvalues,
        "lambda_min": lambda_min_exact,
        "eigenvector": eigenvector_exact,
        "min_index": min_idx,
    }


# ============================================================================
# Головна функція
# ============================================================================


def main():
    print("=" * 80)
    print("Лабораторна робота 3: Задачі власних значень")
    print("Степеневій метод для знаходження найменшого власного значення")
    print("Автор: Кузнецов Богдан, група ТК-31")
    print("=" * 80)

    A = np.array([[2.3, 1.4, 0.6], [1.4, 1.7, 0.5], [0.6, 0.5, 1.3]], dtype=float)

    print("\nМатриця A:")
    print(A)
    print()

    default_tol = 1e-6
    print(f"Точність за замовчуванням: ε = {default_tol}")
    user_input = input(
        "Введіть нову точність (або натисніть Enter для використання за замовчуванням): "
    ).strip()

    if user_input:
        try:
            tol = float(user_input)
            if tol <= 0:
                print(
                    "Помилка: точність повинна бути додатньою. Використовується значення за замовчуванням."
                )
                tol = default_tol
        except ValueError:
            print(
                "Помилка: некоректне значення. Використовується значення за замовчуванням."
            )
            tol = default_tol
    else:
        tol = default_tol

    print(f"\nВикористовується точність: ε = {tol}")
    print("=" * 80)

    print("\n1. СТЕПЕНЕВІЙ МЕТОД ДО ОБЕРНЕНОЇ МАТРИЦІ")
    print("-" * 80)
    print("Алгоритм:")
    print("  - На кожній ітерації розв'язуємо систему A·y = x")
    print("  - Нормалізуємо вектор y")
    print("  - Обчислюємо λ_min(A) = 1 / λ_max(A⁻¹)")
    print("-" * 80)

    try:
        lambda_min, eigenvector, iterations = power_method_inverse(A, tol=tol)

        print_iteration_table(iterations, show_all=(len(iterations) <= 50))

        print(f"\nРезультат степеневого методу:")
        print(f"  Найменше власне значення: λ_min = {lambda_min:.10f}")
        print(f"  Відповідний власний вектор:")
        for i, component in enumerate(eigenvector):
            print(f"    v[{i}] = {component:.10f}")
        print(f"  Норма вектора: ||v|| = {np.linalg.norm(eigenvector):.10f}")
        print(f"  Кількість ітерацій: {len(iterations)}")

    except Exception as e:
        print(f"Помилка при виконанні степеневого методу: {e}")
        return

    print("\n" + "=" * 80)
    print("2. ПОРІВНЯННЯ З ТОЧНИМ РОЗВ'ЯЗКОМ")
    print("-" * 80)

    exact_result = compare_with_exact(A)

    print("Точні власні значення (через numpy.linalg.eig):")
    for i, eigval in enumerate(exact_result["eigenvalues"]):
        marker = " ← найменше" if i == exact_result["min_index"] else ""
        print(f"  λ_{i + 1} = {eigval:.10f}{marker}")

    print(
        f"\nНайменше власне значення (точне): λ_min = {exact_result['lambda_min']:.10f}"
    )
    print(f"Найменше власне значення (степеневий метод): λ_min = {lambda_min:.10f}")

    error = abs(lambda_min - exact_result["lambda_min"])
    relative_error = (
        error / abs(exact_result["lambda_min"])
        if exact_result["lambda_min"] != 0
        else error
    )

    print(f"\nАбсолютна похибка: |λ_min_exact - λ_min_approx| = {error:.10e}")
    print(f"Відносна похибка: {relative_error:.10e}")

    print(f"\nТочний власний вектор (нормалізований):")
    for i, component in enumerate(exact_result["eigenvector"]):
        print(f"  v_exact[{i}] = {component:.10f}")

    print("\n" + "=" * 80)
    print("3. ПЕРЕВІРКА: A·v = λ·v")
    print("-" * 80)

    Av_approx = np.dot(A, eigenvector)
    lambda_v_approx = lambda_min * eigenvector
    residual_approx = Av_approx - lambda_v_approx
    residual_norm_approx = np.linalg.norm(residual_approx)

    print("Для наближеного розв'язку:")
    print(f"  ||A·v - λ·v|| = {residual_norm_approx:.10e}")

    Av_exact = np.dot(A, exact_result["eigenvector"])
    lambda_v_exact = exact_result["lambda_min"] * exact_result["eigenvector"]
    residual_exact = Av_exact - lambda_v_exact
    residual_norm_exact = np.linalg.norm(residual_exact)

    print("Для точного розв'язку:")
    print(f"  ||A·v - λ·v|| = {residual_norm_exact:.10e}")

    print("\n" + "=" * 80)
    print("РОБОТУ ЗАВЕРШЕНО")
    print("=" * 80)


if __name__ == "__main__":
    main()
