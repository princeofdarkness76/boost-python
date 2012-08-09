# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from implicit_ext import *
>>> x_value(X(42))
42
>>> x_value(42)
42
>>> x = make_x(X(42))
>>> x.value()
42
>>> make_x('fool')
Traceback (most recent call last):
...
Boost.Python.ArgumentError: Python argument types in
    implicit_ext.make_x(str)
did not match C++ signature:
    make_x(int)

>>> x_value.__doc__.splitlines()[1]
'x_value( (X)arg1) -> int :'

>>> make_x.__doc__.splitlines()[1]
'make_x( (object)arg1) -> X :'

>>> X.value.__doc__.splitlines()[1]
'value( (X)arg1) -> int :'

>>> X.set.__doc__.splitlines()[1]
'set( (X)arg1, (object)arg2) -> None :'

'''
