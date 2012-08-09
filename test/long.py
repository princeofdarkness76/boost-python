# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

'''
>>> from long_ext import *
>>> print(new_long())
0
>>> print(longify(42))
42
>>> print(longify_string('300'))
300
>>> is_long(20L)
'yes'
>>> is_long('20')
0
>>> x = Y(4294967295L)
'''

def should_run(version):
    return version < 300

