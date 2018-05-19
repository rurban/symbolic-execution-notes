#!/usr/bin/python
import sys
from z3 import *

s = Solver()

sn = IntVector('sn', 11)

def is_valid(x):
  return Or(And(x >= 0x41, x <= 0x5a), And(x >= 0x30, x <= 0x39))

for i in range(11):
  if (i != 5):
    s.add(is_valid(sn[i]))

# constraint 1
s.add(sn[5] == 0x2d)

# constraint 2
s.add(sn[0] == 0x31)
s.add(sn[6] == 0x35)

# constraint 3
s.add(sn[1] >= 0x30, sn[1] <= 0x39)
s.add(sn[7] >= 0x30, sn[7] <= 0x39)

# constraint 4
s.add(sn[2] >= 0x41, sn[2] <= 0x5a)
s.add(sn[8] >= 0x41, sn[8] <= 0x5a)

# constraint 5
s.add(sn[3] >= 0x30, sn[3] <= 0x39)
s.add(sn[4] >= 0x30, sn[4] <= 0x39)
s.add(sn[9] >= 0x30, sn[9] <= 0x39)
s.add(sn[10] >= 0x30, sn[10] <= 0x39)

# constraint 6
s.add(sn[3] == sn[4])
s.add(sn[9] == sn[10])

if s.check() == unsat:
  print "UNSAT"
  sys.exit()

m = s.model()

for x in m:
  print x, chr(m[x].as_long())
