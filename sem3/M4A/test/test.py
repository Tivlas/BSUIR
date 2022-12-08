import numpy as np


def input():
    x = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]
    p = [0.0, 0.41, 0.79, 1.13, 1.46, 1.76, 2.04, 2.3, 2.55, 2.79, 3.01]
    k = 3
    m = 1.5
    y = [(p[i] + ((-1) ** k) * m) for i in range(len(x))]
    dots = list(zip(x, y))

    # dots = [(0, 0), (1, 1)] # Пример 1
    # dots = [(-2, 3), (-1, 0), (0, -1), (1, 0), (2, 3)] # Пример 2
    # dots = [(1, 1 / 1), (2, 1 / 2), (3, 1 / 3), (4, 1 / 4), (5, 1 / 5)]  # Пример 3
    # dots = list(
    #     zip([-2, -1, 0, 1, 2], [np.tanh(i) for i in [-2, -1, 0, 1, 2]]),
    # )  # Пример 4

    return dots

dots = input()
print(dots)
(x, y) = map(list, zip(*dots))
#print("(x,y) =", dots, "\n")

polynom = np.poly1d([3])
print(polynom)
