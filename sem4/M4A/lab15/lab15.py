import numpy as np
import matplotlib.pyplot as plt
import typing as tp
import scipy.sparse as spa
import scipy.sparse.linalg as spa_linalg
from mpl_toolkits.mplot3d import Axes3D
from math import *

VARIANT_NUMBER = 7

width = 180
height = 100
radius = 40
thickness = 4
P = 55
E = 70
v = 0.3
def function(x, y): return P / (E * thickness**3 / (12 * (1 - v**2)))


def solve(
    width: float,
    height: float,
    radius: float,
    function: tp.Callable[[float, float], float],
    step: float,
) -> np.array:
    cols_amount = int(ceil(width / step)) + 1
    rows_amount = int(ceil(height / step)) + 1

    total_equations_amount = cols_amount * rows_amount
    data = []
    i_indexes = []
    j_indexes = []

    b = np.zeros(total_equations_amount)

    def get_mapped_index(i: int, j: int) -> int:
        return cols_amount * i + j

    def is_bound_point(i: int, j: int) -> bool:
        x, y = i * step, j * step
        if np.isclose(x, 0) or np.isclose(y, 0):
            return True
        elif x >= height or y >= width:
            return True
        elif x**2 + (y - width / 2) ** 2 <= radius**2:
            return True
        else:
            return False

    for i in range(rows_amount):
        for j in range(cols_amount):
            if not is_bound_point(i, j):
                data.append(1)
                i_indexes.append(get_mapped_index(i, j))
                j_indexes.append(get_mapped_index(i, j - 1))

                data.append(1)
                i_indexes.append(get_mapped_index(i, j))
                j_indexes.append(get_mapped_index(i, j + 1))

                data.append(1)
                i_indexes.append(get_mapped_index(i, j))
                j_indexes.append(get_mapped_index(i + 1, j))

                data.append(1)
                i_indexes.append(get_mapped_index(i, j))
                j_indexes.append(get_mapped_index(i - 1, j))

                data.append(-4)
                i_indexes.append(get_mapped_index(i, j))
                j_indexes.append(get_mapped_index(i, j))

                b[get_mapped_index(i, j)] = function(
                    i * step, j * step) * step**2
            else:
                data.append(1)
                i_indexes.append(get_mapped_index(i, j))
                j_indexes.append(get_mapped_index(i, j))
    A = spa.coo_matrix((data, (i_indexes, j_indexes)))
    solution = spa_linalg.cg(A, b)[0]
    return solution.reshape((rows_amount, cols_amount))


def plot_solution(solution: np.array, width: float, height: float, step: float) -> None:
    y = np.arange(0, height + step, step)
    x = np.arange(0, width + step, step)
    X, Y = np.meshgrid(x, y)

    fig = plt.figure(figsize=(10, 10))
    ax = plt.axes(projection="3d")
    ax.plot_surface(X, Y, solution, cmap="summer")


# Закомменчены числовые решения, так в отчете они не используются, но если дед захочет, то можно все
solution = solve(width, height, radius, function, 1)
# print(solution)
plot_solution(solution, width, height, 1)

solution = solve(width, height, radius, function, 2)
# print(solution)
plot_solution(solution, width, height, 2)

solution = solve(width, height, radius, function, 10)
# print(solution)
plot_solution(solution, width, height, 10)

# заглушка, чтобы в отладке строились графики(на каждый метод рисования брэкпоинт пихать)
a = width + height
