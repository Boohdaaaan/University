import numpy as np


def gauss_elimination_with_pivoting(
    A: np.ndarray, b: np.ndarray
) -> tuple[np.ndarray, float]:
    n = len(A)
    A = A.copy().astype(float)
    b = b.copy().astype(float)

    sign = 1

    for i in range(n):
        max_row = i
        for k in range(i + 1, n):
            if abs(A[k, i]) > abs(A[max_row, i]):
                max_row = k

        if max_row != i:
            A[[i, max_row]] = A[[max_row, i]]
            b[[i, max_row]] = b[[max_row, i]]
            sign *= -1

        if abs(A[i, i]) < 1e-10:
            raise ValueError("Матриця вироджена або близька до виродженої")

        for k in range(i + 1, n):
            factor = A[k, i] / A[i, i]
            A[k, i:] -= factor * A[i, i:]
            b[k] -= factor * b[i]

    determinant = sign
    for i in range(n):
        determinant *= A[i, i]

    x = np.zeros(n)
    for i in range(n - 1, -1, -1):
        x[i] = (b[i] - np.dot(A[i, i + 1 :], x[i + 1 :])) / A[i, i]

    return x, determinant


def matrix_infinity_norm(A: np.ndarray) -> float:
    """Обчислює L∞ норму матриці (максимальна сума модулів елементів рядка)."""
    return np.max(np.sum(np.abs(A), axis=1))


def condition_number(A: np.ndarray) -> float:
    A_inv = np.linalg.inv(A)
    return matrix_infinity_norm(A) * matrix_infinity_norm(A_inv)


def check_diagonal_dominance(A: np.ndarray) -> bool:
    n = len(A)
    for i in range(n):
        row_sum = np.sum(np.abs(A[i])) - np.abs(A[i, i])
        if np.abs(A[i, i]) <= row_sum:
            return False
    return True


def jacobi_method(
    A: np.ndarray, b: np.ndarray, tol: float = 1e-6, max_iter: int = 1000
) -> tuple[np.ndarray, dict]:
    n = len(A)
    x = np.zeros(n)
    x_old = np.zeros(n)

    info = {
        "first_iter_norm": None,
        "last_iter_norm": None,
        "iterations": 0,
        "converged": False,
    }

    for iteration in range(max_iter):
        x_old = x.copy()

        for i in range(n):
            sigma = np.sum(A[i, :i] * x_old[:i]) + np.sum(
                A[i, i + 1 :] * x_old[i + 1 :]
            )
            x[i] = (b[i] - sigma) / A[i, i]

        if np.any(np.isinf(x)) or np.any(np.isnan(x)):
            info["last_iter_norm"] = np.inf
            break

        diff_norm = np.max(np.abs(x - x_old))

        if iteration == 0:
            info["first_iter_norm"] = diff_norm

        info["last_iter_norm"] = diff_norm
        info["iterations"] = iteration + 1

        if diff_norm < tol:
            info["converged"] = True
            break

        if iteration > 10 and diff_norm > 1e10:
            break

    return x, info


def main():
    A = np.array([[3.3, 2.1, 2.8], [4.1, 3.7, 4.8], [2.7, 1.8, 1.1]])

    b = np.array([0.8, 5.7, 3.2])

    print("=" * 60)
    print("Лабораторна робота 2: Розв'язання СЛАР двома методами")
    print("=" * 60)
    print("\nСистема рівнянь:")
    print("3.3x₁ + 2.1x₂ + 2.8x₃ = 0.8")
    print("4.1x₁ + 3.7x₂ + 4.8x₃ = 5.7")
    print("2.7x₁ + 1.8x₂ + 1.1x₃ = 3.2")
    print("\n" + "=" * 60)

    print("\n1. ПРЯМИЙ МЕТОД (Метод Гаусса з частковим вибором головного елемента)")
    print("-" * 60)

    try:
        x_gauss, det = gauss_elimination_with_pivoting(A, b)
        cond = condition_number(A)

        print(f"Визначник матриці: {det:.10f}")
        print(f"Число обумовленості: {cond:.10f}")
        print("\nРозв'язок (метод Гаусса):")
        print(f"x₁ = {x_gauss[0]:.10f}")
        print(f"x₂ = {x_gauss[1]:.10f}")
        print(f"x₃ = {x_gauss[2]:.10f}")

    except ValueError as e:
        print(f"Помилка: {e}")
        x_gauss = None

    print("\n" + "=" * 60)
    print("\n2. МЕТОД ЯКОБІ")
    print("-" * 60)

    is_diagonally_dominant = check_diagonal_dominance(A)
    print(f"Діагональна перевага: {'Так' if is_diagonally_dominant else 'Ні'}")

    if not is_diagonally_dominant:
        print(
            "Увага: достатня умова збіжності не виконується, але метод може збігатися."
        )

    x_jacobi, info = jacobi_method(A, b)

    print(f"\nКількість ітерацій: {info['iterations']}")
    print(f"Збіжність: {'Так' if info['converged'] else 'Ні'}")
    print(f"\nНорма різниці векторів (L∞):")
    print(f"  Перша ітерація: {info['first_iter_norm']:.10f}")
    print(f"  Остання ітерація: {info['last_iter_norm']:.10f}")

    print("\nРозв'язок (метод Якобі):")
    print(f"x₁ = {x_jacobi[0]:.10f}")
    print(f"x₂ = {x_jacobi[1]:.10f}")
    print(f"x₃ = {x_jacobi[2]:.10f}")

    print("\n" + "=" * 60)
    print("\nРЕЗУЛЬТАТИ:")
    print("=" * 60)

    print("\nРозв'язок системи:")
    if x_gauss is not None:
        print(f"x₁ = {x_gauss[0]:.10f}")
        print(f"x₂ = {x_gauss[1]:.10f}")
        print(f"x₃ = {x_gauss[2]:.10f}")
    else:
        print(f"x₁ = {x_jacobi[0]:.10f}")
        print(f"x₂ = {x_jacobi[1]:.10f}")
        print(f"x₃ = {x_jacobi[2]:.10f}")

    print("\nМетод Якобі - норма різниці векторів (L∞):")
    print(f"  Перша ітерація: {info['first_iter_norm']:.10f}")
    print(f"  Остання ітерація: {info['last_iter_norm']:.10f}")


if __name__ == "__main__":
    main()
