# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from auto_ptr_ext import *
>>> x = X(42)
>>> x.value()
42
>>> look(x), look(x)
(42, 42)

>>> maybe_steal(x, 0)
42
>>> look(x)
42

>>> maybe_steal(x, 1)
42
>>> broken_auto_ptr and -1 or look(x)
-1

>>> x = X(69)
>>> steal(x)
69
>>> broken_auto_ptr and -1 or look(x)
-1

>>> x.value()
Traceback (most recent call last):
...
Boost.Python.ArgumentError: Python argument types in
    X.value(X)
did not match C++ signature:
    value(test_class<0>)

>>> x = make()
>>> look(x)
77

>>> z = callback(lambda z: z)
>>> z.value()
77

>>> extract(x).value()
77

#
# Test derived to base conversions
#

>>> y = Y(42)
>>> y.value()
42

>>> maybe_steal(y, 0)
Traceback (most recent call last):
...
Boost.Python.ArgumentError: Python argument types in
    auto_ptr_ext.maybe_steal(Y, int)
did not match C++ signature:
    maybe_steal(std::auto_ptr<test_class<0> > {lvalue}, bool)

>>> y.value()
42

>>> broken_auto_ptr and 42 or steal(y)
42

>>> y.value()
Traceback (most recent call last):
...
Boost.Python.ArgumentError: Python argument types in
    X.value(Y)
did not match C++ signature:
    value(test_class<0>)

>>> print(look.__doc__.splitlines()[1])
look( (X)arg1) -> int :

>>> print(steal.__doc__.splitlines()[1])
steal( (X)arg1) -> int :

>>> print(maybe_steal.__doc__.splitlines()[1])
maybe_steal( (X)arg1, (bool)arg2) -> int :

>>> print(make.__doc__.splitlines()[1])
make() -> X :

>>> print(callback.__doc__.splitlines()[1])
callback( (object)arg1) -> X :

>>> print(extract.__doc__.splitlines()[1])
extract( (object)arg1) -> X :

'''

