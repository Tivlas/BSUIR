import hashlib
import gost3411
import random


def point_add(a, b, p, P1, P2):
    x1, y1 = P1
    x2, y2 = P2

    if (x1, y1) == (0, 0):
        return P2
    if (x2, y2) == (0, 0):
        return P1
    if x1 == x2 and (y1 != y2 or y1 == 0):
        return (0, 0)

    if x1 == x2 and y1 == y2:
        m = (3 * x1 ** 2 + a) * minverse(2 * y1, p) % p
    else:
        m = (y2 - y1) * minverse(x2 - x1, p) % p

    x3 = (m ** 2 - x1 - x2) % p
    y3 = (m * (x1 - x3) - y1) % p
    return (x3, y3)


def point_mul(a, b, p, P, n):
    result = (0, 0)
    addend = P

    for bit in bits(n):
        if bit == 1:
            result = point_add(a, b, p, result, addend)
        addend = point_add(a, b, p, addend, addend)
    return result


def bits(number):
    while number:
        yield number & 1
        number >>= 1


def euc_ext(a, b):
    x, x_, y, y_ = 1, 0, 0, 1
    while b:
        q = a // b
        a, b = b, a % b
        x, x_ = x_, x - x_ * q
        y, y_ = y_, y - y_ * q
    return x, y


def minverse(a, b):
    x, _ = euc_ext(a, b)
    return (x + b) % b


def sign_message(d, message, q, P, a, b, p):
    h = gost3411.gost3411(message)
    z = int.from_bytes(h, byteorder='big')
    e = z % q
    if e == 0:
        e = 1

    while True:
        k = random.randint(1, q - 1)
        C = point_mul(a, b, p, P, k)
        r = C[0] % q
        if r == 0:
            continue
        s = (r * d + k * e) % q
        if s != 0:
            break

    return r, s


def verify_signature(Q, message, signature, q, P, a, b, p):
    r, s = signature
    if not (0 < r < q and 0 < s < q):
        return False

    h = gost3411.gost3411(message)
    z = int.from_bytes(h, byteorder='big')
    e = z % q
    if e == 0:
        e = 1

    v = minverse(e, q)
    z1 = s * v % q
    z2 = -r * v % q

    C = point_add(a, b, p, 
                  point_mul(a, b, p, P, z1), 
                  point_mul(a, b, p, Q, z2))
    return C[0] % q == r


if __name__ == '__main__':
    p = 0x8000000000000000000000000000000000000000000000000000000000000431
    a = 0x7
    b = 0x5FBFF498AA938CE739B8E022FBAFEF40563F6E6A3472FC2A514C0CE9DAE23B7E
    q = 0x8000000000000000000000000000000150FE8A1892976154C59CFC193ACCF5B3
    P = (0x2, 0x8E2A8A0E65147D4BD6316030E16D19C85C97F0A9CA267122B96ABBCEA7E8FC8)

    d = 0x7A929ADE789BB9BE10ED359DD39A72C11B60961F49397EEE1D19CE9891EC3B28
    Q = point_mul(a, b, p, P, d)

    message = "привет world 123 #$%^"
    signature = sign_message(d, message.encode(), q, P, a, b, p)
    is_correct = verify_signature(Q, message.encode(), signature, q, P, a, b, p)

    print(f"Message: {message}")
    print("Signature:")
    print(f"{signature[0]:x}")
    print(f"{signature[1]:x}")
    print("Correct signature" if is_correct else "Wrong signature")
