# Copyright Niall Douglas 2005.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
"""
>>> from voidptr_ext import *


    Check for correct conversion
    
>>> use(get())

    Check that None is converted to a NULL void pointer
    
>>> useany(get())
1
>>> useany(None)
0

    Check that we don't lose type information by converting NULL
    opaque pointers to None
    
>>> assert getnull() is None
>>> useany(getnull())
0

   Check that there is no conversion from integers ...
   
>>> try: use(0)
... except TypeError: pass
... else: print('expected a TypeError')

   ... and from strings to opaque objects
   
>>> try: use("")
... except TypeError: pass
... else: print('expected a TypeError')
"""
