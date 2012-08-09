# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
"""
>>> from ambig6_ext import *

>>> f(1.0)
Traceback (most recent call last):
...
AmbiguousCall: Ambiguous call to Boost.Python function ambig6_ext.f
C++ signatures:
    f(int)
    f(bool)


>>> f(1.0, 1.0)
Traceback (most recent call last):
...
AmbiguousCall: Ambiguous call to Boost.Python function ambig6_ext.f
C++ signatures:
    f(float, int)
    f(float, bool)


>>> f(1, 1.0)
Traceback (most recent call last):
...
AmbiguousCall: Ambiguous call to Boost.Python function ambig6_ext.f
C++ signatures:
    f(float, int)
    f(float, bool)


>>> f(True, 1.0)
Traceback (most recent call last):
...
AmbiguousCall: Ambiguous call to Boost.Python function ambig6_ext.f
C++ signatures:
    f(float, int)
    f(float, bool)


>>> f(True, True, True)
Traceback (most recent call last):
...
AmbiguousCall: Ambiguous call to Boost.Python function ambig6_ext.f
C++ signatures:
    f(float, float, double)
    f(float, float, int)


>>> f(1.0, 1, True)
Traceback (most recent call last):
...
AmbiguousCall: Ambiguous call to Boost.Python function ambig6_ext.f
C++ signatures:
    f(float, float, double)
    f(float, float, int)


>>> f(1.0, 1.0, True)
Traceback (most recent call last):
...
AmbiguousCall: Ambiguous call to Boost.Python function ambig6_ext.f
C++ signatures:
    f(float, float, double)
    f(float, float, int)
"""
