# Copyright Maximilian Matthe 2008
# Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
"""
>>> from return_pointee_value_ext import *
>>> a = returnIntWrapper()
>>> a
42
>>> get_value()
0.5
>>> gah = get_null_value()  # this returns None
>>> gah
>>> print(gah)
None
"""
