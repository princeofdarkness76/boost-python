# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from injected_ext import *
>>> X(3,5).value() - (3+5)
0
>>> X(a=3,b=5,c=7).value() - (3*5*7)
0
>>> X().value()
1000
'''
