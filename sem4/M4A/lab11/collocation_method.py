import numpy
from consts import sympy, x
from help_func import (
    build_function_from_basis,
    func_for_substantiation,
    get_basis_system,
    numerical_diff,
)
from sympy import linsolve


def collocations_method(basis, points):
    func = build_function_from_basis(basis)
    psi_func = func_for_substantiation(func)
    symbols = [sympy.Symbol("a" + str(i)) for i in range(len(points))]
    lin_system = []

    for point in points:
        lin_system.append(psi_func.subs(x, point).evalf())

    answer = linsolve(lin_system, *symbols)

    result = []

    for value in answer.args[0]:
        result.append(round(value, 6))

    return result


def collocation_method(coefficients, f, collocation_points):
    num_of_basis_functions = len(collocation_points) + 1
    basis = get_basis_system(num_of_basis_functions)

    def resudial_part_diff(a):
        return lambda d: sum(
            coefficients[i](d) * numerical_diff(basis[a], d, i)
            for i in range(len(coefficients))
        )

    matrix = [
        [resudial_part_diff(i)(point)
         for i in range(1, num_of_basis_functions)]
        for point in collocation_points
    ]
    right_side = [
        f(point) - resudial_part_diff(0)(point) for point in collocation_points
    ]
    answer = numpy.linalg.solve(numpy.matrix(matrix), numpy.array(right_side))
    return lambda x: basis[0](x) + sum(
        answer[i - 1] * basis[i](x) for i in range(1, num_of_basis_functions)
    )
