# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from multi_arg_constructor_ext import *

  be sure others dont exist

>>> a = A()
Traceback (most recent call last):
...
ArgumentError: Python argument types in
    A.__init__(A)
did not match C++ signature:
    __init__(_object*, double, double, double, double, double, double, double, double, double)
>>> a = A(1.0)
Traceback (most recent call last):
...
ArgumentError: Python argument types in
    A.__init__(A, float)
did not match C++ signature:
    __init__(_object*, double, double, double, double, double, double, double, double, double)
>>> a = A(1.0, 2.0)
Traceback (most recent call last):
...
ArgumentError: Python argument types in
    A.__init__(A, float, float)
did not match C++ signature:
    __init__(_object*, double, double, double, double, double, double, double, double, double)
>>> a = A(1.0, 2, 3, 4, 5, 6, 7.0, 8.1, 9.3)
'''
