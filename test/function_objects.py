# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from function_objects_ext import *
>>> x = X()
>>> x.y = 13
>>> x.fobj(12)
25
>>> x.bf1()
13
>>> x.bf2(2)
26
>>> x.bf3(2, -27)
-1
>>> x.add_to_int(10)
23
>>> x.add_to_double(20.5)
33.5
'''
