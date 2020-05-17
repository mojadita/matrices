# Gauss/Jordan implementation.


`matrices` is an implementation of the Gauss/Jordan
process to solve linear systems.  An application is
made to do polynomial regression of degrees greater
or equal than zero.

Several other programs implement other things:

* `cos` creates a matrix with the cosine transformation DFT
matrix.
* `gram_schmidt` generates an ortonormal basis from a set of
linearly independent vectors.  If the vector set given is not
linearly independent, the program zeros the vectors on the point
they depend linearly of the previous.
* `regr_polin` accepts a set of pairs of data and tries to best
fit a polynomial of degree specified on the command line.

This program is copyright (C) 1979-2020 Luis Colorado
<luiscoloradourcola@gmail.com> and distribution and use
is granted under the BSD license version 2
