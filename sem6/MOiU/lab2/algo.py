import copy

import numpy


def initial_step():
    matrix = numpy.array([[3, 5, 7], [6, 3, 8], [9, 5, 6]])
    matrix_inverted = numpy.linalg.inv(matrix)
    x_vector_column = numpy.array([6, 7, 8])
    index = 2
    n_size = 3
    matrix_with_overline = copy.deepcopy(matrix)
    for i in range(0, n_size):
        matrix_with_overline[i][index - 1] = x_vector_column[i]
    return matrix, matrix_inverted, x_vector_column, index, n_size, matrix_with_overline


def step1(matrix_inverted, x_vector_column, index):
    l_vector_column = numpy.dot(matrix_inverted, x_vector_column)
    if l_vector_column[index - 1] == 0:
        print('Matrix A^- are irreversible')
        return 0
    else:
        return l_vector_column


def step2(l, index):
    l[index - 1] = -1
    return l


def step3(l, l_with_overline, index):
    l_with_roof = ((-1) / l[index - 1]) * l_with_overline
    return l_with_roof


def step4(n_size, l_with_roof, index):
    q = numpy.eye(n_size)
    for i in range(0, n_size):
        q[i][index - 1] = l_with_roof[i]

    return q


def step5(n_size, index, matrix_q, matrix_inverted):
    final_matrix = numpy.zeros((n_size, n_size))
    for i in range(0, n_size):
        for k in range(0, n_size):
            if index - 1 != i:
                final_matrix[i][k] = matrix_q[i][i] * matrix_inverted[i][k] + \
                    matrix_q[i][index - 1] * matrix_inverted[index - 1][k]
            else:
                final_matrix[i][k] = matrix_q[i][index - 1] * \
                    matrix_inverted[index - 1][k]
    return final_matrix


def get_inv(matrix_inverted, x_vector_column, index, n_size):
    l = step1(matrix_inverted, x_vector_column, index)
    if l.size == 0:
        return

    l_with_overline = step2(copy.deepcopy(l), index)

    l_with_roof = step3(copy.deepcopy(
        l), copy.deepcopy(l_with_overline), index)

    matrix_q = step4(n_size, copy.deepcopy(l_with_roof), index)

    final_matrix = step5(n_size, index, copy.deepcopy(
        matrix_q), copy.deepcopy(matrix_inverted))

    return final_matrix
