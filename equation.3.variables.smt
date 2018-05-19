(declare-const x Int)
(declare-const y Int)
(declare-const z Int)

(assert(=(+(- x (* 2 y)) (* 3 z)) 7))
(assert(=(+(+(* 2 x) y) z) 4))
(assert(=(-(-(* 2 y) (* 3 x)) (* 2 z)) -10))

(check-sat)
(get-model)
