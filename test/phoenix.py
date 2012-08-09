# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from phoenix_ext import *
>>> three()
3
>>> plus(2.5,2.5)
5.0
>>> shazam()
'shazam'
>>> argtwo(1,2,3,4)
2
>>> factorial_of_sum(1,1)
2
>>> factorial_of_sum(1,2)
6
>>> factorial_of_sum(2,3)
120
>>> maybe_throw(0)
>>> maybe_throw(10)
Traceback (most recent call last):
...
RuntimeError: too big!!
>>> concatenate('lhs', 'rhs')
'lhsconcatrhs'
>>> x = X()
>>> x.x = 37
>>> x.add7()
>>> x.x
44
>>> x.add8()
>>> x.x
52
>>> x.add9()
>>> x.x
61
>>> plus_ints_to_double(3,4)
7.0
'''
