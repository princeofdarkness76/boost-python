# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
"""
>>> from ambig5_ext import *
>>> f(True,True)
'float,bool'
>>> f(1.0,1.0)
'float,float'
>>> f(True)
'bool'
>>> f(1)
'int'
>>> f(1.0)
'float'
>>> f('1')
'std::string'
>>> f(1, 1.0)
'int,float'
>>> f(1.0, 1)
'float,int'
"""
