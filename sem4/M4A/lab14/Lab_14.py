from matplotlib import cm
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sympy as sp

# initial data variant 10
x, t = sp.symbols("x t")
# initial condition
a, b = 0, 1
k = 2  # 0.2
T = 0.02  # 1
# steps size
h_x = 0.02
h_t = 0.5 * h_x**2 / k
# expression of initial function
phi_expr = 0
g1_expr = 0
g2_expr = sp.sin(10*t)
f_expr = x*(1-x)
# initial functions
phi = sp.lambdify(x, phi_expr)
g1 = sp.lambdify(t, g1_expr)
g2 = sp.lambdify(t, g2_expr)
f = sp.lambdify((x, t), f_expr)

display = pd.options.display


def plot_3d(xs, ts, Z):
    X, Y = np.meshgrid(xs, ts)

    fig = plt.figure()
    ax = fig.add_subplot(projection="3d")

    # surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm, linewidth=0, antialiased=False)
    surf = ax.plot_surface(X, Y, Z, cmap=cm.coolwarm, linewidth=2)

    fig.colorbar(surf, shrink=0.5, aspect=5)

    plt.show()


def plot_2d(xs, matrix_ys, step):
    for i in range(0, len(matrix_ys), step):
        plt.plot(xs, matrix_ys[i])

    plt.grid()
    plt.show()


def explicit_method(a, b, k, T, phi, g1, g2, f, h_x, h_t):
    hx_step_amount = int((b - a) / h_x)
    ht_step_amount = int(T / h_t)

    h_x = (b - a) / hx_step_amount
    h_t = T / ht_step_amount

    h_xs = np.linspace(a, b, hx_step_amount)
    h_ts = np.linspace(0, T, ht_step_amount)

    matrix = np.zeros(shape=(ht_step_amount, hx_step_amount))

    matrix[0, 1:-1] = np.array([phi(hs) for hs in h_xs[1:-1]])
    matrix[:, 0] = np.array([g1(ht) for ht in h_ts])
    #     matrix[:, -1] = np.array([g2(h_ts[i]) * h_t + matrix[i-1, -1] for i in range(len(h_ts))])

    coefs = [
        k * h_t / h_x**2,
        1 - 2 * k * h_t / h_x**2,
        k * h_t / h_x**2,
    ]

    for i in range(1, ht_step_amount):
        prev_layer = matrix[i - 1]
        matrix[i, 1:-1] = sum(
            coefs[i] * prev_layer[i: len(prev_layer) - 2 + i] for i in range(3)
        ) + np.array(  # sum yn-1 yn yn+1
            [h_t * f(hx, h_ts[i - 1]) for hx in h_xs[1:-1]]
        )  # vector of tau * f(x, t)
        matrix[i, -1] = matrix[i, -2] + h_x * g2(h_ts[i])

    # plot_2d(h_xs, matrix, 5)
    # plot_3d(h_xs, h_ts, matrix)

    return matrix


exp_res1 = explicit_method(a, b, k, T, phi, g1, g2, f, h_x, h_t)


def explicit_method_2(a, b, k, T, phi, g1, g2, f, h_x, h_t):
    hx_step_amount = int((b - a) / h_x) + 1
    ht_step_amount = int(T / h_t) + 1

    h_xs = np.linspace(a, b, hx_step_amount)
    h_ts = np.linspace(0, T, ht_step_amount)

    matrix = np.zeros(shape=(ht_step_amount, hx_step_amount))

    matrix[0, 1:-1] = np.array([phi(hs) for hs in h_xs[1:-1]])
    matrix[:, 0] = np.array([g1(ht) for ht in h_ts])
    #     matrix[:, -1] = np.array([g2(h_ts[i]) * h_t + matrix[i-1, -1] for i in range(len(h_ts))])

    coefs = [
        k * h_t / h_x**2,
        1 - 2 * k * h_t / h_x**2,
        k * h_t / h_x**2,
    ]

    for i in range(1, ht_step_amount):
        prev_layer = matrix[i - 1]
        matrix[i, 1:-1] = sum(
            coefs[i] * prev_layer[i: len(prev_layer) - 2 + i] for i in range(3)
        ) + np.array(  # sum yn-1 yn yn+1
            [h_t * f(hx, h_ts[i - 1]) for hx in h_xs[1:-1]]
        )  # vector of tau * f(x, t)
        matrix[i, -1] = (
            2 * coefs[0] * matrix[i - 1, -2]
            + coefs[1] * matrix[i - 1, -1]
            + h_t * f(b, h_ts[i])
            + 2 * h_x * coefs[0] * g2(h_ts[i])
        )

    plot_2d(h_xs, matrix, 5)
    plot_3d(h_xs, h_ts, matrix)

    return matrix


