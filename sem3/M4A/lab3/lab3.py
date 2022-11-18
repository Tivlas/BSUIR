import numpy
from sympy import symbols, solve

# A = numpy.array([[11, 4.8, 5.5],
#                     [21, 5125, 1],
#                     [3, 4.77, 55]], dtype=float)

# МЕТОД ДАНИЛЕВСКОГО


def get_M_matrix(A, M_index):
    M_index = M_index - 1
    n = A.shape[0]
    M_row_index = M_index
    M = numpy.identity(n)
    for i in range(n):
        if A[M_row_index + 1, M_index] == 0:
            raise Exception("ДЕЛЕНИЕ НА НОЛЬ")
        if i != M_index:
            M[M_row_index, i] = -A[M_row_index + 1, i] / \
                A[M_row_index + 1, M_index]
        else:
            M[M_row_index, i] = 1/A[M_row_index + 1, M_index]
    return M


def solution():
    numpy.set_printoptions(
        suppress=True, precision=4, floatmode="fixed")

    A = numpy.array([[25, -0.0908, 0.0182, 0.0363, 0.1271],
                     [0.0944, 44, 0, -0.0726, 0.0726],
                     [0.0545, 0, 66, -0.2541, 0.1452],
                     [-0.1089, 0.2287, 0, 0.8531, -0.0363],
                     [0.4538, 0, 0.1634, 0.0182, 1.0164]], dtype=float)

    n = A.shape[0]
    print("\nМатрица А:")
    print(A)

    S = numpy.identity(n)
    for i in range(n-1, 0, -1):
        m = get_M_matrix(A, i)
        S = S @ m
        A = A.dot(m)
        m_inverse = numpy.linalg.inv(m)
        A = m_inverse @ A
        print("\nМатрица А(", n-i, "):", sep="")
        print(A)

    p = A[0][:]
    print("\nКоэффициенты многочлена:")
    print(p)

    x = symbols('x')
    eq = "x**" + str(n)
    for i in range(n):
        eq += "-" + "{0:5f}".format(p[i]) + "*x**" + str(n-1-i)

    full_eq = "(-1)**"+str(n)+"*(" + eq + ")"
    print("\nМногочлен:")
    print(full_eq)
    lambdas = solve(full_eq, x)
    print("\nСобственные значения:")
    print(lambdas)
    print()


solution()


# МЕТОД ВРАЩЕНИЙ ЯКОБИ

EPS = 10.0**-6
numpy.set_printoptions(suppress=True, precision=4, floatmode="fixed")


def input():
    C = numpy.array(
        [
            [0.2, 0.0, 0.2, 0.0, 0.0],
            [0.0, 0.2, 0.0, 0.2, 0.0],
            [0.2, 0.0, 0.2, 0.0, 0.2],
            [0.0, 0.2, 0.0, 0.2, 0.0],
            [0.0, 0.0, 0.2, 0.0, 0.2],
        ]
    )
    D = numpy.array(
        [
            [2.33, 0.81, 0.67, 0.92, -0.53],
            [0.81, 2.33, 0.81, 0.67, 0.92],
            [0.67, 0.81, 2.33, 0.81, 0.92],
            [0.92, 0.67, 0.81, 2.33, -0.53],
            [-0.53, 0.92, 0.92, -0.53, 2.33],
        ]
    )
    A = 3 * C + D

    return A


A = input()
n = len(A)
print("A = ")
print(A)


iters = 0

ansV = numpy.eye(n)
while True:
    iters += 1
    max_elem = (0, 1)
    for i in range(n):
        for j in range(i + 1, n):
            if abs(A[i][j]) > abs(A[max_elem]):
                max_elem = (i, j)
    (i, j) = max_elem
    if A[i][i] == A[j][j]:
        p = numpy.pi / 4
    else:
        p = 2 * A[i][j] / (A[i][i] - A[j][j])
    c = numpy.cos(1 / 2 * numpy.arctan(p))
    s = numpy.sin(1 / 2 * numpy.arctan(p))
    V = numpy.eye(n)
    V[i][i] = c
    V[i][j] = -s
    V[j][i] = s
    V[j][j] = c
    A = V.T @ A @ V
    print(f"\nМатрица A({iters}):", sep="")
    print(A)
    ansV = ansV @ V
    if abs((A - numpy.diag(numpy.diag(A)))**2).sum() < EPS:
        ansW = numpy.diag(A)
        break


def answer(W, V):
    V = numpy.array([(-i if i[0] < 0 else i) for i in V.T]).T
    (W, V) = list(zip(*(sorted(list(zip(W, V.T)), key=lambda t: t[0]))))
    W = numpy.array(W)
    V = numpy.array(V).T
    return (W, V)


print("-----")
print("Ответ:")
(W, V) = answer(ansW, ansV)
print("Собственные значения:")
print(W)
print("Собственные векторы:")
print(V)
print(f"Количество итераций: {iters}")
