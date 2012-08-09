# Copyright David Abrahams 2004. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
'''
>>> from m1 import *

>>> from m2 import *

   Prove that we get an appropriate error from trying to return a type
   for which we have no registered to_python converter

>>> make_unregistered(1)
Traceback (most recent call last):
TypeError: No to_python (by-value) converter found for C++ type: Unregistered

>>> make_unregistered2(1)
Traceback (most recent call last):
TypeError: No Python class registered for C++ class Unregistered

>>> n = new_noddy()
>>> s = new_simple()
>>> unwrap_int(n)
42
>>> unwrap_int_ref(n)
42
>>> unwrap_int_const_ref(n)
42
>>> unwrap_simple(s)
'hello, world'
>>> unwrap_simple_ref(s)
'hello, world'
>>> unwrap_simple_const_ref(s)
'hello, world'
>>> unwrap_int(5)
5

Cannot get a non-const reference to a built-in integer object
>>> unwrap_int_ref(7)  # hrm, on py3k that is m1.unwrap_int_ref :/
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    m2.unwrap_int_ref(int)
did not match C++ signature:
    unwrap_int_ref(int {lvalue})
>>> unwrap_int_const_ref(9)
9

>>> wrap_int(n)
42

>>> wrap_int_ref(n)
42

>>> wrap_int_const_ref(n)
42

>>> unwrap_simple_ref(wrap_simple(s))
'hello, world'

>>> unwrap_simple_ref(wrap_simple_ref(s))
'hello, world'

>>> unwrap_simple_ref(wrap_simple_const_ref(s))
'hello, world'

>>> f(s)
12

>>> unwrap_simple(g(s))
'hello, world'

>>> f(g(s))
12

>>> f_mutable_ref(g(s))
12

>>> f_const_ptr(g(s))
12

>>> f_mutable_ptr(g(s))
12

>>> f2(g(s))
12

Create an extension class which wraps "complicated" (init1 and get_n)
are a complicated constructor and member function, respectively.

>>> c1 = complicated(s, 99)
>>> c1.get_n()
99
>>> c2 = complicated(s)
>>> c2.get_n()
0

     a quick regression test for a bug where None could be converted
     to the target of any member function. To see it, we need to
     access the __dict__ directly, to bypass the type check supplied
     by the Method property which wraps the method when accessed as an
     attribute. 

>>> A.__dict__['name'](None)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    A.name(NoneType)
did not match C++ signature:
    name(A)

>>> a = A()
>>> b = B()
>>> c = C()
>>> d = D()


>>> take_a(a).name()
'A'

>>> take_b(a)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    m1.take_b(A)
did not match C++ signature:
    take_b(B {lvalue})

>>> take_c(a)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    m1.take_c(A)
did not match C++ signature:
    take_c(C*)

>>> take_d(a)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    m1.take_d(A)
did not match C++ signature:
    take_d(D*)

------
>>> take_a(b).name()
'A'

>>> take_b(b).name()
'B'

>>> take_c(b)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    m1.take_c(B)
did not match C++ signature:
    take_c(C*)

>>> take_d(b)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    m1.take_d(B)
did not match C++ signature:
    take_d(D*)


>>> take_a(c).name()
'A'

>>> take_b(c)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    m1.take_b(C)
did not match C++ signature:
    take_b(B {lvalue})

>>> take_c(c).name()
'C'

>>> take_d(c)
Traceback (most recent call last):
Boost.Python.ArgumentError: Python argument types in
    m1.take_d(C)
did not match C++ signature:
    take_d(D*)



-------
>>> take_a(d).name()
'A'
>>> take_b(d).name()
'B'
>>> take_c(d).name()
'C'
>>> take_d(d).name()
'D'

>>> take_d_shared_ptr(d).name()
'D'

>>> d_as_a = d_factory()
>>> dd = take_d(d_as_a)
>>> dd.name()
'D'
>>> g.__doc__.splitlines()[1]
'g( (Simple)arg1) -> Simple :'

'''
