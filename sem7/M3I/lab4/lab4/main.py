import random
import numpy as np

G = np.array([[1, 1, 0, 1],
              [1, 0, 1, 1],
              [1, 0, 0, 0],
              [0, 1, 1, 1],
              [0, 1, 0, 0],
              [0, 0, 1, 0],
              [0, 0, 0, 1]])


H = np.array([[1, 0, 1, 0, 1, 0, 1],
              [0, 1, 1, 0, 0, 1, 1],
              [0, 0, 0, 1, 1, 1, 1]])


R = np.array([[0, 0, 1, 0, 0, 0, 0],
              [0, 0, 0, 0, 1, 0, 0],
              [0, 0, 0, 0, 0, 1, 0],
              [0, 0, 0, 0, 0, 0, 1]])

S = np.array([[1, 0, 0, 0],
              [0, 1, 0, 0],
              [0, 0, 1, 0],
              [1, 0, 0, 1]])
SI = np.linalg.inv(S).astype(int)

P = np.array([[1, 0, 0, 0, 0, 0, 0],
              [0, 1, 0, 0, 0, 0, 0],
              [0, 0, 1, 0, 0, 0, 0],
              [0, 0, 0, 1, 0, 0, 0],
              [0, 0, 0, 0, 1, 0, 0],
              [0, 0, 0, 0, 0, 1, 0],
              [0, 0, 0, 0, 0, 0, 1]])
PI = np.linalg.inv(P).astype(int)

G_ = np.transpose(np.mod((S.dot(np.transpose(G))).dot(P), 2))

def encode(p_str):
    return np.mod(G_.dot(np.array([int(x) for x in p_str])), 2)


def decode(c):
    return np.mod(R.dot(c), 2)


def flip(bits, n):
    bits[n] = (bits[n] + 1) % 2


def add_error(bits):
    error = bits * 0
    idx = random.randint(0, 6)
    error[idx] = 1
    return np.mod(bits + error, 2)

def get_error_index(bits):
    mul = np.mod(H.dot(bits), 2)
    mul = mul[::-1]
    err_idx = int(''.join(str(bit) for bit in mul), 2)
    return err_idx - 1

def split_bits(str, n):
    return [str[i:i + n] for i in range(0, len(str), n)]


def bits_to_str(bits):
    bytes = bytearray(int(chunk, 2) for chunk in [bits[i:i + 8] for i in range(0, len(bits), 8)])
    text = bytes.decode("utf-8", errors="replace")
    return  text.replace('\n', '')


if __name__ == '__main__':
    with open("i", "rb") as f:
        text = f.read()
    binary_str = ''.join(format(x, '08b') for x in text)

    bits = split_bits(binary_str, 4)
    encrypted_text = []
    for b in bits:
        encrypted_text.append( ''.join(str(x) for x in add_error(encode(b))))

    encoded = ''.join(encrypted_text)
    with open("e", "w", encoding="utf-8") as f:
        f.write(encoded)

    decrypted_bits_str_arr = []
    for c in encrypted_text:
        c = np.array([int(x) for x in c])
        c_ = np.mod(c.dot(PI), 2)
        flip(c_, get_error_index(c_))
        m_ = decode(c_)
        decrypted_bits_str_arr.append(''.join(str(x) for x in np.mod(m_.dot(SI), 2)))

    decrypted_bits_str = ''.join(decrypted_bits_str_arr)
    decrypted_text = bits_to_str(decrypted_bits_str)
    with open("d", "w", encoding="utf-8") as f:
        f.write(decrypted_text)
