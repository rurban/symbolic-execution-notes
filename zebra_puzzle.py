#!/usr/bin/env python
'''
My solution for: https://en.wikipedia.org/wiki/Zebra_Puzzle

The following version of the puzzle appeared in Life International in 1962:

0.  There are five houses.
1.  The Englishman lives in the red house.
2.  The Spaniard owns the dog.
3.  Coffee is drunk in the green house.
4.  The Ukrainian drinks tea.
5.  The green house is immediately to the right of the ivory house.
6.  The Old Gold smoker owns snails.
7.  Kools are smoked in the yellow house.
8.  Milk is drunk in the middle house.
9.  The Norwegian lives in the first house.
10. The man who smokes Chesterfields lives in the house next to the man with the fox.
11. Kools are smoked in the house next to the house where the horse is kept.
12. The Lucky Strike smoker drinks orange juice.
13. The Japanese smokes Parliaments.
14. The Norwegian lives next to the blue house.

Now, who drinks water? Who owns the zebra?

In the interest of clarity, it must be added that each of the five houses is painted a different color, and their inhabitants are of different national extractions, own different pets, drink different beverages and smoke different brands of American cigarets [sic]. One other thing: in statement 6, right means your right.
'''

from z3 import *

s = Solver()

zebra = {
         'color' : ('red', 'green', 'ivory', 'yellow', 'blue'),
         'nationality' : ('British', 'Spanish', 'Ukrainian', 'Norwegian', 'Japanese'),
         'pet' : ('dog', 'snail', 'fox', 'horse', 'zebra'),
         'beverage' : ('coffee', 'tea', 'milk', 'juice', 'water'),
         'cigarets' : ('Old Gold', 'Kools', 'Lucky Strike', 'Parliaments', 'Chesterfields')
	 }

# assign an integer/index to zebra[name] from 0 to 5
color, nationality, pet, beverage, cigarets = range(5) 

# Create Z3 integer variables for matrix cells
cells = [ [ Int("%s_%d" % (j, i)) for j in zebra ] for i in range(5) ]

# debug
#print(cells)

# Add cell constraints
for y in range(5):
  for x in range(5):
    s.add(cells[x][y] >= 0, cells[x][y] <= 4)
'''
# Add column constraints
for x in range(5):
  s.add(Distinct(cells[x]))
'''
# Add row constraints
for y in range(5):
  s.add(Distinct([cells[x][y] for x in range(5)]))


# feed the solver with the hints above

# The Englishman lives in the red house
s.add( Or( [ And(cells[i][nationality] == zebra['nationality'].index('British'), cells[i][color] == zebra['color'].index('red') ) for i in range(5) ] ) )

# The Spaniard owns the dog
s.add( Or( [ And(cells[i][nationality] == zebra['nationality'].index('Spanish'), cells[i][pet] == zebra['pet'].index('dog') ) for i in range(5) ] ) )

# Coffee is drunk in the green house
s.add( Or( [ And(cells[i][beverage] == zebra['beverage'].index('coffee'), cells[i][color] == zebra['color'].index('green') ) for i in range(5) ] ) )

# The Ukrainian drinks tea
s.add( Or( [ And(cells[i][nationality] == zebra['nationality'].index('Ukrainian'), cells[i][beverage] == zebra['beverage'].index('tea') ) for i in range(5) ] ) )

# The green house is immediately to the right of the ivory house
s.add( Or( [ And(cells[i][color] == zebra['color'].index('green'), cells[i - 1][color] == zebra['color'].index('ivory') ) for i in range(5) ] ) )

# The Old Gold smoker owns snails
s.add( Or( [ And(cells[i][cigarets] == zebra['cigarets'].index('Old Gold'), cells[i][pet] == zebra['pet'].index('snail') ) for i in range(5) ] ) ) 

# Kools are smoked in the yellow house
s.add( Or( [ And(cells[i][cigarets] == zebra['cigarets'].index('Kools'), cells[i][color] == zebra['color'].index('yellow') ) for i in range(5) ] ) )

# Milk is drunk in the middle house
s.add(cells[2][beverage] == zebra['beverage'].index('milk'))

