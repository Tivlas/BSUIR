import numpy
numpy.set_printoptions(suppress=True, precision=3, floatmode="fixed")


def input():
    b = numpy.array([[4.2], [4.2], [4.2], [4.2], [4.2]])

    A = numpy.array([
        [2.33,  0.81,  0.67,  0.92, -0.53],
        [-0.53,  2.33,  0.81,  0.67,  0.92],
        [0.92, -0.53,  2.33,  0.81,  0.67],
        [0.67,  0.92, -0.53,  2.33,  0.81],
        [0.81,  0.67,  0.92, -0.53,  2.33]
    ])
    return (A, b)


def nan(n):
    v = numpy.zeros((n, 1))
    v[:] = numpy.NaN
    return v


def gauss_method(A, b):
    n = A.shape[0]

    for k in range(n):
        for i in range(k + 1, n):
            if (A[k, k] == 0.0):
                return nan(n)
            q = A[i, k] / A[k, k]
            for j in range(n):
                A[i, j] -= A[k, j] * q
            b[i] -= b[k] * q

    x = numpy.zeros((n, 1))
    for i in range(n - 1, -1, -1):
        for j in range(i + 1, n):
            b[i] -= A[i, j] * x[j]
        if (A[i, i] == 0.0):
            return nan(n)
        x[i] = b[i] / A[i, i]

    return x


def gauss_jordan_inverse_matrix(A):
    n = A.shape[0]
    E = numpy.array([[1, 0, 0, 0, 0], [0, 1, 0, 0, 0], [
                    0, 0, 1, 0, 0], [0, 0, 0, 1, 0], [0, 0, 0, 0, 1]])
    B = numpy.concatenate((A, E), axis=1)

    for k in range(n):
        for i in range(2*n):
            if (A[k, k] == 0.0):
                return "Обратной матрицы не существует"
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
    inverse_matrix = gauss_jordan_inverse_matrix(A)
    return inverse_matrix.dot(b)

def gauss_jordan_solve(A, b):
    n = A.shape[0]
    B = numpy.concatenate((A, b), axis=1)
    for k in range(n):
        for i in range(n+1):
            if (A[k, k] == 0.0):
                return "Обратной матрицы не существует"
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
                return nan(n)
            q = A[i, k] / A[k, k]
            for j in range(n):
                A[i, j] -= A[k, j] * q
            b[i] -= b[k] * q

    x = numpy.zeros((n, 1))
    for k in range(n - 1, -1, -1):
        for j in range(k + 1, n):
            b[k] -= A[k, j] * x[j]
        if (A[k, k] == 0.0):
            return nan(n)
        x[k] = b[k] / A[k, k]

    return x


def matrix_determinant(A):
    n = A.shape[0]
    rows_swapped = 0
    for k in range(n):
        maxi = k
        for i in range(k, n):
            if (abs(A[i, k]) > abs(A[maxi, k])):
                maxi = i
        if (maxi != k):
            A[[k, maxi]] = A[[maxi, k]]
            rows_swapped += 1

        for i in range(k + 1, n):
            if (A[k, k] == 0.0):
                return 0
            q = A[i, k] / A[k, k]
            for j in range(n):
                A[i, j] -= A[k, j] * q

    det = 1
    for i in range(n):
        det *= A[i, i]
    return rows_swapped % 2 == 0 and det or -det


def output(A, x, b):

    numpy.set_printoptions(suppress=True, precision=6, floatmode="fixed")

    print("A = ")
    print(A)

    print("x = ")
    print(x.T)

    print("Check: b = ")
    print(b.T)
    print(A.dot(x).T)
    print("----------")


def test(method):
    (A, b) = input()
    x = method(A.copy(), b.copy())
    output(A, x, b)


# test(gauss_method)
# test(gauss_main_selection_partial)
# test(gauss_jordan_solve)
test(matrix_method)

(A, b) = input()
#print(gauss_jordan_inverse_matrix(A))
# print(matrix_determinant(A))
