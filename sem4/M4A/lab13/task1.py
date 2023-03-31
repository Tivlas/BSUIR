from numpy import linspace
from matplotlib import pyplot as plt
from scipy import integrate


def tridiag_solve(a, b, c, d):
    alpha = [-c[0] / b[0]]
    beta = [d[0] / b[0]]
    gamma = [b[0]]
    n = len(d)
    result = [0 for i in range(n)]

    for i in range(1, n - 1):
        gamma.append(b[i] + a[i] * alpha[i - 1])
        alpha.append(-c[i] / gamma[i])
        beta.append((d[i] - a[i] * beta[i - 1]) / gamma[i])

    gamma.append(b[n - 1] + a[n - 1] * alpha[n - 2])
    beta.append((d[n - 1] - a[n - 1] * beta[n - 2]) / gamma[n - 1])
    result[n - 1] = beta[n - 1]

    for i in reversed(range(n - 1)):
        result[i] = alpha[i] * result[i + 1] + beta[i]

    return result


def get_c(x1, x2, k_x):
    return (integrate.quad(lambda x: 1 / k_x(x), x1, x2)[0]) ** (-1)


def get_phi(x1, x2, f):
    return integrate.quad(f, x1, x2)[0]


n = 100
A = 1
B = 2
Ua = 3
Ub = 3


def task1_1_3():
    k_x_variants = [
        [lambda x: x, 1],
        [lambda x: 2 * x, 2],
        [lambda x: 0.1 * x, 3]
    ]

    def f(x): return 3*x + x**2

    for k_x, number in k_x_variants:
        result = []

        h = (B - A) / n
        points = list(linspace(A, B, n))

        a = []
        b = []
        c = []
        d = []

        for i in range(1, n - 1):
            a.append(get_c(points[i - 1], points[i], k_x))
            b.append(-get_c(points[i - 1], points[i], k_x) -
                     get_c(points[i], points[i + 1], k_x))
            c.append(get_c(points[i], points[i + 1], k_x))
            d.append(-get_phi(points[i] - h / 2, points[i] + h / 2, f))

        d[0] = d[0] - a[0] * Ua
        d[-1] = d[-1] - c[-1] * Ub
        a[0] = 0
        c[-1] = 0

        result = tridiag_solve(a, b, c, d)
        result.insert(0, Ua)
        result.append(Ub)

        plt.plot(points, result, label=f'Set #{number}')

    plt.legend()
    plt.show()


def task1_4():
    k_x_variants = [
        [lambda x: x, 1],
        [lambda x: 1 / x, 4]
    ]

    def f(x): return 3*x + x**2

    for k_x, number in k_x_variants:
        result = []

        h = (B - A) / n
        points = list(linspace(A, B, n))

        a = []
        b = []
        c = []
        d = []

        for i in range(1, n - 1):
            a.append(get_c(points[i - 1], points[i], k_x))
            b.append(-get_c(points[i - 1], points[i], k_x) -
                     get_c(points[i], points[i + 1], k_x))
            c.append(get_c(points[i], points[i + 1], k_x))
            d.append(-get_phi(points[i] - h / 2, points[i] + h / 2, f))

        d[0] = d[0] - a[0] * Ua
        d[-1] = d[-1] - c[-1] * Ub
        a[0] = 0
        c[-1] = 0

        result = tridiag_solve(a, b, c, d)
        result.insert(0, Ua)
        result.append(Ub)

        plt.plot(points, result, label=f'Set #{number}')

    plt.legend()
    plt.show()


def task1_5_7():
    u_variants = [
        [-3, 3, 5],
        [3, -3, 6],
        [-3, -3, 7]
    ]

    def k_x(x): return x
    def f(x): return 3*x + x**2

    for Ua, Ub, number in u_variants:
        result = []

        h = (B - A) / n
        points = list(linspace(A, B, n))

        a = []
        b = []
        c = []
        d = []

        for i in range(1, n - 1):
            a.append(get_c(points[i - 1], points[i], k_x))
            b.append(-get_c(points[i - 1], points[i], k_x) -
                     get_c(points[i], points[i + 1], k_x))
            c.append(get_c(points[i], points[i + 1], k_x))
            d.append(-get_phi(points[i] - h / 2, points[i] + h / 2, f))

        d[0] = d[0] - a[0] * Ua
        d[-1] = d[-1] - c[-1] * Ub
        a[0] = 0
        c[-1] = 0

        result = tridiag_solve(a, b, c, d)
        result.insert(0, Ua)
        result.append(Ub)

        plt.plot(points, result, label=f'Set #{number}')

    plt.legend()
    plt.show()


task1_1_3()
task1_4()
task1_5_7()
