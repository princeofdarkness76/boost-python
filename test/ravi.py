# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from ravi_ext import *
>>> s = S()
>>> s.set(13)
>>> s.show()
'13'
>>> s.set(None)
>>> s.show()
'<unset>'
>>> s.set(None)
>>> s.add(13)
>>> s.show()
'<unset>'
>>> s.set(13)
>>> s.add(13)
>>> s.show()
'26'
'''

