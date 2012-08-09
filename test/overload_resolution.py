# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
"""
>>> from overload_resolution_ext import *
>>> bid(True)
'bool'
>>> bid(1)
'int'
>>> bid(1.0)
'float'
>>> dib(True)
'bool'
>>> dib(1)
'int'
>>> dib(1.0)
'float'
>>> idb(True)
'bool'
>>> idb(1)
'int'
>>> idb(1.0)
'float'
>>> bdi(True)
'bool'
>>> bdi(1)
'int'
>>> bdi(1.0)
'float'
"""
