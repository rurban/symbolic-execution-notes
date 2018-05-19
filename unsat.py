#!/usr/bin/python

from z3 import *

s = Solver()

a = Int('a')
b = Int('b')

s.add(a > 0)
s.add(a + 1 == b)
s.add(b < 0)

if s.check() == unsat:
  print("NO SOLUTION!")
else:
  print s.model()
