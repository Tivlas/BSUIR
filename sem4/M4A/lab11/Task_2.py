import math

import matplotlib.pyplot as plt
import numpy as np
from scipy import sparse

# вариант
K = 7


def solve_de(px, fx, a, b, n):
    h = (b - a) / n
    X = np.linspace(a, b, n + 1)  # строим сетку иксов
    P = px(X)

    data = np.array(
        [[1] * (n - 1), [-(h * h * P[i] + 2)
                         for i in range(1, n)], [1] * (n - 1)]
    )

    offsets = [-1, 0, 1]

    M = sparse.dia_matrix((data, offsets), shape=(n - 1, n - 1)).toarray()
    M = np.insert(M, 0, np.array([0] * (n - 1)), 0)
    M = np.insert(M, n, np.array([0] * (n - 1)), 0)
    M = np.insert(M, 0, np.array([1, 1] + [0] * (n - 1)), 1)
    M = np.insert(M, n, np.array([0] * (n - 1) + [1, 1]), 1)
    print(M)

    F = fx(X) * h * h
    F[0] = 0
    F[-1] = 0

    print(F)
    solution = np.linalg.solve(M, F)
    return X, solution


def px2(x):
    return -(1 + np.cos(K) * x * x) / np.sin(K)


def fx2(x):
    return (x - x) - 1 / np.sin(K)


nset = [400]

for i in nset:
    X, solution = solve_de(px2, fx2, -1, 1, i)
    plt.plot(X, solution)
    plt.show()
