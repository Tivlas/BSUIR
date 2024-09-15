from copy import deepcopy
from simple_colors import *

import numpy


def place_marks_on_B(B, basis):
    tmp = not B[basis]
    for i, j in B.keys():
        if basis[0] == i:
            if B[(i, j)] is None:
                B[(i, j)] = tmp
                place_marks_on_B(B, (i, j))
        if basis[1] == j:
            if B[(i, j)] is None:
                B[(i, j)] = tmp
                place_marks_on_B(B, (i, j))


def potentials_method(a, b, c):
    difference = numpy.sum(a) - numpy.sum(b)
    if difference > 0:
        b = numpy.append(b, difference)
        c = numpy.hstack((c, numpy.zeros((len(a), 1))))
        print('измененный B')

    elif difference < 0:
        a = numpy.append(a, -difference)
        c = numpy.vstack((c, numpy.zeros((len(a), 1))))
        print('измененный A')


    x = numpy.zeros((len(a), len(b)))
    B = []

    i = 0
    j = 0

    while i < len(a) and j < len(b):
        minimum = min(a[i], b[j])
        x[i, j] = minimum
        B.append((i, j))
        a[i] -= minimum
        b[j] -= minimum
        if a[i] == 0 and i < len(a) - 1:
            i += 1
        elif b[j] == 0:
            j += 1
    print(x)
    print(B)

    iteration = 0
    while True:
        iteration += 1
        print(yellow(f'\nитерация {iteration}'))

        u_v_vals = []
        result_vals = []
        for i, j in B:
            u_vals = [0] * a.shape[0]
            v_vals = [0] * b.shape[0]
            u_vals[i] = 1
            v_vals[j] = 1
            u_v_vals.append(u_vals + v_vals)
            result_vals.append(c[i, j])
        u_vals = [0] * (a.shape[0] + b.shape[0])
        u_vals[0] = 1
        u_v_vals.append(u_vals)
        result_vals.append(0)

        result = numpy.linalg.solve(u_v_vals, result_vals)
        u = result[:len(a)]
        v = result[len(b):]

        print(f'\nu: {u}')
        print(f'v: {v}\n')

        basis_to_insert = None

        for i in range(len(c)):
            for j in range(len(c)):
                if (i, j) not in B and u[i] + v[j] > c[i, j]:
                    basis_to_insert = (i, j)
                    break
            if basis_to_insert is not None:
                break
        if basis_to_insert is None:
            print(green(f'x is :\n {x}\nоптимальный план'))
            return

        B.append(basis_to_insert)
        B = sorted(B)
        copy_of_B = deepcopy(B)

        for i in range(len(x)):
            counter = 0
            for j in range(len(x)):
                if (i, j) in copy_of_B:
                    counter += 1
            if counter <= 1:
                for j in range(len(x)):
                    if (i, j) in copy_of_B:
                        copy_of_B.remove((i, j))

        for j in range(len(x)):
            counter = 0
            for i in range(len(x)):
                if (i, j) in copy_of_B:
                    counter += 1
            if counter <= 1:
                for i in range(len(x)):
                    if (i, j) in copy_of_B:
                        copy_of_B.remove((i, j))

        B_with_plus_and_minus = {item: None for item in copy_of_B}
        B_with_plus_and_minus[basis_to_insert] = True

        place_marks_on_B(B_with_plus_and_minus, basis_to_insert)

        minimum_from_x_with_minus = numpy.inf

        for i in range(len(x)):
            for j in range(len(x)):
                if not B_with_plus_and_minus.get((i, j), True):
                    minimum_from_x_with_minus = min(minimum_from_x_with_minus, x[i, j])

        for (i, j) in B_with_plus_and_minus.keys():
            if B_with_plus_and_minus[(i, j)]:
                x[i, j] += minimum_from_x_with_minus
            else:
                x[i, j] -= minimum_from_x_with_minus

        for i, j in B:
            if x[i, j] == 0 and not B_with_plus_and_minus.get((i, j), True):
                B.remove((i, j))
                break
        print(f'x is: \n{x}')


def main():
    a = numpy.array([100, 300, 300])
    b = numpy.array([300, 200, 200])
    c = numpy.array([[8, 4, 1],
                     [8, 4, 3],
                     [9, 7, 5]])

    potentials_method(a, b, c)


if __name__ == '__main__':
    main()
