# Copyright (C) 2003 Rational Discovery LLC.  Distributed under the Boost
# Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy
# at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> import crossmod_exception_a
>>> import crossmod_exception_b
>>> crossmod_exception_b.tossit()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: b-blah!
>>> crossmod_exception_a.tossit()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: a-blah!
>>>
'''

