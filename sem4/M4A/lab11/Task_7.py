import numpy as np
import math
import scipy as sp
from scipy import sparse
import matplotlib.pyplot as plt


a: float = 0
b: float = 3
c: float = 1.875


def K(x: float) -> float:
    if a <= x <= c:
        return 1.5
    else:
        return 0.6


def Q(x: float) -> float:
    if a <= x <= c:
        return 8.3
    else:
        return 12


def F(x: float) -> float:
    return 7*math.e**(-0.5*x)


def get_norm(values: list, step: float) -> float:
    summ: float = 0.0

    for value in values:
        summ += value * value * step

    return math.sqrt(summ)


# initial conditions are "hardcoded" in this solution below:
def get_4_task_solution_with_number_of_subranges(n: int) -> tuple:
    step: float = (b - a) / n

    def y_kprev(x): return K(x)
    def y_k(x): return 2 * step * Q(x)
    def y_knext(x): return -1 * K(x)

    def b_k(x): return 2 * step * F(x)  # for free coefficients vector

    # coefficients matrices with first initial condition already
    free_coefficients: list[float] = [0]
    main_coefficients: list[list[float]] = [[0] * (n + 1)]
    main_coefficients[0][0] = 3 * K(a) + step
    main_coefficients[0][1] = -4 * K(a)
    main_coefficients[0][2] = K(a)

    x_array: list[float] = [a]

    for k in range(1, n):
        new_row: list[float] = [0] * (n + 1)
        new_row[k - 1] = y_kprev(x_array[-1])
        x_array.append(x_array[-1] + step)
        new_row[k] = y_k(x_array[-1])
        new_row[k + 1] = y_knext(x_array[-1] + step)

        main_coefficients.append(new_row)
        free_coefficients.append(b_k(x_array[-1]))

    x_array.append(b)

    main_coefficients.append([0] * (n + 1))
    main_coefficients[-1][-3] = K(b)
    main_coefficients[-1][-2] = -4 * K(b)
    main_coefficients[-1][-1] = 3 * K(b) + step

    free_coefficients.append(0)

    y_array: list[float] = list(np.linalg.solve(
        main_coefficients, free_coefficients))

    plt.plot(x_array, y_array, mew=2, ms=10)
    plt.show()

    return x_array, y_array, get_norm(y_array, step)


def get_4_task_solution(epsilon: float) -> None:
    number_of_ranges: int = 3

    first_solution = get_4_task_solution_with_number_of_subranges(
        number_of_ranges)
    print(number_of_ranges)

    number_of_ranges *= 2
    print(number_of_ranges)

    second_solution = get_4_task_solution_with_number_of_subranges(
        number_of_ranges)
    print(abs(first_solution[2] - second_solution[2]))

    while abs(first_solution[2] - second_solution[2]) > epsilon:
        number_of_ranges *= 2
        first_solution = second_solution
        second_solution = get_4_task_solution_with_number_of_subranges(
            number_of_ranges)
        print(number_of_ranges)
        print(abs(first_solution[2] - second_solution[2]))


def solve_task4() -> None:
    epsilon: float = 0.001
    get_4_task_solution(epsilon)


solve_task4()
