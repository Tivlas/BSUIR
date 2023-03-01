import numpy
from consts import *
from help_func import (
    build_function_from_basis,
    func_for_substantiation,
    get_basis_system,
    numerical_diff,
    numerical_integration,
)
from sympy import linsolve


def galerkin_method(basis, a, b):
    func = build_function_from_basis(basis)
    print(f"Functions from basics: {func}")
    psi_func = func_for_substantiation(func)
    symbols = [sympy.Symbol("a" + str(i)) for i in range(len(basis))]
    lin_system = []

    for i in range(len(basis)):
        lin_system.append(sympy.integrate(psi_func * basis[i], (x, a, b)).evalf())

    answer = linsolve(lin_system, *symbols)

    result = []

    for value in answer.args[0]:
        result.append(round(value, 6))

    return result


def galerkin_method_2(coefficients, f, num_of_basis_functions, a, b):
    basis = get_basis_system(num_of_basis_functions)

    def resudial_part_diff(a):
        return lambda x: sum(
            coefficients[i](x) * numerical_diff(basis[a], x, i)
            for i in range(len(coefficients))
        )

    part_diffs = [resudial_part_diff(i) for i in range(num_of_basis_functions)]

    def optimal_numerical_integration(f1, f2, points):
        dx = 2 / len(points)
        return sum([(f1[i] * f2[i] * dx) for i in range(0, len(points))])

    points = numpy.linspace(a, b, 100)
    points = [(points[i] + points[i - 1]) / 2 for i in range(1, len(points))]

    opt_part_diffs = [None] * num_of_basis_functions
    opt_basis = [None] * num_of_basis_functions
    for i in range(1, num_of_basis_functions):
        opt_part_diffs[i] = []
        opt_basis[i] = []
        for point in points:
            opt_part_diffs[i].append(part_diffs[i](point))
            opt_basis[i].append(basis[i](point))

    matrix = [
        [
            optimal_numerical_integration(opt_part_diffs[j], opt_basis[i], points)
            for j in range(1, num_of_basis_functions)
        ]
        for i in range(1, num_of_basis_functions)
    ]

    right_side = [
        numerical_integration(lambda x: (f(x) - part_diffs[0](x)) * basis[i](x), a, b)
        for i in range(1, num_of_basis_functions)
    ]

    answer = numpy.linalg.solve(numpy.matrix(matrix), numpy.array(right_side))

    return lambda x: basis[0](x) + sum(
        answer[i - 1] * basis[i](x) for i in range(1, num_of_basis_functions)
    )
