import numpy


def Norm3(A):
    n = A.shape[0]
    norm3 = max(numpy.absolute(A.T[j]).sum() for j in range(n))
    return norm3


A = numpy.array([[-1, -55, 7, -2]], dtype=float)
print(abs(A))
