import numpy
from simple_colors import *

import algo

A = numpy.array([[-1, 1, 1, 0, 0], [1, 0, 0, 1, 0], [0, 1, 0, 0, 1]])
C_with_T = numpy.array([1, 1, 0, 0, 0])
X_with_T = numpy.array([0, 0, 1, 3, 2])
B = numpy.array([2, 3, 4])  

A_B = numpy.zeros((B.size, B.size))
A_B_INV = numpy.zeros((B.size, B.size))
C_T_B = numpy.zeros(B.size)
U_T = numpy.zeros(C_T_B.size)
TRIANGLE_T = numpy.zeros(C_with_T)
j_0 = 0  
z = numpy.zeros(len(A))
TETHA = numpy.zeros(len(z))
TETHA_MIN_INDEX = 0
TETHA_MIN = 0


def main():
    for iteration in range(0, 100):
        print()
        print(cyan('Iretation ' + str(iteration + 1)))
        print(green('Step 1'))

        for index in range(len(A)):
            for jindex in range(len(A)):
                A_B[index][jindex] = A[index][B[jindex]]
        print('A_B =\n', A_B)

        print()

        print(green('Step 2'))

        if iteration == 0:

            A_B_INV = numpy.linalg.inv(A_B)
        else:
            tmp = numpy.zeros(B.size)

            for index in range(0, B.size):
                tmp[index] = A_B[index][TETHA_MIN_INDEX]

            A_B_INV = algo.get_inv(A_B_INV, tmp,
                                   TETHA_MIN_INDEX + 1,
                                   B.size) 

        print('A_B_INV =\n', A_B_INV)

        print()

        print(green('Step 3'))
        for index in range(B.size):
            C_T_B[index] = C_with_T[B[index]]

        print('C_T_B =\n', C_T_B)

        print()

        print(green('Step 4'))

        U_T = numpy.dot(C_T_B, A_B_INV)

        print('U_T =\n', U_T)

        print()

        print(green('Step 5'))

        TRIANGLE_T = numpy.subtract(numpy.dot(U_T, A), C_with_T)

        print('Triangl_t =\n', TRIANGLE_T)

        print()

        print(green('Step 6'))

        counter_tmp = 0

        for index in range(0, TRIANGLE_T.size):
            if TRIANGLE_T[index] < 0:
                j_0 = index
                break
            else:
                counter_tmp += 1

            if counter_tmp == len(TRIANGLE_T):
                print(yellow('Optimal plan : '), X_with_T)
                print(B)
                return

        print('j_0 = ', j_0)

        print()

        print(green('Step 7'))

        tmp = numpy.zeros(3)

        for index in range(0, len(A)):
            tmp[index] = A[index][j_0]

        z = numpy.dot(A_B_INV, tmp)

        print('z = ', z)

        print()

        print(green('Step 8'))

        for index in range(0, len(z)):
            if z[index] > 0:
                TETHA[index] = X_with_T[B[index]] / z[index]
                TETHA_MIN_INDEX = index
                TETHA_MIN = X_with_T[B[index]] / z[index]
            else:
                TETHA[index] = 999
        print("TETHA = ", TETHA)
        print("TETHA_MIN = ", TETHA_MIN) 

        print()

        print(green('Step 9'))

        B[TETHA_MIN_INDEX] = j_0
        print('NEW B IS : ', B) 

        for index in range(0, len(B)):
            if index == TETHA_MIN_INDEX:
                X_with_T[B[index]] = TETHA_MIN
            else:
                X_with_T[B[index]] = X_with_T[B[index]] - \
                    (TETHA_MIN * z[index])

        for index in range(0, len(X_with_T)):
            if index not in B:
                X_with_T[index] = 0

        print('NEW X_with_T IS: ', X_with_T)

        print()
        print()


if __name__ == '__main__':
    main()
