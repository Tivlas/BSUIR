import math

import matplotlib

matplotlib.use("TkAgg")
import matplotlib.pyplot as plt

from consts import *
from numpy import linspace
from collocation_method import collocations_method, collocation_method
from galerkin_method import galerkin_method, galerkin_method_2
from help_func import generate_basis_sequence, show_plots, get_basis_system
from min_squares_method import (
    integral_least_square_method,
    discrete_least_square_method,
    integral_LSM,
    discrete_LSM,
)
import numpy


print(
    "collocations method: \n",
    collocations_method(generate_basis_sequence(VARIABLES), linspace(a, b, VARIABLES)),
)


print(
    "integral lsm method: \n",
    integral_least_square_method(generate_basis_sequence(VARIABLES), a, b),
)

print(
    "discrete lsm method: \n",
    discrete_least_square_method(
        generate_basis_sequence(VARIABLES), VARIABLES + DIFF_POINTS, a, b
    ),
)

print(
    "galerkin method: \n",
    galerkin_method(generate_basis_sequence(VARIABLES), a, b),
)

answer_with_collocation_method = []
number_of_basic_func = [3,10]
for n in number_of_basic_func:
    collocation_points = numpy.linspace(a, b, n)
    answer_with_collocation_method.append(
        collocation_method(COEFFS, f, collocation_points)
    )
show_plots(answer_with_collocation_method, a, b, 0.01, "Collocations method")

answer_with_integral_LSM_method = []
for n in number_of_basic_func:
    answer_with_integral_LSM_method.append(integral_LSM(COEFFS, f, n, a, b))
show_plots(answer_with_integral_LSM_method, a, b, 0.01, "Integral LSM method")

answer_with_discrete_LSM_method = []
points = numpy.linspace(a, b, 200)
for n in number_of_basic_func:
    answer_with_discrete_LSM_method.append(discrete_LSM(COEFFS, f, n, points))
show_plots(answer_with_discrete_LSM_method, a, b, 0.01, "Discrete LSM method")

answer_with_galerkin_method = []
for n in number_of_basic_func:
    answer_with_galerkin_method.append(galerkin_method_2(COEFFS, f, n, a, b))
show_plots(answer_with_galerkin_method, a, b, 0.01, "Galerkin method")
