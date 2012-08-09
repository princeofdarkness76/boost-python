#!/usr/bin/env python
#
#
# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
#
# Check that virtual inheritance works
#
"""
>>> from virtual_inheritance_ext import *

    Base is just base.

>>> b = base()
>>> b.f()
'base::f()'
>>> b.g()
'base::g()'

    Derived is just derived.
 
>>> d = derived()
>>> d.f()
'derived::f()'
>>> d.g()
'derived_wrapper::default_g() derived::g()'

    Inherit from base, don't redefine f()

>>> class B1(base):
...     def __init__(self):
...         base.__init__(self)
... 
>>> b = B1()
>>> b.f()
'base::f()'
>>> call_base_f(b)
'base::f()'
>>> call_base_g(b)
'base::g()'

    Inherit from base and implement f() only
   
>>> class B2(base):
...     def __init__(self):
...         base.__init__(self)
...     def f(self):
...         return 'B2.f'
... 
>>> b = B2()
>>> b.f()
'B2.f'
>>> call_base_f(b)
'B2.f'
>>> call_base_g(b)
'base::g()'

    Inherit from derived, don't override f() 

>>> class D1(derived):
...     def __init__(self):
...         derived.__init__(self)
... 
>>> d1 = D1()
>>> d1.f()
'derived::f()'
>>> call_derived_f(d1)
'derived::f()'
>>> call_base_f(d1)
'derived::f()'

    Inherit from derived, do override f()

>>> class D2(derived):
...     def __init__(self):
...         derived.__init__(self)
...     def f(self):
...         return 'D2.f'
... 
>>> d2 = D2()
>>> d2.f()
'D2.f'
>>> call_base_f(d2)
'D2.f'
>>> call_derived_f(d2)
'D2.f'
>>>
"""
