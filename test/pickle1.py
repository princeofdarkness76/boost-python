# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
r'''
>>> import pickle1_ext
>>> import pickle
>>> pickle1_ext.world.__module__
'pickle1_ext'
>>> pickle1_ext.world.__safe_for_unpickling__
1
>>> pickle1_ext.world.__name__
'world'
>>> pickle1_ext.world('Hello').__reduce__()
(<class 'pickle1_ext.world'>, ('Hello',))
>>> wd = pickle1_ext.world('California')
>>> pstr = pickle.dumps(wd)
>>> wl = pickle.loads(pstr)
>>> wd.greet()
'Hello from California!'
>>> wl.greet()
'Hello from California!'

>>> noop = pickle1_ext.noop()
>>> pickle.dumps(noop)
Traceback (most recent call last):
RuntimeError: Pickling of "pickle1_ext.noop" instances is not enabled (http://www.boost.org/libs/python/doc/v2/pickle.html)
'''
