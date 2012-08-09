# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from raw_ctor_ext import *
>>> import sys
>>> f = Foo(1, 2, 'a', bar = 3, baz = 4)
>>> f.args
(1, 2, 'a')
>>> list(f.kw.items())   # convert to list so that py2 and 3 look the same
[('bar', 3), ('baz', 4)]
'''