exp_res2 = explicit_method_2(a, b, k, T, phi, g1, g2, f, h_x, h_t)


def implicit_method(a, b, k, T, phi, g1, g2, f, h_x, h_t):
    hx_step_amount = int((b - a) / h_x) + 1
    ht_step_amount = int(T / h_t) + 1

    h_xs = np.linspace(a, b, hx_step_amount)
    h_ts = np.linspace(0, T, ht_step_amount)

    matrix = np.zeros(shape=(ht_step_amount, hx_step_amount))

    matrix[0, 1:-1] = np.array([phi(hs) for hs in h_xs[1:-1]])
    matrix[:, 0] = np.array([g1(ht) for ht in h_ts])
    matrix[:, -1] = np.array([g2(ht) * h_t for ht in h_ts])

    coefs = [
        -k * h_t / h_x**2,
        1 + 2 * k * h_t / h_x**2,
        -k * h_t / h_x**2,
    ]

    coefs_matrix = np.zeros(shape=(hx_step_amount, hx_step_amount))
    # fill coefs matrix
    np.fill_diagonal(coefs_matrix, coefs[1])
    np.fill_diagonal(coefs_matrix[1:], coefs[0])
    np.fill_diagonal(coefs_matrix[:, 1:], coefs[2])
    # fill border condition coefs matrix
    coefs_matrix[0, 0] = 1
    coefs_matrix[0, 1] = 0
    coefs_matrix[-1, -2] = -1
    coefs_matrix[-1, -1] = 1

    for i in range(1, ht_step_amount):
        rhs = np.array([h_t * f(hx, h_ts[i]) for hx in h_xs]) + matrix[i - 1]
        rhs[0] = g1(h_ts[i])
        rhs[-1] = h_x * g2(h_ts[i])
        matrix[i] = np.linalg.solve(coefs_matrix, rhs)

    # plot_2d(h_xs, matrix, 5)
    # plot_3d(h_xs, h_ts, matrix)

    return matrix


imp_res1 = implicit_method(a, b, k, T, phi, g1, g2, f, h_x, h_t)


def implicit_method_2(a, b, k, T, phi, g1, g2, f, h_x, h_t):
    hx_step_amount = int((b - a) / h_x) + 1
    ht_step_amount = int(T / h_t) + 1

    h_xs = np.linspace(a, b, hx_step_amount)
    h_ts = np.linspace(0, T, ht_step_amount)

    matrix = np.zeros(shape=(ht_step_amount, hx_step_amount))

    matrix[0, 1:-1] = np.array([phi(hs) for hs in h_xs[1:-1]])
    matrix[:, 0] = np.array([g1(ht) for ht in h_ts])
    matrix[:, -1] = np.array([g2(ht) * h_t for ht in h_ts])

    coefs = [
        -k * h_t / h_x**2,
        1 + 2 * k * h_t / h_x**2,
        -k * h_t / h_x**2,
    ]

    coefs_matrix = np.zeros(shape=(hx_step_amount, hx_step_amount))
    # fill coefs matrix
    np.fill_diagonal(coefs_matrix, coefs[1])
    np.fill_diagonal(coefs_matrix[1:], coefs[0])
    np.fill_diagonal(coefs_matrix[:, 1:], coefs[2])
    # fill border condition coefs matrix
    coefs_matrix[0, 0] = 1
    coefs_matrix[0, 1] = 0
    coefs_matrix[-1, -2] = 2 * coefs[0]

    # rhs = np.zeros(shape=(1, hx_step_amount))

    for i in range(1, ht_step_amount):
        rhs = np.array([h_t * f(hx, h_ts[i]) for hx in h_xs]) + matrix[i - 1]
        rhs[0] = g1(h_ts[i])
        rhs[-1] += -2 * h_x * coefs[0] * g2(b)
        matrix[i] = np.linalg.solve(coefs_matrix, rhs)

    plot_2d(h_xs, matrix, 5)
    plot_3d(h_xs, h_ts, matrix)

    return matrix


imp_res2 = implicit_method_2(a, b, k, T, phi, g1, g2, f, h_x, h_t)

# initial condition
# a, b = 0, 1
# k = 1
# T = 0.05
# # expression of initial function
# phi_expr = 0
# g1_expr = 0
# g2_expr = 0
# f_expr = x
# # initial functions
# phi = sp.lambdify(x, phi_expr)
# g1 = sp.lambdify(t, g1_expr)
# g2 = sp.lambdify(t, g2_expr)
# f = sp.lambdify((x, t), f_expr)

