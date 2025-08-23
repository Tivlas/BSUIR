import numpy as np

pi = [i for i in range(512)]

A = np.random.randint(0, 10000000, size=(64, 64))


def X(a, b):
    return bytes(x ^ y for x, y in zip(a, b))


def S(data):
    return bytes([pi[b] for b in data])


def P(data):
    permuted_data = bytearray(64)
    for i in range(0, 64, 2):
        permuted_data[i] = data[i + 1]
        permuted_data[i + 1] = data[i]

    return bytes(permuted_data)


def L(data):
    data_bits = np.unpackbits(np.frombuffer(data, dtype=np.uint8))
    result_bits = np.zeros(512, dtype=int)
    for i in range(8):
        block_bits = data_bits[i*64:(i+1)*64]
        result_bits[i*64:(i+1)*64] = np.dot(A, block_bits) % 2
    result = np.packbits(result_bits).tobytes()
    return result


def KeySchedule(K, i):
    C = [123, 51, 52, 11, 52, 13, 24, 245, 84, 11, 212, 112]
    K = X(K, bytes([C[i]] * 64))
    K = S(K)
    K = P(K)
    K = L(K)
    return K

def E(K, m):
    state = X(K, m)
    for i in range(12):
        state = S(state)
        state = P(state)
        state = L(state)
        K = KeySchedule(K, i)
        state = X(state, K)
    return state

def g(N, m, h):
    K = X(h, N)
    K = S(K)
    K = P(K)
    K = L(K)
    t = E(K, m)
    t = X(h, t)
    G = X(t, m)
    return G



def gost3411(M):
    M = text_to_bytes(M)


    h = bytes(64)
    N = bytes(64)
    Sigma = bytes(64)

    while len(M) >= 64:
        m = M[-64:]
        h = g(N, m, h)
        N = (int.from_bytes(N, 'big') + 512) % (2**512)
        N = N.to_bytes(64, 'big')
        Sigma = (int.from_bytes(Sigma, 'big') +
                 int.from_bytes(m, 'big')) % (2**512)
        Sigma = Sigma.to_bytes(64, 'big')
        M = M[:-64]

    m = b'\x00' * (63 - len(M)) + b'\x01' + M
    h = g(N, m, h)
    N = (int.from_bytes(N, 'big') + len(M) * 8) % (2**512)
    N = N.to_bytes(64, 'big')
    Sigma = (int.from_bytes(Sigma, 'big') +
             int.from_bytes(m, 'big')) % (2**512)
    Sigma = Sigma.to_bytes(64, 'big')
    h = g(bytes(64), h, N)
    h = g(bytes(64), h, Sigma)

    return h.hex()



def text_to_bytes(text):
    return text.encode('utf-8')
