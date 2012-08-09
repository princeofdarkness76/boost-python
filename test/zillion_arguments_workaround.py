#!/usr/bin/env python
# Copyright Troy D. Straszheim 2009.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
# Demonstrates how to wrap a function that has so many arguments
# that it is unwrappable, since type_traits chokes on it.
#
'''
>>> import zillion_arguments_workaround_ext as z
>>> z.f = lambda *args: z.f_impl(args)
>>> z.f(3,4)
12
'''
