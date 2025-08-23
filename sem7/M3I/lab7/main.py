import random
import binascii

# Параметры эллиптической кривой
p = 2 ** 256 - 2 ** 32 - 2 ** 9 - 2 ** 8 - 2 ** 7 - 2 ** 6 - 2 ** 4 - 1
G = (0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798,
     0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8)
n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F

def point_add(P, Q):
    if Q is None:
        return P

    if P[0] == Q[0] and P[1] == Q[1]:
        lambd = (3 * P[0] ** 2) * pow(2 * P[1], p - 2, p) % p
    else:
        lambd = (Q[1] - P[1]) * pow(Q[0] - P[0], p - 2, p) % p

    x = (lambd ** 2 - P[0] - Q[0]) % p
    y = (lambd * (P[0] - x) - P[1]) % p
    return (x, y)

def point_mul(P, n):
    if n == 0:
        return None

    Q = P
    R = None
    while n:
        if n & 1:
            R = point_add(R, Q) if R else Q
        Q = point_add(Q, Q)
        n //= 2
    return R

def generate_keys():
    private_key = random.randint(1, n - 1)
    public_key = point_mul(G, private_key)
    return private_key, public_key

def encrypt(message, public_key):
    encrypted = b''
    k = random.randint(1, n - 1)
    R = point_mul(G, k)
    for byte in message:
        m = (byte, 0)
        S = point_mul(public_key, k)
        c_x = (m[0] + S[0]) % p
        c_y = (m[1] + S[1]) % p
        encrypted += c_x.to_bytes(32, byteorder='big') + c_y.to_bytes(32, byteorder='big')
    return R, encrypted

def decrypt(R, encrypted, private_key):
    decrypted = b''
    for i in range(0, len(encrypted), 64):
        c_x = int.from_bytes(encrypted[i:i + 32], byteorder='big')
        c_y = int.from_bytes(encrypted[i + 32:i + 64], byteorder='big')
        c = (c_x, c_y)
        S = point_mul(R, private_key)
        m_x = (c[0] - S[0]) % p
        m_y = (c[1] - S[1]) % p
        m = (m_x, m_y)
        decrypted += bytes([m[0]])
    return decrypted.decode('utf-8')

def main():
    plaintext = ('русский english 1234 $#%#$#@!@! \n sfa')
    private_key, public_key = generate_keys()
    R, ciphertext = encrypt(plaintext.encode('utf-8'), public_key)
    decrypted = decrypt(R, ciphertext, private_key)
    print(f'Открытый ключ: {public_key[0]}, {public_key[1]}')
    print(f'Закрытый ключ: {private_key}')
    print(
        f'Зашифрованное сообщение: {binascii.hexlify(R[0].to_bytes(32, byteorder="big") + R[1].to_bytes(32, byteorder="big") + ciphertext)}')
    print(f'Расшифрованное сообщение: {decrypted}')

if __name__ == "__main__":
    main()
