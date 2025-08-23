from sha1 import sha1
from gost3411 import gost3411
import hashlib

text = "Hello мир 1234 !@#$%^&*"
text = ""

res = sha1(text)
assert res == hashlib.sha1(text.encode()).hexdigest()
print("sha1: " + res)

print("gost3411: " + gost3411(text))
