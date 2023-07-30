import numpy as np
from matplotlib import pyplot as plt


L = 0.18
du = 0.002
E = 120e9
rho = 59e2
T = 0.0001


def solve(N, M, build_plot=False, count=0):
    f = lambda x: (-4 * du / L**2) * x**2 + (4 * du / L) * x

    tau = T / M
    h = L / N
    C = np.sqrt(E / rho) * tau / h

    x_values = np.linspace(0, L, N)
    matrix = np.zeros((M, N))

    matrix[0] = [f(x) for x in x_values]
    matrix[1][1:-1] = [f(x_values[i]) * (1 - tau**2 / 2) for i in range(1, N - 1)]

    for i in range(1, M - 1):
        matrix[i + 1][1:-1] = C**2 * (
            matrix[i][2:] - 2 * matrix[i][1:-1] + matrix[i][:-2]
        )
        matrix[i + 1][1:-1] += 2 * matrix[i][1:-1] - matrix[i - 1][1:-1]

    if build_plot:
        for i in range(0, M, int(M / count)):
            plt.plot(x_values, matrix[i])
            plt.xlabel("x")
            plt.ylabel("u(x, t)")

        plt.grid()
        plt.show()


N = 100
M = 10000
A = solve(N, M, build_plot=True, count=15)
