from bitarray import bitarray


def get_bits(text):
    bits = bitarray(endian="big")
    bits.frombytes(text.encode("utf-8"))
    return bits


def add_padding(bits):
    bits.append(1)
    while len(bits) % 512 != 448:
        bits.append(0)
    return bitarray(bits, endian="big")


def append_k(bits, k):
    bits.extend(bin(k)[2:].zfill(64))
    return bits


def bits_to_int(bits):
    n = []
    for i in range(len(bits)//32):
        tmp = bits[:32]
        n.append(int.from_bytes(tmp.tobytes(), byteorder="big"))
        bits = bits[32:]
    return n


def get_hash(ints):
    A = 0x67452301
    B = 0xEFCDAB89
    C = 0x98BADCFE
    D = 0x10325476
    E = 0xC3D2E1F0
    def F1(x, y, z): return (x & y) | (~x & z)
    def F2(x, y, z): return x ^ y ^ z
    def F3(x, y, z): return (x & y) | (x & z) | (y & z)
    def shift_left_by_n(x, n): return (x << n) | (x >> (32 - n))
    def sum_mod_2_32(a, b): return (a + b) % pow(2, 32)

    for i in range(len(ints)//16):
        X = ints[0:16]
        ints = ints[16:]

        w = [0 for w in range(80)]
        for k in range(80):
            if 0 <= k <= 15:
                w[k] = X[k]
            if 16 <= k <= 79:
                w[k] = shift_left_by_n(
                    (w[k-3] ^ w[k-8] ^ w[k-14] ^ w[k-16]), 1) % pow(2, 32)

        a = A
        b = B
        c = C
        d = D
        e = E

        for j in range(80):
            if 0 <= j <= 19:
                k = 0x5A827999
                temp = F1(b, c, d)
            elif 20 <= j <= 39:
                k = 0x6ED9EBA1
                temp = F2(b, c, d)
            elif 40 <= j <= 59:
                k = 0x8F1BBCDC
                temp = F3(b, c, d)
            elif 60 <= j <= 79:
                k = 0xCA62C1D6
                temp = F2(b, c, d)

            temp = sum_mod_2_32(shift_left_by_n(a, 5), temp)
            temp = sum_mod_2_32(temp, e)
            temp = sum_mod_2_32(temp, k)
            temp = sum_mod_2_32(temp, w[j])
            e = d
            d = c
            c = shift_left_by_n(b, 30)
            b = a
            a = temp

        A = sum_mod_2_32(A, a)
        B = sum_mod_2_32(B, b)
        C = sum_mod_2_32(C, c)
        D = sum_mod_2_32(D, d)
        E = sum_mod_2_32(E, e)
    return A, B, C, D, E


def sha1(text):
    bits = get_bits(text)
    length = len(bits) % pow(2, 64)
    bits = add_padding(bits)
    bits = append_k(bits, length)

    intArray = bits_to_int(bits)
    h0, h1, h2, h3, h4 = get_hash(intArray)
    h0_bin = bin(h0)[2:].zfill(32)
    h1_bin = bin(h1)[2:].zfill(32)
    h2_bin = bin(h2)[2:].zfill(32)
    h3_bin = bin(h3)[2:].zfill(32)
    h4_bin = bin(h4)[2:].zfill(32)
    hash = h0_bin + h1_bin + h2_bin + h3_bin + h4_bin
    tmp = bitarray(endian="big")
    tmp.extend(hash)
    res = ''
    for i in range(len(tmp)//4):
        res += hex(int.from_bytes(tmp[:4].tobytes(), byteorder="big"))[2:][:1]
        tmp = tmp[4:]
    return res
