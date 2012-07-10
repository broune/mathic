mathic
======

Mathic is a C++ library of fast data structures designed for use in
Groebner basis computation. This includes data structures for ordering
S-pairs, performing divisor queries and ordering polynomial terms
during polynomial reduction.

With Mathic you get to use highly optimized code with little effort so
that you can focus more of your time on whatever part of your Groebner
basis implementation that you are interested in. The data structures
use templates to allow you to use them with whatever representation of
monomials/terms and coefficients that your code uses. In fact the only
places where Mathic defines its own monomials/terms is in the test
code and example code. Currently only dense representations of
terms/monomials are suitable since Mathic will frequently ask "what is
the exponent of variable number x in this term/monomial?".

The paper "Practical Grobner Basis Computation" describes the data
structures from a high level. It was presented at ISSAC 2012 and is
available at http://arxiv.org/abs/1206.6940
