import numpy


def SetMatrix():
    b = numpy.array([[-4.4], [-141]], dtype=float)
    A = numpy.array([
        [0.4,  0.41],
        [0.12,  0.65]], dtype=float)

    return (A, b)


POWER = -5
EPSILON = 10.0 ** POWER


def nan(n):
    v = numpy.zeros((n, 1))
    v[:] = numpy.NaN
    return v


def jacobi_scheme(A, b):
    n = A.shape[0]
    B = numpy.zeros((n, n))
    for i in range(n):
        for j in range(n):
            B[i, j] = - A[i, j] / A[i, i]
        B[i, i] = 0

    c = numpy.zeros((n, 1), dtype=float)
    for i in range(n):
        c[i] = b[i] / A[i, i]
    return (B, c)


def second_way(A, b):
    n = A.shape[0]
    E = numpy.array([[1 if i == j else 0 for j in range(n)]
                    for i in range(n)], dtype=float)
    B = E - A
    return (B, b)


def Norm1(A):
    n = A.shape[0]
    norm1 = max(numpy.absolute(A[i]).sum() for i in range(n))
    return norm1


def Norm2(A):
    n = A.shape[0]
    norm2 = ((A ** 2).sum()) ** (1/2)
    return norm2


def Norm3(A):
    n = A.shape[0]
    norm3 = max(numpy.absolute(A.T[i]).sum() for i in range(n))
    return norm3


def simple_iteration_jacobi(A, b):
    print("МЕТОД ПРОСТЫХ ИТЕРАЦИЙ, СХЕМА ЯКОБИ\n")
    n = A.shape[0]

    for i in range(n):
        if (A[i, i] == 0.0):
            print("Элемент", [i, i], "равен нулю")
            return nan(n)

    (B, c) = jacobi_scheme(A, b)

    # проверка условия диагонального преобладания по строкам
    sum3 = 0
    condition3 = True
    for i in range(n):
        sum3 = 0
        for j in range(n):
            if (i != j):
                sum3 += abs(A[i, j])
        if (sum3 >= abs(A[i, i])):
            print("Условие диагонального преобладания не выполняется")
            condition3 = False
            break

    condition5 = True
    if (not (Norm1(B) < 1)):
        print("Норма_1 матрицы >= 1, не выполняется условие сходимости")
        condition5 = False

    # проверка условия диагонального преобладания по столбцам
    condition4 = True
    condition6 = True
    if (not condition3):
        sum4 = 0
        for i in range(n):
            sum4 = 0
            for j in range(n):
                if (i != j):
                    sum4 += abs(A[j, i])
            if (sum4 >= abs(A[i, i])):
                print("Условие диагонального преобладания не выполняется")
                condition4 = False
                break

        if (not (Norm3(B) < 1)):
            print("Норма_3 матрицы >= 1, не выполняется условие сходимости")
            condition6 = False

    x = c

    norm = 1
    coef = 1
    if (condition3 and condition5):
        norm = Norm1(B)
        coef = norm / (1 - norm)
    elif (condition4 and condition6):
        norm = Norm3(B)
        coef = norm / (1 - norm)

    count = 0
    prev_x = x
    x = B.dot(x) + c
    if (condition3):
        print("Условие диагонального преобладания по строкам выполняется")
        while (coef*max(abs(x-prev_x)) >= EPSILON):
            prev_x = x
            x = B.dot(x) + c
            count += 1

    elif (condition4):
        print("Условие диагонального преобладания по столбцам выполняется")
        while (coef*abs(x-prev_x).sum() >= EPSILON):
            prev_x = x
            x = B.dot(x) + c
            count += 1

    else:
        print("Цикл с огранчением по количеству итераций")
        NUMBER_OF_ITERATIONS = 101
        while (NUMBER_OF_ITERATIONS > 0):
            prev_x = x
            x = B.dot(x) + c
            NUMBER_OF_ITERATIONS -= 1
            count += 1

    print("Количество итераций:", count)
    return x


def simple_iteration_second_way(A, b):
    print("МЕТОД ПРОСТЫХ ИТЕРАЦИЙ, ВТОРОЙ СПОСОБ\n")
    n = A.shape[0]

    for i in range(n):
        if (A[i, i] == 0.0):
            print("Элемент", [i, i], "равен нулю")
            return nan(n)

    (B, c) = second_way(A, b)

    condition1 = True
    if (not (Norm1(B) < 1)):
        print("Норма_1 матрицы >= 1, не выполняется условие сходимости")
        condition1 = False

    condition2 = True
    if (not (Norm3(B) < 1)):
        print("Норма_3 матрицы >= 1, не выполняется условие сходимости")
        condition2 = False

    x = c

    norm = 1
    coef = 1
    if (condition1):
        norm = Norm1(B)
        coef = norm / (1 - norm)
    elif (condition2):
        norm = Norm3(B)
        coef = norm / (1 - norm)

    count = 0
    prev_x = x
    x = B.dot(x) + c
    if (condition1):
        print("Условие сходимости по норме 1 выполняется")
        while (coef*max(abs(x-prev_x)) >= EPSILON):
            prev_x = x
            x = B.dot(x) + c
            count += 1

    elif (condition2):
        print("Условие сходимости по норме 3 выполняется")
        while (coef*abs(x-prev_x).sum() >= EPSILON):
            prev_x = x
            x = B.dot(x) + c
            count += 1

    else:
        print("Цикл с огранчением по количеству итераций")
        NUMBER_OF_ITERATIONS = 101
        while (NUMBER_OF_ITERATIONS > 0):
            prev_x = x
            x = B.dot(x) + c
            NUMBER_OF_ITERATIONS -= 1
            count += 1

    print("Количество итераций:", count)
    return x


