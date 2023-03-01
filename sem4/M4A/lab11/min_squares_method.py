import functools

import numpy
from consts import *
from help_func import (
    build_function_from_basis,
    func_for_substantiation,
    get_basis_system,
    numerical_diff,
    numerical_integration,
)
from numpy import linspace
from sympy import linsolve


def integral_least_square_method(basis, a, b):
    func = build_function_from_basis(basis)
    psi_func = func_for_substantiation(func)
    symbols = [sympy.Symbol("a" + str(i)) for i in range(len(basis))]
    lin_system = []

    for i in range(len(basis)):
        lin_system.append(
            sympy.integrate(
                2 * sympy.diff(psi_func, symbols[i]) * psi_func, (x, a, b)
            ).evalf()
        )

    answer = linsolve(lin_system, *symbols)

    result = []

    for value in answer.args[0]:
        result.append(round(value, 6))

    return result


def discrete_least_square_method(basis, points_num, a, b):
    func = build_function_from_basis(basis)
    psi_func = func_for_substantiation(func)

    seq = [
        psi_func.subs(x, point) ** 2
        for point in linspace(a + 0.05, b - 0.05, points_num)
    ]

    psi_sqr_sum = functools.reduce((lambda a, b: a + b), seq)
    symbols = [sympy.Symbol("a" + str(i)) for i in range(len(basis))]
    lin_system = []

    for i in range(len(basis)):
        lin_system.append(sympy.diff(psi_sqr_sum, symbols[i]).evalf())

    answer = linsolve(lin_system, *symbols)

    result = []

    for value in answer.args[0]:
        result.append(round(value, 6))

    return result


def integral_LSM(coefficients, f, num_of_basis_functions, a, b):
    basis = get_basis_system(num_of_basis_functions)

    def resudial_part_diff(a):
        return lambda x: sum(
            coefficients[i](x) * numerical_diff(basis[a], x, i)
            for i in range(len(coefficients))
        )

    part_diffs = [resudial_part_diff(i) for i in range(num_of_basis_functions)]

    matrix = [
        [
            numerical_integration(lambda x: part_diffs[i](x) * part_diffs[j](x), a, b)
            for j in range(1, num_of_basis_functions)
        ]
        for i in range(1, num_of_basis_functions)
    ]
    right_side = [
        numerical_integration(
            lambda x: (f(x) - part_diffs[0](x)) * part_diffs[i](x), a, b
        )
        for i in range(1, num_of_basis_functions)
    ]
    answer = numpy.linalg.solve(numpy.matrix(matrix), numpy.array(right_side))
    return lambda x: basis[0](x) + sum(
        answer[i - 1] * basis[i](x) for i in range(1, num_of_basis_functions)
    )


def discrete_LSM(coefficients, f, num_of_basis_functions, points):
    basis = get_basis_system(num_of_basis_functions)

    def resudial_part_diff(a, x):
        return sum(
            coefficients[i](x) * numerical_diff(basis[a], x, i)
            for i in range(len(coefficients))
        )

    part_diffs_x = {
        (i, x): resudial_part_diff(i, x)
        for i in range(num_of_basis_functions)
        for x in points
    }

    matrix = [
        [
            sum(part_diffs_x[(i, x)] * part_diffs_x[(j, x)] for x in points)
            for j in range(1, num_of_basis_functions)
        ]
        for i in range(1, num_of_basis_functions)
    ]
    right_side = [
        sum([(f(x) - part_diffs_x[(0, x)]) * part_diffs_x[(i, x)] for x in points])
        for i in range(1, num_of_basis_functions)
    ]

    answer = numpy.linalg.solve(numpy.matrix(matrix), numpy.array(right_side))
    return lambda x: basis[0](x) + sum(
        answer[i - 1] * basis[i](x) for i in range(1, num_of_basis_functions)
    )
