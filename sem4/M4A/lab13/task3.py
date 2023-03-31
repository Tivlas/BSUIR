# importing necessary libraries
import math
import sympy as sp
import numpy as np
import matplotlib.pyplot as plt

# from mpl_toolkits import mplot3d

x, t = sp.symbols("x t")


def task3(x_h, t_h, a, b, k, T, g1, g2, phi, f):
    x_h_step_amount = int((b - a) / x_h) + 1
    t_h_step_amount = int(T / t_h) + 1
    x_hs = np.linspace(a, b, x_h_step_amount)
    t_hs = np.linspace(0, T, t_h_step_amount)

    matrix = np.zeros(shape=(t_h_step_amount, x_h_step_amount))

    # initial condition
    matrix[0, 1:-1] = np.array([phi(x_hs[i])
                               for i in range(1, x_h_step_amount - 1)])

    # bounds condition
    matrix[:, 0] = np.array([g1(x_hs[0], t_hs[i])
                            for i in range(t_h_step_amount)])
    matrix[:, -1] = np.array([g2(x_hs[-1], t_hs[i])
                             for i in range(t_h_step_amount)])

    for i in range(1, t_h_step_amount):
        for j in range(1, x_h_step_amount - 1):
            matrix[i, j] = sum(
                [
                    k(x_hs[j] - x_h / 2) * t_h / x_h**2 * matrix[i - 1, j - 1],
                    (1 - (k(x_hs[j] - x_h / 2) +
                     k(x_hs[j] + x_h / 2)) * t_h / x_h**2)
                    * matrix[i - 1, j],
                    k(x_hs[j] + x_h / 2) * t_h / x_h**2 * matrix[i - 1, j + 1],
                    t_h * f(x_hs[j], t_hs[i]) * (1 - math.exp(-t_hs[i])),
                ]
            )

    # plotting
    ax = plt.axes(projection="3d")
    ax.set_ylabel("$T$ time axis")
    ax.set_xlabel("$X$ spatial axis")
    ax.set_zlabel("$Y$ function value axis")

    for i in range(0, t_h_step_amount, 100):
        ax.plot3D(x_hs, np.array([t_hs[i]] * x_h_step_amount), matrix[i, :])
    plt.show()


a, b = 1, 2
g1 = sp.lambdify((x, t), 3)
g2 = sp.lambdify((x, t), 3)
f = sp.lambdify((x, t), 3.0*x + x**2)  # сюда функцию f(x)
k = sp.lambdify(x, x)  # сюда k(x)
phi = sp.lambdify(
    x,
    # x**2
    # - 3 * x
    # + 5
    3
)


h_x, h_t = 0.05, 0.0005
T = 6500 * h_t

task3(h_x, h_t, a, b, k, T, g1, g2, phi, f)
