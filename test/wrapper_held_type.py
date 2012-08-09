# Copyright David Abrahams 2005. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from wrapper_held_type_ext import *
>>> d = data()
>>> d.id()
42
>>> do_nothing( d )
>>> d.id()
42
>>> d = create_data()
>>> d.id()
42
>>> do_nothing( d )
>>> d.id()
42
'''

