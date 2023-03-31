# importing necessary libraries
import math
import sympy as sp
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

x, t = sp.symbols("x t")


def task4(a, b, k, T, phi, g1, g2, f):
    x_h = (b - a) / 50
    x_h_step_amount = int((b - a) / x_h) + 1
    t_h = 0.5 * x_h**2 / k
    t_h_step_amount = int(T / t_h) + 1
    x_hs = np.linspace(a, b, x_h_step_amount)
    t_hs = np.linspace(0, T, t_h_step_amount)

    matrix = np.zeros(shape=(t_h_step_amount, x_h_step_amount))

    # initial condition
    matrix[0, 1:-1] = np.array(
        [phi(x_hs[i], t_hs[0]) for i in range(1, x_h_step_amount - 1)]
    )

    # bounds condition
    matrix[:, 0] = np.array([g1(x_hs[0], t_hs[i])
                            for i in range(t_h_step_amount)])
    matrix[:, -1] = np.array([g2(x_hs[-1], t_hs[i])
                             for i in range(t_h_step_amount)])

    coef = np.array(
        [k * t_h / x_h**2, 1 - 2 * k * t_h / x_h**2, k * t_h / x_h**2]
    )

    for i in range(1, t_h_step_amount):
        for j in range(1, x_h_step_amount - 1):
            matrix[i][j] = matrix[i - 1, j - 1: j + 2].dot(coef) + t_h * f(
                x_hs[j], t_hs[i - 1]
            )

    # plotting
    ax = plt.axes(projection="3d")
    ax.set_ylabel("$T$ time axis")
    ax.set_xlabel("$X$ spatial axis")
    ax.set_zlabel("$Y$ function value axis")

    for i in range(0, t_h_step_amount, 130):
        ax.plot3D(x_hs, np.array([t_hs[i]] * x_h_step_amount), matrix[i, :])

    plt.show()


k = 2
T = 0.8
a, b = 0, 1
phi = sp.lambdify((x, t), 0)
g1 = sp.lambdify((x, t), 0)
g2 = sp.lambdify((x, t), sp.sin(10*t))
f = sp.lambdify((x, t), x*(1-x))

task4(a, b, k, T, phi, g1, g2, f)
