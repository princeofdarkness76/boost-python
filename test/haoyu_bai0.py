# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
"""
>>> from haoyu_bai0_ext import *
>>> type(return_string())
<type 'str'>
>>> type(return_wstring())
<type 'unicode'>
>>> type(return_int())
<type 'int'>
>>> type(return_longlong())
<type 'long'>
>>> print return_wstring.__doc__
<BLANKLINE>
return_wstring() -> unicode :
<BLANKLINE>
    C++ signature :
        std::wstring return_wstring()
>>> print return_string.__doc__
<BLANKLINE>
return_string() -> str :
<BLANKLINE>
    C++ signature :
        std::string return_string()
>>> print return_int.__doc__
<BLANKLINE>
return_int() -> int :
<BLANKLINE>
    C++ signature :
        int return_int()
>>> print return_longlong.__doc__
<BLANKLINE>
return_longlong() -> long :
<BLANKLINE>
    C++ signature :
        long long return_longlong()
"""
