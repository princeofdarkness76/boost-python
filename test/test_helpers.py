#!/usr/bin/env python
#
# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Test the testers themselves.
#
s = '''
>>> 
>>> def f(x):
...     raise ZeroDivisionError
>>> thunk = shouldthrow(ZeroDivisionError, f)
>>> thunk(7)
>>> thunk = shouldthrow(TypeError, f)
>>> thunk(7)
Traceback (most recent call last):
  ...
ZeroDivisionError
>>> def g(x):
...     return x
>>> thunk = shouldthrow(ZeroDivisionError, g)
>>> thunk(17)
Expected a ZeroDivisionError exception
'''

from bpl_doctest import * ; main(s)

