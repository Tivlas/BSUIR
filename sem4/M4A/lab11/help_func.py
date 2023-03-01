import matplotlib.pyplot as plt
import numpy
from consts import *


def func_for_substantiation(subs):
    func = sympy.sin(VARIANT) * sympy.diff(subs, x, x) + (
        (1 + sympy.cos(VARIANT) * x**2) * subs + 1
    )
    return func


# def func_for_substantiation(subs):
#     func = sympy.diff(subs, x, x) + ((1 + x**2) * subs + 1)
#     return func


def generate_basis_sequence(n):
    sequence = []
    for i in range(n):
        sequence.append((x**i) * (1 - x**2))
    return sequence


def build_function_from_basis(basis):
    result = 0
    for i in range(len(basis)):
        current_a = sympy.Symbol("a" + str(i))
        result += current_a * basis[i]
    return result


def show_plots(functions, start_x, end_x, dx, title):
    for function in functions:
        x_list = numpy.arange(start_x, end_x, dx)
        y_list = [function(p) for p in x_list]
        plt.plot(x_list, y_list)
        plt.title(title)
    plt.grid(True)
    plt.show()


def get_basis_function(n):
    if not n:
        return lambda x: 0
    return lambda x: x ** (n - 1) * (1 - x**2)


def get_basis_system(num_of_basis_functions):
    return [get_basis_function(n) for n in range(num_of_basis_functions)]


def numerical_diff(f, x, n):
    h = 0.001
    if not n:
        return f(x)
    elif n == 1:
        return (f(x + h) - f(x - h)) / (2 * h)
    elif n == 2:
        return (f(x - h) - 2 * f(x) + f(x + h)) / (h**2)
    else:
        raise NotImplementedError


def numerical_integration(f, a, b):
    n = 100
    dx = (b - a) / n
    xlist = numpy.arange(a, b, dx)
    ylist = [f(p) for p in xlist]
    return numpy.trapz(ylist, dx=dx)
