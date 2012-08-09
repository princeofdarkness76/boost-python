# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from list_ext import *

>>> new_list()
[]

>>> listify((1,2,3))
[1, 2, 3]

>>> letters = listify_string('hello')
>>> letters
['h', 'e', 'l', 'l', 'o']

>>> X(22)
X(22)

>>> def identity(x):
...     return x
>>> assert apply_object_list(identity, letters) is letters

  5 is not convertible to a list

>>> apply_object_list(identity, 5)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    list_ext.apply_object_list(function, int)
did not match C++ signature:
    apply_object_list(boost::python::api::object, boost::python::list)

>>> assert apply_list_list(identity, letters) is letters

  5 is not convertible to a list as a return value

>>> apply_list_list(len, letters)
Traceback (most recent call last):
...
TypeError: Expecting an object of type list; got an object of type int instead

>>> append_object(letters, '.')
>>> letters
['h', 'e', 'l', 'l', 'o', '.']

  tuples do not automatically convert to lists when passed as arguments
  
>>> append_list(letters, (1,2))
Traceback (most recent call last):
...
Boost.Python.ArgumentError: Python argument types in
    list_ext.append_list(list, tuple)
did not match C++ signature:
    append_list(boost::python::list {lvalue}, boost::python::list)

>>> append_list(letters, [1,2])
>>> letters
['h', 'e', 'l', 'l', 'o', '.', [1, 2]]

    Check that subclass functions are properly called
    
>>> class mylist(list):
...     def append(self, o):
...         list.append(self, o)
...         if not hasattr(self, 'nappends'):
...             self.nappends = 1
...         else:
...             self.nappends += 1
...
>>> l2 = mylist()
>>> append_object(l2, 'hello')
>>> append_object(l2, 'world')
>>> l2
['hello', 'world']
>>> l2.nappends
2

>>> def printer(*args):
...     for x in args:
...         print(x)
...     print ("\\n")
>>> y = X(42)
>>> exercise(letters, y, printer) #doctest: +NORMALIZE_WHITESPACE
after append:
['h', 'e', 'l', 'l', 'o', '.', [1, 2], X(42), 5, X(3)]
number of X(42) instances: 1
number of 5s: 1
after extend:
['h', 'e', 'l', 'l', 'o', '.', [1, 2], X(42), 5, X(3), 'x', 'y', 'z']
index of X(42) is: 7
index of 'l' is: 2
after inserting 666:
['h', 'e', 'l', 'l', 666, 'o', '.', [1, 2], X(42), 5, X(3), 'x', 'y', 'z']
inserting with object as index:
['h', 'e', 'l', 'l', 666, '---', 'o', '.', [1, 2], X(42), 5, X(3), 'x', 'y', 'z']
popping...
['h', 'e', 'l', 'l', 666, '---', 'o', '.', [1, 2], X(42), 5, X(3), 'x', 'y']
['h', 'e', 'l', 'l', 666, 'o', '.', [1, 2], X(42), 5, X(3), 'x', 'y']
['h', 'e', 'l', 'l', 666, 'o', '.', [1, 2], X(42), X(3), 'x', 'y']
removing X(42)
['h', 'e', 'l', 'l', 666, 'o', '.', [1, 2], X(3), 'x', 'y']
removing 666
['h', 'e', 'l', 'l', 'o', '.', [1, 2], X(3), 'x', 'y']
reversing...
['y', 'x', X(3), [1, 2], '.', 'o', 'l', 'l', 'e', 'h']
sorted:
['.', 'e', 'h', 'l', 'l', 'o', 'x', 'y']
reverse sorted:
['y', 'x', 'o', 'l', 'l', 'h', 'e', '.']
'''