a, b = 0, 1
k = 2  # 0.2
T = 0.02  # 1
# steps size
h_x = 0.02
h_t = 0.5 * h_x**2 / k
# expression of initial function
phi_expr = 0
g1_expr = 0
g2_expr = sp.sin(10*t)
f_expr = x*(1-x)
# initial functions
phi = sp.lambdify(x, phi_expr)
g1 = sp.lambdify(t, g1_expr)
g2 = sp.lambdify(t, g2_expr)
f = sp.lambdify((x, t), f_expr)

mantissa_len = 10

# standard deviation


def SD_t(m1, m2, layer1, layer2):
    if len(m1[0]) > len(m2[0]):
        m1, m2 = m2, m1
        layer1, layer2 = layer2, layer1
    return np.sqrt(
        sum((m1[layer1][i] - m2[layer2][2 * i])
            ** 2 for i in range(len(m1[0])))
        / len(m1[0])
    )


def max_deviation_t(m1, m2, layer1, layer2):
    if len(m1[0]) > len(m2[0]):
        m1, m2 = m2, m1
        layer1, layer2 = layer2, layer1
    return max(np.abs([m1[layer1][i] - m2[layer2][2 * i] for i in range(len(m1[0]))]))


Ns = np.array([5 * 2**i for i in range(4, -1, -1)])

table = np.zeros(shape=(len(Ns), 7))

# select randomly t1 and t2
# layer1 = random.randint(1, Ns[-1] - 1)
# layer2 = random.randint(1, Ns[-1] - 1)
layer1 = 3
layer2 = 4

prev_res = None

print("t1 = {}, t2 = {}".format(T / Ns[-1] * layer1, T / Ns[-1] * layer2))

Xs = np.linspace(a, b, Ns[0] + 1)
tau = 0.5 * (Xs[1] - Xs[0]) ** 2 / k

for i, N in enumerate(Ns):
    h_x = (b - a) / N
    res = explicit_method(a, b, T, k, phi, g1, g2, f, h_x, tau)
    if prev_res is None:
        table[i][0] = Ns[i]
        table[i][1] = Xs[1] - Xs[0]
        table[i][2] = round(tau, mantissa_len)
        table[i][3] = 0
        table[i][4] = 0
        table[i][5] = 0
        table[i][6] = 0
    else:
        # compute indexes of t1 and t2
        t11 = layer1 * Ns[i - 1] // Ns[-1]
        t12 = layer1 * Ns[i] // Ns[-1]
        t21 = layer2 * Ns[i - 1] // Ns[-1]
        t22 = layer2 * Ns[i] // Ns[-1]
        table[i][0] = Ns[i]
        table[i][1] = h_x
        table[i][2] = tau
        table[i][3] = SD_t(prev_res, res, t11, t12)
        table[i][4] = SD_t(prev_res, res, t21, t22)
        table[i][5] = max_deviation_t(prev_res, res, t11, t12)
        table[i][6] = max_deviation_t(prev_res, res, t21, t22)
    prev_res = res

df = pd.DataFrame(
    table,
    columns=[
        "N",
        "N h",
        "t",
        "s(t=t_n1)",
        "s(t=t_n2)",
        "max|t=t_n1|",
        "max|t=t_n2|",
    ],
)

print(df)


table = np.zeros(shape=(len(Ns), 7))

# select randomly t1 and t2
# layer1 = random.randint(1, Ns[-1] - 1)
# layer2 = random.randint(1, Ns[-1] - 1)
layer1 = 3
layer2 = 4
prev_res = None

print("t1 = {}, t2 = {}".format(T / Ns[-1] * layer1, T / Ns[-1] * layer2))

Xs = np.linspace(a, b, Ns[0] + 1)
tau = 0.5 * (Xs[1] - Xs[0]) ** 2 / k

for i, N in enumerate(Ns):
    h_x = (b - a) / N
    res = implicit_method(a, b, T, k, phi, g1, g2, f, h_x, tau)
    if prev_res is None:
        table[i][0] = Ns[i]
        table[i][1] = Xs[1] - Xs[0]
        table[i][2] = round(tau, mantissa_len)
        table[i][3] = 0
        table[i][4] = 0
        table[i][5] = 0
        table[i][6] = 0
    else:
        # compute indexes of t1 and t2
        t11 = layer1 * Ns[i - 1] // Ns[-1]
        t12 = layer1 * Ns[i] // Ns[-1]
        t21 = layer2 * Ns[i - 1] // Ns[-1]
        t22 = layer2 * Ns[i] // Ns[-1]
        table[i][0] = Ns[i]
        table[i][1] = h_x
        table[i][2] = tau
        table[i][3] = SD_t(prev_res, res, t11, t12)
        table[i][4] = SD_t(prev_res, res, t21, t22)
        table[i][5] = max_deviation_t(prev_res, res, t11, t12)
        table[i][6] = max_deviation_t(prev_res, res, t21, t22)
    prev_res = res

