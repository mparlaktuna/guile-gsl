;; test GSL vectors
(define a (make-gsl-vector 3))
(set-gsl-vector a 1.0)
(display a)(newline)
(zero-gsl-vector a)
(display a)(newline)

;; test GSL matrices
(define A (make-gsl-matrix 3 3))
(set-gsl-matrix A 1.0)
(display A )(newline)
(zero-gsl-matrix A)
(display A)(newline)
