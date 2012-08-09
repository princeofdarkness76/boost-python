# Copyright David Abrahams 2004. 
# Copyright Troy D. Straszheim 2009. Distributed under the Boost
# Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
#  This is the docstring used in the polymorphism and
#  polymorphism-with-auto-ptr tests
#
code = '''
  get reference to cpp created B Object

>>> a = getBCppObj()

  Python Created B Object and Cpp B Object
  Should have same result by calling f()

>>> a.f()
'B::f()'
>>> call_f(a) 
'B::f()'
>>> call_f(A()) 
'A::f()'

  Test references
  B is not exposed to python

>>> a = getBCppObj()
>>> type(a) == A
True

  C is exposed to python

>>> c = getCCppObj()
>>> type(c) == C
True

  Test Factory

>>> type(factory(0)) == A
True
>>> type(factory(1)) == A
True
>>> type(factory(2)) == C
True

  Test return by 

>>> class X(A):
...     def f(self):
...         return 'X.f'
... 
>>> x = X()
>>> x.f()
'X.f'
>>> call_f(x)
'X.f'
>>> del X

   Test self default

>>> class X(A):
...      def f(self):
...          return 'X.f() -> ' + A.f(self)
... 
>>> x = X()
>>> x.f()
'X.f() -> A::f()'

  Test wrapper downcast

>>> a = pass_a(D())
>>> a.g()
'D::g()'

  Test pure virtual

>>> p = P()
>>> p.f()
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
RuntimeError: Pure virtual function called
>>> q = Q()
>>> q.f()
'Q::f()'
>>> class R(P):
...     def f(self):
...        return 'R.f'
... 
>>> r = R()
>>> r.f()
'R.f'
>>> 
'''

