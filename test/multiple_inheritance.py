#!/usr/bin/env python
#
#
# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
#
# Check that multiple inheritance from wrapped classes works.  Note
# that you have to call your base class constructors explicitly in the
# derived class' constructor... or multiple inheritance won't work.
#
#
"""
>>> from multiple_inheritance_ext import *
>>> class Derived(LeftClass, RightClass):
...     def __init__(self):
...         LeftClass.__init__(self)
...         RightClass.__init__(self)
... 
>>> obj = Derived()
>>> test_left(obj)
'Successfully converted to LeftClass'
>>> test_right(obj)
'Successfully converted to RightClass'

    Here's what happens if you don't initialize your base classes.

>>> class BuggyDerived(LeftClass, RightClass):
...     pass
... 
>>> buggy_obj = BuggyDerived()

    This succeeds, but I haven't investigated why.  It'd be nice if it
    didn't.

>>> test_left(buggy_obj)
'Successfully converted to LeftClass'

    Fail as expected

>>> test_right(buggy_obj)
Traceback (most recent call last):
...
Boost.Python.ArgumentError: Python argument types in
    multiple_inheritance_ext.test_right(BuggyDerived)
did not match C++ signature:
    test_right(RightClass)
>>> 
"""

