# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from enum_ext import *

>>> identity(color.red) # in case of duplicated enums it always take the last enum
enum_ext.color.blood

>>> identity(color.green)
enum_ext.color.green

>>> identity(color.blue)
enum_ext.color.blue

>>> identity(color(1)) # in case of duplicated enums it always take the last enum
enum_ext.color.blood

>>> identity(color(2))
enum_ext.color.green

>>> identity(color(3))
enum_ext.color(3)

>>> identity(color(4))
enum_ext.color.blue

  --- check export to scope ---

>>> identity(red)
enum_ext.color.blood

>>> identity(green)
enum_ext.color.green

>>> identity(blue)
enum_ext.color.blue

>>> identity(1) # expect typeerror
Traceback (most recent call last):
  ...
Boost.Python.ArgumentError: Python argument types in
    enum_ext.identity(int)
did not match C++ signature:
    identity(color)

>>> c = colorized()
>>> c.x
enum_ext.color.blood
>>> c.x = green
>>> c.x
enum_ext.color.green
>>> red == blood
True
>>> red == green
False
>>> hash(red) == hash(blood)
True
>>> hash(red) == hash(green)
False
>>> import pickle
>>> p = pickle.dumps(color.green, pickle.HIGHEST_PROTOCOL)
>>> l = pickle.loads(p)
>>> identity(l)
enum_ext.color.green
'''

