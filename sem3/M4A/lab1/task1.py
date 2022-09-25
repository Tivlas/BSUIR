import numpy


def input():
    custom_precision = 4
    # b = numpy.array([[4.2], [4.2], [4.2], [4.2], [4.2]])

   # A = numpy.array([
   #    [2.33,  0.81,  0.67,  0.92, -0.53],
   #    [-0.53,  2.33,  0.81,  0.67,  0.92],
   #    [0.92, -0.53,  2.33,  0.81,  0.67],
   #    [0.67,  0.92, -0.53,  2.33,  0.81],
   #    [0.81,  0.67,  0.92, -0.53,  2.33]
   # ])

    #b = numpy.array([[8], [10], [12]], dtype=float)
    # A = numpy.array([
    #    [1,  2,  1],
    #    [2,  1,  2],
    #    [3, 3,  1]], dtype=float)

    b = numpy.array([[1], [10]], dtype=float)
    A = numpy.array([[0, 0], [0, 0]], dtype=float)

    return (A, b, custom_precision)


def nan(n, m):
    v = numpy.zeros((n, m))
    v[:] = numpy.NaN
    return v


def gauss_method(A, b):
    print("Метод Гаусса:")
    n = A.shape[0]

    for k in range(n):
        for i in range(k + 1, n):
            if (A[k, k] == 0.0):
                return nan(n, 1)
            q = A[i, k] / A[k, k]
            for j in range(n):
                A[i, j] -= A[k, j] * q
            b[i] -= b[k] * q

    x = numpy.zeros((n, 1))
    for i in range(n - 1, -1, -1):
        for j in range(i + 1, n):
            b[i] -= A[i, j] * x[j]
        if (A[i, i] == 0.0):
            return nan(n, 1)
        x[i] = b[i] / A[i, i]

    return x


def gauss_jordan_inverse_matrix(A):
    if (matrix_determinant(A) == 0):
        return nan(A.shape[0], A.shape[1])

    n = A.shape[0]
    E = numpy.array([[1 if i == j else 0 for j in range(n)] for i in range(n)])
    B = numpy.concatenate((A, E), axis=1)

    for k in range(n):
        for i in range(2*n):
            if (A[k, k] == 0.0):
                return nan(n, n)
            B[k, i] /= A[k, k]
        for i in range(k+1, n):
            q = B[i, k] / B[k, k]
            for j in range(2*n):
                B[i, j] -= B[k, j] * q

        for i in range(n):
            for j in range(n):
                A[i, j] = B[i, j]

    for k in range(n-1, -1, -1):
        for i in range(2*n-1, -1, -1):

            B[k, i] /= A[k, k]
        for i in range(k-1, -1, -1):
            q = B[i, k] / B[k, k]
            for j in range(2*n-1, -1, -1):
                B[i, j] -= B[k, j] * q

    return B[:, n:]


def matrix_method(A, b):
    print("Матричный метод:")
    inverse_matrix = gauss_jordan_inverse_matrix(A)
    return inverse_matrix.dot(b)


def gauss_jordan_solve(A, b):
    print("Метод Гаусса-Жордана:")
    n = A.shape[0]
    B = numpy.concatenate((A, b), axis=1)
    for k in range(n):
        for i in range(n+1):
            if (A[k, k] == 0.0):
                return nan(n, 1)
            B[k, i] /= A[k, k]
        for i in range(k+1, n):
            q = B[i, k] / B[k, k]
            for j in range(n+1):
                B[i, j] -= B[k, j] * q
        for i in range(n):
            for j in range(n):
                A[i, j] = B[i, j]

    for k in range(n-1, -1, -1):
        for i in range(n-1, -1, -1):
            B[k, i] /= A[k, k]
        for i in range(k-1, -1, -1):
            q = B[i, k] / B[k, k]
            for j in range(n, -1, -1):
                B[i, j] -= B[k, j] * q

    return B[:, n:]


def gauss_main_selection_partial(A, b):
    print("Метод Гаусса с частичным выбором главного элемента по столбцу:")
    n = A.shape[0]

    for k in range(n):
        maxi = k
        for i in range(k, n):
            if (abs(A[i, k]) > abs(A[maxi, k])):
                maxi = i
        A[[k, maxi]] = A[[maxi, k]]
        b[[k, maxi]] = b[[maxi, k]]

        for i in range(k + 1, n):
            if (A[k, k] == 0.0):
                return nan(n, 1)
            q = A[i, k] / A[k, k]
            for j in range(n):
                A[i, j] -= A[k, j] * q
            b[i] -= b[k] * q

    x = numpy.zeros((n, 1))
    for k in range(n - 1, -1, -1):
        for j in range(k + 1, n):
            b[k] -= A[k, j] * x[j]
        if (A[k, k] == 0.0):
            return nan(n, 1)
        x[k] = b[k] / A[k, k]

    return x


def matrix_determinant(A):
    n = A.shape[0]
    A_copy = A.copy()
    rows_swapped = 0
    for k in range(n):
        maxi = k
        for i in range(k, n):
            if (abs(A_copy[i, k]) > abs(A_copy[maxi, k])):
                maxi = i
        if (maxi != k):
            A_copy[[k, maxi]] = A_copy[[maxi, k]]
            rows_swapped += 1

        for i in range(k + 1, n):
            if (A_copy[k, k] == 0.0):
                return 0
            q = A_copy[i, k] / A_copy[k, k]
            for j in range(n):
                A_copy[i, j] -= A_copy[k, j] * q

    det = 1
    for i in range(n):
        det *= A_copy[i, i]
    return rows_swapped % 2 == 0 and det or -det


def output_solutions(A, x, b, custom_prec):
    numpy.set_printoptions(
        suppress=True, precision=custom_prec, floatmode="fixed")

    print("A = ")
    print(A)
    print()

    print("x = ")
    print(x)
    print()

    print("Начальная матрица b = ")
    print(b)
    print()
    print("Проверка: A * x = ")
    print(A.dot(x))
    print("-----------------------------------------------------------------------")


def output(somthing, custom_prec):
    numpy.set_printoptions(
        suppress=True, precision=custom_prec, floatmode="fixed")
    print(somthing)
    print("-----------------------------------------------------------------------")


def test(method):
    (A, b, custom_prec) = input()
    x = method(A.copy(), b.copy())
    output_solutions(A, x, b, custom_prec)


test(gauss_method)
test(gauss_main_selection_partial)
test(gauss_jordan_solve)
test(matrix_method)

(A, b, custom_prec) = input()
print("\nОбратная матрица и определитель:\n")
output(gauss_jordan_inverse_matrix(A), custom_prec)
output(matrix_determinant(A), custom_prec)
