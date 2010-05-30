#!/usr/bin/guile -s
!#

;; test GSL vectors

; make a 3-vector
(define a (make-gsl-vector 3))

; set elements to 1.0
(set-all-gsl-vector a 1.0)
(display a)(newline)

; get index 1 element
(define a11 (get-gsl-vector a 1))
(display a11)(newline)(newline)

; set elements to 0.0
(zero-gsl-vector a)
(display a)(newline)

; get index 1 element
(define a11 (get-gsl-vector a 1))
(display a11)(newline)(newline)

;; test GSL matrices

; make a 3x3 matrix
(define A (make-gsl-matrix 3 3))

; set elements to 1.0
(set-all-gsl-matrix A 1.0)
(display A)(newline)

; get row 1 col 1 element
(define A11 (get-gsl-matrix A 1 1))
(display A11)(newline)(newline)

; set elements to 0.0
(zero-gsl-matrix A)
(display A)(newline)

; get row 1 col 1 element
(define A11 (get-gsl-matrix A 1 1))
(display A11)(newline)(newline)
