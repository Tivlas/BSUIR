import numpy
from simple_colors import *

import lab1


def lab2(A, CT, XT, B):
    A_B = numpy.zeros((B.size, B.size))
    A_B_INV = numpy.zeros((B.size, B.size))
    C_T_B = numpy.zeros(B.size)
    U_T = numpy.zeros(C_T_B.size)
    j_0 = 0
    z = numpy.zeros(len(A))
    TETHA = numpy.zeros(len(z))
    TETHA_MIN_INDEX = 0
    TETHA_MIN = 0

    for index in range(0, B.size):
        B[index] = B[index] - 1

    for iteration in range(0, 100):
        print()
        print(green('итерация 2.' + str(iteration + 1)))
        print(yellow("шаг #2.1"))

        for index in range(len(A)):
            for jindex in range(len(A)):
                A_B[index][jindex] = A[index][B[jindex]]
        print('A_B =\n', A_B)

        print()

        print(yellow("шаг #2.2"))

        if iteration == 0:

            A_B_INV = numpy.linalg.inv(A_B)
        else:
            tmp = numpy.zeros(B.size)

            for index in range(0, B.size):
                tmp[index] = A_B[index][TETHA_MIN_INDEX]

            A_B_INV = lab1.get_inv(A_B_INV, tmp,
                                   TETHA_MIN_INDEX + 1,
                                   B.size)
        print('A_B_INV =\n', A_B_INV)

        print()

        print(yellow("шаг #2.3"))

        for index in range(B.size):
            C_T_B[index] = CT[B[index]]

        print('C_T_B =\n', C_T_B)

        print()

        print(yellow("шаг #2.4"))

        U_T = numpy.dot(C_T_B, A_B_INV)

        print('U_T =\n', U_T)

        print()

        print(yellow("шаг #2.5"))

        TRIANGLE_T = numpy.subtract(numpy.dot(U_T, A), CT)

        print('TRIANGLE_T =\n', TRIANGLE_T)

        print()

        print(yellow("шаг #2.6"))

        counter_tmp = 0

        for index in range(0, TRIANGLE_T.size):
            if TRIANGLE_T[index] < 0:
                j_0 = index
                break
            else:
                counter_tmp += 1

            if counter_tmp == len(TRIANGLE_T):
                return [XT, B]

        print('j_0 = ', j_0)

        print(yellow("шаг #2.7"))

        print()

        tmp = numpy.zeros(2)

        for index in range(0, len(A)):
            tmp[index] = A[index][j_0]

        z = numpy.dot(A_B_INV, tmp)

        print('z = ', z)

        print()

        print(yellow("шаг #2.8"))

        for index in range(0, len(z)):
            if z[index] > 0:
                TETHA[index] = XT[B[index]] / z[index]
                TETHA_MIN_INDEX = index
                TETHA_MIN = XT[B[index]] / z[index]
            else:
                TETHA[index] = 999
        print("TETHA = ", TETHA)
        print("TETHA_MIN = ", TETHA_MIN) 

        print()

        print(yellow("шаг #2.9"))

        B[TETHA_MIN_INDEX] = j_0
        print('B\': ', B)

        for index in range(0, len(B)):
            if index == TETHA_MIN_INDEX:
                XT[B[index]] = TETHA_MIN
            else:
                XT[B[index]] = XT[B[index]] - \
                    (TETHA_MIN * z[index])

        for index in range(0, len(XT)):
            if index not in B:
                XT[index] = 0

        print('NEW X_with_T IS: ', XT)

        print()