def seidel_jacobi(A, b):
    print("МЕТОД ЗЕЙДЕЛЯ, СХЕМА ЯКОБИ\n")
    n = A.shape[0]

    for i in range(n):
        if (A[i, i] == 0.0):
            print("Элемент", [i, i], "равен нулю")
            return nan(n)

    (B, c) = jacobi_scheme(A, b)

    # проверка условия диагонального преобладания по строкам
    sum3 = 0
    condition3 = True
    for i in range(n):
        sum3 = 0
        for j in range(n):
            if (i != j):
                sum3 += abs(A[i, j])
        if (sum3 >= abs(A[i, i])):
            print("Условие диагонального преобладания не выполняется")
            condition3 = False
            break

    condition5 = True
    if (not (Norm1(B) < 1)):
        print("Норма_1 матрицы >= 1, не выполняется условие сходимости")
        condition5 = False

    # проверка условия диагонального преобладания по столбцам
    condition4 = True
    condition6 = True
    if (not condition3):
        sum4 = 0
        for i in range(n):
            sum4 = 0
            for j in range(n):
                if (i != j):
                    sum4 += abs(A[j, i])
            if (sum4 >= abs(A[i, i])):
                print("Условие диагонального преобладания не выполняется")
                condition4 = False
                break

        if (not (Norm3(B) < 1)):
            print("Норма_3 матрицы >= 1, не выполняется условие сходимости")
            condition6 = False

    H = numpy.zeros((n, n))
    for i in range(n):
        for j in range(i):
            H[i, j] = B[i, j]

    F = numpy.zeros((n, n))
    for i in range(n):
        for j in range(i+1, n):
            F[i, j] = B[i, j]

    count = 0
    x = c
    norm = 1
    coef = 1
    if (condition3 and condition5):
        norm = Norm1(B)
        coef = norm / (1 - norm)
    elif (condition4 and condition6):
        norm = Norm3(B)
        coef = norm / (1 - norm)

    prev_x = x
    x = H.dot(x) + F.dot(prev_x) + c
    if (condition3):
        print("Условие диагонального преобладания по строкам выполняется")
        while (coef*max(abs(x-prev_x)) >= EPSILON):
            prev_x = x
            x = H.dot(x) + F.dot(prev_x) + c
            count += 1

    elif (condition4):
        print("Условие диагонального преобладания по столбцам выполняется")
        while (coef*abs(x-prev_x).sum() >= EPSILON):
            prev_x = x
            x = H.dot(x) + F.dot(prev_x) + c
            count += 1

    else:
        print("Цикл с огранчением по количеству итераций")
        NUMBER_OF_ITERATIONS = 101
        while (NUMBER_OF_ITERATIONS > 0):
            prev_x = x
            x = H.dot(x) + F.dot(prev_x) + c
            NUMBER_OF_ITERATIONS -= 1
            count += 1

    print("Количество итераций:", count)
    return x


def seidel_second_way(A, b):
    print("МЕТОД ЗЕЙДЕЛЯ, ВТОРОЙ СПОСОБ\n")
    n = A.shape[0]

    for i in range(n):
        if (A[i, i] == 0.0):
            print("Элемент", [i, i], "равен нулю")
            return nan(n)

    (B, c) = second_way(A, b)

    condition1 = True
    if (not (Norm1(B) < 1)):
        print("Норма_1 матрицы >= 1, не выполняется условие сходимости")
        condition1 = False

    condition2 = True
    if (not (Norm3(B) < 1)):
        print("Норма_3 матрицы >= 1, не выполняется условие сходимости")
        condition2 = False

    x = c

    norm = 1
    coef = 1
    if (condition1):
        norm = Norm1(B)
        coef = norm / (1 - norm)
    elif (condition2):
        norm = Norm3(B)
        coef = norm / (1 - norm)

    H = numpy.zeros((n, n))
    for i in range(n):
        for j in range(i):
            H[i, j] = B[i, j]

    F = numpy.zeros((n, n))
    for i in range(n):
        for j in range(i, n):
            F[i, j] = B[i, j]

    count = 0
    prev_x = x
    x = H.dot(x) + F.dot(prev_x) + c
    if (condition1):
        print("Условие сходимости по норме 1 выполняется")
        while (coef*max(abs(x-prev_x)) >= EPSILON):
            prev_x = x
            x = H.dot(x) + F.dot(prev_x) + c
            count += 1

    elif (condition2):
        print("Условие сходимости по норме 3 выполняется")
        while (coef*abs(x-prev_x).sum() >= EPSILON):
            prev_x = x
            x = H.dot(x) + F.dot(prev_x) + c
            count += 1

    else:
        print("Цикл с огранчением по количеству итераций")
        NUMBER_OF_ITERATIONS = 101
        while (NUMBER_OF_ITERATIONS > 0):
            prev_x = x
            x = H.dot(x) + F.dot(prev_x) + c
            NUMBER_OF_ITERATIONS -= 1
            count += 1

    print("Количество итераций:", count)
    return x


def output(A, x, b):

    numpy.set_printoptions(
        suppress=True, precision=abs(POWER), floatmode="fixed")

    print("A = ")
    print(A)
    print()
    print("x = ")
    print(x.T)
    print()
    print("Check: b = ")
    print(b.T)
    print()
    print(A.dot(x).T)
    print("———————————————————————————————————————————————————————————————————————————————————————")
    print("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||")
    print("———————————————————————————————————————————————————————————————————————————————————————")


def test(method):
    (A, b) = SetMatrix()
    x = method(A.copy(), b.copy())
    output(A, x, b)


test(simple_iteration_jacobi)
test(simple_iteration_second_way)
test(seidel_jacobi)
test(seidel_second_way)
