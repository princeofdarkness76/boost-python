# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from back_reference_ext import *
>>> y = Y(3)
>>> z = Z(4)
>>> x_instances()
2
>>> y2 = copy_Y(y)
>>> x_instances()
3
>>> z2 = copy_Z(z)
>>> x_instances()
4
>>> assert y_identity(y) is y
>>> y_equality(y, y)
1

>>> print(y_identity.__doc__.splitlines()[1])
y_identity( (Y)arg1) -> object :
'''