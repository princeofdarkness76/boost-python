# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from function_objects2_ext import *
>>> x = X()
>>> x.y = 13
>>> x.add_to(10)
23
>>> x.y
13
>>> x.add_to("foo")
'foo13'
>>> v1 = Visitor1()
>>> v2 = Visitor2()
>>> x.accept(v1)
>>> x.y
39
>>> x.accept(v2)
>>> x.y
32
>>> std_plus_int(3,9)
12

  Phoenix actors
  
>>> plus(3.5,4.5)
8.0
>>> three()
3
'''
