from z3 import *

def func1(a1, a2):
  s.add(a1 * 2 * (a2 ^ a1) - a2 == 10858)
  s.add(a1 > 85)
  s.add(a1 <= 95)
  s.add(a2 > 96 )
  s.add(a2 <= 111)

def func2(a1, a2):
  s.add(a1 % a2 == 7)
  s.add(a2 > 90)

def func3(a1, a2):
  s.add(a1 / a2 + (a2 ^ a1) == 21)
  s.add(a1 <= 99 )
  s.add(a2 <= 119)

def func4(a1, a2):
  v2 = a1 << 32 | a1
  s.add((v2 % a2) + a1 == 137)
  s.add(a1 > 115)
  s.add(a2 <= 99)
  s.add(a2 == 95)

def func5(a1, a2):
  s.add((a1 + a2) ^ a2 == 225)
  s.add(a1 > 90 )
  s.add(a2 <= 89 )

def func6(a1, a2, a3):
  s.add(a1 <= a2)
  s.add(a2 <= a3)
  s.add(a1 > 85)
  s.add(a2 > 110)
  s.add(a3 > 115)
  s.add(((a2 + a3) ^ (a1 + a2)) == 44)
  s.add(URem(a2 + a3, a1) + a2 == 161 ) # Use the function URem() for unsigned remainder, and SRem() for signed remainder. https://z3prover.github.io/api/html/classz3py_1_1_bit_vec_ref.html

def func7(a1, a2, a3):
  s.add(a1 >= a2)
  s.add(a2 >= a3)
  s.add(a1 <= 119)
  s.add(a2 > 90)
  s.add(a3 <= 89)
  s.add(((a1 + a3) ^ (a2 + a3)) == 122 )
  s.add(URem(a1 + a3,  a2) + a3 == 101)

def func8(a1, a2, a3):
  s.add(a1 <= a2)
  s.add(a2 <= a3)
  s.add( a3 <= 114)
  s.add( UDiv(a1 + a2, a3) * a2 == 97 ) # Use the function UDiv() for unsigned division. https://z3prover.github.io/api/html/classz3py_1_1_bit_vec_ref.html
  s.add( a3 ^ (a1 - a2) == -8*13)

def func9(a1, a2, a3):
  s.add(a1 == a2)
  s.add(a2 >= a3)
  s.add(a3 <= 99)
  s.add(a3 + a1 * (a3 - a2) - a1 == -1443)

def func10(a1, a2, a3):
  s.add(a1 >= a2)
  s.add(a2 >= a3)
  s.add(a2 * (a1 + a3 + 1) - a3 == 15514)
  s.add(a2 > 90)
  s.add(a2 <= 99)

def func11(a1, a2, a3):
  s.add(a2 >= a1)
  s.add(a1 >= a3)
  s.add(a2 > 100)
  s.add(a2 <= 104)
  s.add(a1 + (a2 ^ (a2 - a3)) - a3 == 70)
  s.add(UDiv(a2 + a3, a1) + a1 == 68 )

def func12(a1, a2, a3):
  s.add(a1 >= a2)
  s.add(a2 >= a3)
  s.add(a2 <= 59)
  s.add(a3 <= 44)
  s.add(a1 + (a2 ^ (a3 + a2)) - a3 == 111)
  s.add((a2 ^ (a2 - a3)) + a2 == 101 )

def func13(a1, a2, a3):
  s.add(a1 <= a2)
  s.add(a2 <= a3)
  s.add(a1 > 40 )
  s.add(a2 > 90 )
  s.add(a3 <= 109)
  s.add(a3 + (a2 ^ (a3 + a1)) - a1 == 269)
  s.add((a3 ^ (a2 - a1)) + a2 == 185)

def func14(a1, a2, a3):
  s.add(a1 >= a3)
  s.add(a2 >= a3)
  s.add(a2 <= 99)
  s.add(a3 > 90)
  s.add(a1 + (a2 ^ (a2 + a1)) - a3 == 185 )

def func15(a1, a2, a3):
  s.add(a2 >= a3)
  s.add(a2 >= a1)
  s.add(a3 > 95)
  s.add( a2 <= 109)
  s.add(((a2 - a1) * a2 ^ a3) - a1 == 1214)
  s.add(((a3 - a2) * a3 ^ a1) + a2 == -1034)

def is_valid(x):
  return Or(And(x >= 65, x <= 90), And(x >= 97, x <= 122), And(x >= 48, x <= 57), And(x == 95), And(x == 33), And(x == 64), And(x == 35), And(x == 36), And(x == 37), And(x == 94), And(x == 38), And(x == 42), And(x == 63), And(x == 58), And(x == 40), And(x == 41))

s = Solver()

x = [ BitVec('a%i' % i, 32) for i in range(26) ]

for i in range(26):
  s.add(is_valid(x[i]))

func1(x[0], x[1])
func2(x[1], x[2])
func3(x[2], x[3])
func4(x[3], x[4])
func5(x[4], x[5])
func6(x[5], x[6], x[7])
func7(x[7], x[8], x[9])
func8(x[9], x[10], x[11])
func9(x[11], x[12], x[13])
func10(x[13], x[14], x[15])
func11(x[15], x[16], x[17])
func12(x[17], x[18], x[19])
func13(x[19], x[20], x[21])
func14(x[21], x[22], x[23])
func15(x[23], x[24], x[25])

while s.check() == sat:
  solution = s.model()
  block = []
  for d in solution:
    c = d()
    block.append(c != solution[d])
    flag = ""
    for n in x:
      char = solution[n].as_long()
      flag += chr(char)
    print flag
  s.add(Or(block))