# The Norwegian lives in the first house
s.add(cells[0][nationality] == zebra['nationality'].index('Norwegian'))

# The man who smokes Chesterfields lives in the house next to the man with the fox
s.add( Or(
  And(cells[0][cigarets] == zebra['cigarets'].index('Chesterfields'), cells[1][pet] == zebra['pet'].index('fox')),
  And(cells[1][cigarets] == zebra['cigarets'].index('Chesterfields'), Or(cells[0][pet] == zebra['pet'].index('fox'), cells[2][pet] == zebra['pet'].index('fox'))),
  And(cells[2][cigarets] == zebra['cigarets'].index('Chesterfields'), Or(cells[1][pet] == zebra['pet'].index('fox'), cells[3][pet] == zebra['pet'].index('fox'))),
  And(cells[3][cigarets] == zebra['cigarets'].index('Chesterfields'), Or(cells[2][pet] == zebra['pet'].index('fox'), cells[4][pet] == zebra['pet'].index('fox'))),
  And(cells[4][cigarets] == zebra['cigarets'].index('Chesterfields'), cells[3][pet] == zebra['pet'].index('fox')),
))

# Kools are smoked in the house next to the house where the horse is kept
s.add( Or(
  And(cells[0][cigarets] == zebra['cigarets'].index('Kools'), cells[1][pet] == zebra['pet'].index('horse')),
  And(cells[1][cigarets] == zebra['cigarets'].index('Kools'), Or(cells[0][pet] == zebra['pet'].index('horse'), cells[2][pet] == zebra['pet'].index('horse'))),
  And(cells[2][cigarets] == zebra['cigarets'].index('Kools'), Or(cells[1][pet] == zebra['pet'].index('horse'), cells[3][pet] == zebra['pet'].index('horse'))),
  And(cells[3][cigarets] == zebra['cigarets'].index('Kools'), Or(cells[2][pet] == zebra['pet'].index('horse'), cells[4][pet] == zebra['pet'].index('horse'))),
  And(cells[4][cigarets] == zebra['cigarets'].index('Kools'), cells[3][pet] == zebra['pet'].index('horse')),
))
 
# The Lucky Strike smoker drinks orange juice
s.add( Or( [ And(cells[i][cigarets] == zebra['cigarets'].index('Lucky Strike'), cells[i][beverage] == zebra['beverage'].index('juice') ) for i in range(5) ] ) )

# The Japanese smokes Parliaments
s.add( Or( [ And(cells[i][nationality] == zebra['nationality'].index('Japanese'), cells[i][cigarets] == zebra['cigarets'].index('Parliaments') ) for i in range(5) ] ) )

# The Norwegian lives next to the blue house
s.add( Or(
  And(cells[0][nationality] == zebra['nationality'].index('Norwegian'), cells[1][color] == zebra['color'].index('blue')),
  And(cells[1][nationality] == zebra['nationality'].index('Norwegian'), Or(cells[0][color] == zebra['color'].index('blue'), cells[2][color] == zebra['color'].index('blue'))),
  And(cells[2][nationality] == zebra['nationality'].index('Norwegian'), Or(cells[1][color] == zebra['color'].index('blue'), cells[3][color] == zebra['color'].index('blue'))),
  And(cells[3][nationality] == zebra['nationality'].index('Norwegian'), Or(cells[2][color] == zebra['color'].index('blue'), cells[4][color] == zebra['color'].index('blue'))),
  And(cells[4][nationality] == zebra['nationality'].index('Norwegian'), cells[3][color] == zebra['color'].index('blue'))
))

if s.check() == unsat:
  print('Failed to solve. Most likely some of the constraint are wrong.') 

m = s.model()
solution = [[m[case].as_long() for case in line] for line in cells]

for i in range(5):
  print('House: %s, Nationality: %s, Beverage: %s, Smoke: %s, Pet: %s' % (
    zebra['color'][solution[i][color]],
    zebra['nationality'][solution[i][nationality]],
    zebra['beverage'][solution[i][beverage]],
    zebra['cigarets'][solution[i][cigarets]],
    zebra['pet'][solution[i][pet]]
  ))

