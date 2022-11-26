import numpy
import matplotlib.pyplot as plt

print("Нелинейные уравнения\n")

EPS = 10.0**-5


def inp():
    expr = numpy.poly1d([1.0, -14.4921, 60.6959, -70.9238])
    expr = numpy.poly1d([1.0, -3.0, 3.0, -1.0])
    expr = numpy.poly1d([1.0, -1.0, 0.0, -1.0])
    expr = numpy.poly1d([11.0, -3.5, 44.0, -3341.405])
    return expr


(f) = inp()
print(f)


def build_plot():
    z = numpy.linspace(-10, 10)
    plt.plot(z, f(z))
    plt.plot(z, numpy.zeros(len(z)))
    plt.show()


build_plot()


def SturmSeq(f):
    arr = []
    arr.append(f)
    arr.append(numpy.polyder(f))
    while True:
        fn = -numpy.polydiv(arr[-2], arr[-1])[1]
        if fn.order > 0 or abs(fn[0]) > 0.0:
            arr.append(fn)
        else:
            break
    return arr


def N(stseq, x):
    if abs(f(x)) < EPS:
        raise ValueError("Число в N() является корнем")
    ans = 0
    for bound in range(1, len(stseq)):
        if stseq[bound](x) == 0.0:
            raise ValueError("SturmSeq[bound] ноль")
        if stseq[bound - 1](x) * stseq[bound](x) < 0:
            ans += 1
    return ans


def GetBounds(f, a, b):
    if (abs(f(a)) < EPS) or (abs(f(b)) < EPS):
        raise ValueError("Отрезок содержит корень")
    if N(Sturm, a) - N(Sturm, b) == 0:
        return []
    if N(Sturm, a) - N(Sturm, b) > 1:
        while True:
            M = a + (b - a) / (1.5+numpy.random.random())
            if abs(f(M)) > EPS:
                break
        return GetBounds(f, a, M) + GetBounds(f, M, b)
    if b - a < EPS:
        print("Внимание: Отрезок слишком мал")
    return [(a, b)]


Sturm = SturmSeq(f)

print("Количество корней на отрезке [-10, 10]:")
print(N(Sturm, -10) - N(Sturm, 10))

bounds = GetBounds(f, -10, 10)
print("Корни на отрезках:")
print(bounds)


iters = 0


def BinarySearch(L, R):
    global iters
    iters += 1
    M = (L + R) / 2
    if R - L < EPS:
        return M
    if f(L) * f(M) <= 0:
        return BinarySearch(L, M)
    if f(R) * f(M) <= 0:
        return BinarySearch(M, R)
    raise RuntimeError("Что-то пошло не так в бинарном поиске")


def ChordMethod(L, R):
    global iters
    fder2 = numpy.polyder(f, 2)

    if f(R) * fder2(R) > 0:
        (oldx, x) = (R, L)
    elif f(L) * fder2(L) > 0:
        (oldx, x) = (L, R)
    else:
        raise ValueError("Плохой отрезок в методе хорд")
    t = oldx
    while abs(x - oldx) > EPS:
        iters += 1
        oldx = x
        x = x - f(x) * (t - x) / (f(t) - f(x))
        if not (numpy.isfinite(x)):
            raise RuntimeError("Что-то пошло не так, и x не число")
    if (x < L) or (R < x):
        raise RuntimeError("Что-то пошло не так, и x не лежит в [L, R]")
    return x


def Newton(L, R):
    global iters
    fder = numpy.polyder(f)
    fder2 = numpy.polyder(f, 2)
    if f(L) * fder2(L) > 0:
        (oldx, x) = (R, L)
    elif f(R) * fder2(R) > 0:
        (oldx, x) = (L, R)
    else:
        raise ValueError("Плохие границы в методе Ньютона")
    while abs(x - oldx) > EPS:
        iters += 1
        oldx = x
        x = x - f(x) / fder(x)
        if not (numpy.isfinite(x)):
            raise RuntimeError("Что-то пошло не так, и x не число")
    if (x < L) or (R < x):
        raise RuntimeError("Что-то пошло не так, и x не лежит в [L, R]")
    return x


numpy.set_printoptions(suppress=True, precision=4, floatmode="fixed")


def test(method):
    global iters
    for bound in bounds:
        iters = 0
        try:
            result = method(*bound)
            if not result is None:
                result = "{:.4f}".format(result)
            print(
                f"{result} с помощью {method.__name__} метода (с {iters} итерациями)")
        except Exception as ex:
            print(
                f"ОШИБКА: {ex} - в {method.__name__} методе (с {iters} итерациями)")


print()
test(BinarySearch)
print()
test(ChordMethod)
print()
test(Newton)
print()

print("Проверка:")
print(f.r)
