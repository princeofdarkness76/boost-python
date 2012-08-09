# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
"""

  Note: "keyword argument repeated" not checked, this happens in the parser.

>>> from multiple_values_for_keyword_arg_ext import f
>>> f(1,2,3)
123
>>> f(x=1, y=2, z=3)
123
>>> f(y=2, x=1, z=3)
123
>>> f(z=3, y=2, x=1)
123
>>> f(1, z=3, y=2)
123
>>> f(1, y=2, z=3)
123
>>> f(1, 2, z=3)
123
>>> f(1, y=2, z=3, x=1)
Traceback (most recent call last):
...
TypeError: multiple_values_for_keyword_arg_ext.f() got multiple values for keyword argument 'x'
>>> f(1, 2, y=2, z=3, x=1)
Traceback (most recent call last):
...
TypeError: multiple_values_for_keyword_arg_ext.f() got multiple values for keyword argument 'y'
>>> f(1, 2, 3, z=3)
Traceback (most recent call last):
...
TypeError: multiple_values_for_keyword_arg_ext.f() got multiple values for keyword argument 'z'

>>> try: f(1, 2, 3, z=3)
... except(TypeError): 'ok'
...
'ok'
"""
