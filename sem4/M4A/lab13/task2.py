# importing necessary libraries
import math
import sympy as sp
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

x, t = sp.symbols("x t")

ua, ub = 1.5, 2.5
ya, yb = 3, -3
k1, k2, k3 = 80, 1, 20
x0, c = ua + (ub - ua) / 2, 100
h = (ub - ua) / 150
k = sp.Piecewise(
    (k1, x < ua + (ub - ua) / 3),
    (k3, ua + 2 * (ub - ua) / 3 <= x),
    (k2, True),
)
source = [(100, ua + (ub - ua) / 2)]

# integral for delta function


def phi(x, x0, c):
    if abs(x - x0) - h / 2 < 1e-5:
        return c / 2
    elif 2 * x - h < 2 * x0 < 2 * x + h:
        return c
    else:
        return 0


def task2(a, b, ya, yb, h, phi, k_expr, sources):
    l, r = sp.symbols("l r")
    ab = sp.lambdify((l, r), h * (sp.integrate(1 / k_expr, (x, l, r))) ** (-1))
    n = int((b - a) / h) + 1

    matrix = np.zeros(shape=(n, n))
    t = np.zeros(shape=(n, 1))
    xs = np.linspace(a, b, n)
    matrix[0, 0] = matrix[-1, -1] = 1
    t[0] = ya
    t[-1] = yb

    for i in range(1, n - 1):
        matrix[i, i - 1] = ab(xs[i - 1], xs[i])
        matrix[i, i] = -ab(xs[i - 1], xs[i]) - ab(xs[i], xs[i + 1])
        matrix[i, i + 1] = ab(xs[i], xs[i + 1])
        t[i] = -h * sum(phi(xs[i], x0i, ci) for ci, x0i in sources)

    return xs, np.linalg.solve(matrix, t)


xs, ys = task2(ua, ub, ya, yb, h, phi, k, source)

plt.plot(xs, ys)
plt.grid()
plt.show()

ua, ub = 0, 1
ya, yb = 0, 0
x0, c = ua + (ub - ua) / 2, 100
h = (ub - ua) / 150

sources = [
    [(10, ua + (ub - ua) / 2)],
    [(10, ua + (ub - ua) / 4), (10, ua + 3 * (ub - ua) / 4)],
    [(10, ua + (ub - ua) / 4), (50, ua + 3 * (ub - ua) / 4)],
]

# for two k's
for k1, k2 in [[1, 100], [100, 1]]:
    k = sp.Piecewise(
        (k1, x < ua + (ub - ua) / 2),
        (k2, True),
    )
    for source in sources:
        xs, ys = task2(ua, ub, ya, yb, h, phi, k, source)
        plt.plot(xs, ys, label="{} {} {}".format(k1, k2, source))
        plt.legend()
        plt.grid()
        plt.show()

for k1, k2, k3 in [[1, 3, 9], [9, 3, 1], [1, 2, 1], [20, 1, 20]]:
    k = sp.Piecewise(
        (k1, x < ua + (ub - ua) / 3),
        (k3, ua + 2 * (ub - ua) / 3 <= x),
        (k2, True),
    )
    for source in sources:
        xs, ys = task2(ua, ub, ya, yb, h, phi, k, source)
        plt.plot(xs, ys, label="{} {} {} {}".format(k1, k2, k3, source))
        plt.legend()
        plt.grid()
        plt.show()

# set what consists of k1, k2, k3, xr1, xr2
a, b = 0.2, 1.2
sets = [
    [1, 100, 0, (b + a) / 2, 2 * b],
    [100, 1, 0, (b + a) / 2, 2 * b],
    [10, 25, 75, a + (b - a) / 3, a + 2 * (b - a) / 3],
    [100, 40, 15, a + (b - a) / 3, a + 2 * (b - a) / 3],
    [30, 60, 30, a + (b - a) / 3, a + 2 * (b - a) / 3],
    [100, 5, 100, a + (b - a) / 3, a + 2 * (b - a) / 3],
]

a, b = 0.2, 1.2
conditions = [
    [((b + a) / 2, 20)],
    [((b + a) * (0.5 - 0.3), 20), ((b + a) * (0.5 + 0.3), 20)],
    [((b + a) * (0.5 - 0.3), 20), ((b + a) * (0.5 + 0.3), 200)],
]

for s in sets:
    for conds in conditions:
        plt.plot(task2(a, b, ua, ub, h, *s, conds),
                 label="{}, {}, {}".format(*s))
        plt.plot()
        plt.legend()
        plt.show()
