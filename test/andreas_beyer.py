# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
 >>> from andreas_beyer_ext import *
 >>> b=B()
 >>> a=b.get() # let b create an A
 >>> a2=b.get()
 >>> assert id(a) == id(a2)
'''

