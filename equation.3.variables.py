#!/usr/bin/python

from z3 import *

s = Solver()

x = Int('x')
y = Int('y')
z = Int('z')

s.add(x - 2*y + 3*z == 7)
s.add(2*x + y + z == 4)
s.add(-3*x + 2*y - 2*z == -10)

print s.check()
print s.model()
