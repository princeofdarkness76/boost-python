# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from phoenix2_ext import *
>>> t = (1,2,3,4,5)
>>> lazy_len(t)
5
>>> r = [1,2,3,4,5,6,7]
>>> lazy_len(r)
7
>>> lazy_len("four")
4
>>> # extract_string('stringy')
'stringy'
>>> # extract_string(17)
Traceback (most recent call last):
...
TypeError: No registered converter was able to produce a C++ rvalue of type std::string from this Python object of type int
>>> check_string('stringy')
True
>>> check_string(17)
False
'''