df = pd.DataFrame(
    table,
    columns=[
        "N",
        "N h",
        "t",
        "s(t=t_n1)",
        "s(t=t_n2)",
        "max|t=t_n1|",
        "max|t=t_n2|",
    ],
)

print(df)

# standard deviation


def SD_h(m1, m2, layer1, layer2):
    return np.sqrt(
        sum((m1[layer1][i] - m2[layer2][i]) ** 2 for i in range(len(m1[0])))
        / len(m1[0])
    )


def max_deviation_h(m1, m2, layer1, layer2):
    return max(np.abs([m1[layer1][i] - m2[layer2][i] for i in range(len(m1[0]))]))


Ts = np.array([100 * 2**i for i in range(5)])


table = np.zeros(shape=(len(Ns), 7))

max_tau = T / Ts[0]
h_x = np.sqrt(2 * k * max_tau)

# select randomly t1 and t2
# layer1 = random.randint(1, Ts[0] - 1)
# layer2 = random.randint(1, Ts[0] - 1)
layer1 = 3
layer2 = 4

prev_res = None

print("t1 = {}, t2 = {}".format(T / Ts[0] * layer1, T / Ts[0] * layer2))


for i, h_t in enumerate(Ts):
    tau = T / h_t
    res = explicit_method(a, b, T, k, phi, g1, g2, f, h_x, tau)
    if prev_res is None:
        table[i][0] = Ts[i]
        table[i][1] = h_x
        table[i][2] = round(tau, mantissa_len)
        table[i][3] = 0
        table[i][4] = 0
        table[i][5] = 0
        table[i][6] = 0
    else:
        # compute indexes of t1 and t2
        t11 = layer1 * Ts[i - 1] // Ts[0]
        t12 = layer1 * Ts[i] // Ts[0]
        t21 = layer2 * Ts[i - 1] // Ts[0]
        t22 = layer2 * Ts[i] // Ts[0]
        table[i][0] = Ts[i]
        table[i][1] = h_x
        table[i][2] = tau
        table[i][3] = SD_h(prev_res, res, t11, t12)
        table[i][4] = SD_h(prev_res, res, t21, t22)
        table[i][5] = max_deviation_h(prev_res, res, t11, t12)
        table[i][6] = max_deviation_h(prev_res, res, t21, t22)
    prev_res = res


df = pd.DataFrame(
    table,
    columns=[
        "N",
        "N h",
        "t",
        "s(t=t_n1)",
        "s(t=t_n2)",
        "max|t=t_n1|",
        "max|t=t_n2|",
    ],
)

print(df)

table = np.zeros(shape=(len(Ns), 7))

max_tau = T / Ts[0]
h_x = np.sqrt(2 * k * max_tau)

# select randomly t1 and t2
# layer1 = random.randint(1, Ts[0] - 1)
# layer2 = random.randint(1, Ts[0] - 1)
layer1 = 3
layer2 = 4

prev_res = None

print("t1 = {}, t2 = {}".format(T / Ts[0] * layer1, T / Ts[0] * layer2))


for i, h_t in enumerate(Ts):
    tau = T / h_t
    res = implicit_method(a, b, T, k, phi, g1, g2, f, h_x, tau)
    if prev_res is None:
        table[i][0] = Ts[i]
        table[i][1] = h_x
        table[i][2] = round(tau, mantissa_len)
        table[i][3] = 0
        table[i][4] = 0
        table[i][5] = 0
        table[i][6] = 0
    else:
        # compute indexes of t1 and t2
        t11 = layer1 * Ts[i - 1] // Ts[0]
        t12 = layer1 * Ts[i] // Ts[0]
        t21 = layer2 * Ts[i - 1] // Ts[0]
        t22 = layer2 * Ts[i] // Ts[0]
        table[i][0] = Ts[i]
        table[i][1] = h_x
        table[i][2] = tau
        table[i][3] = SD_h(prev_res, res, t11, t12)
        table[i][4] = SD_h(prev_res, res, t21, t22)
        table[i][5] = max_deviation_h(prev_res, res, t11, t12)
        table[i][6] = max_deviation_h(prev_res, res, t21, t22)
    prev_res = res


df = pd.DataFrame(
    table,
    columns=[
        "N",
        "N h",
        "t",
        "s(t=t_n1)",
        "s(t=t_n2)",
        "max|t=t_n1|",
        "max|t=t_n2|",
    ],
)

print(df)
