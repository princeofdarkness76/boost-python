// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if 1
#warning this doesnt fail for the right reason
#else


#include <boost/python.hpp>
#include <boost/function.hpp>

using namespace boost::python;

struct S 
{ 
  virtual void foo() { } 
};

struct T 
{ 
  virtual void foo() { } 
  virtual void default_foo() { }
};

boost::function<void(S*)> sfoo;
boost::function<void(T*)> tfoo;

void fail()
{
  sfoo = &S::foo;
  tfoo = &T::default_foo;

  class_<T>("T")
    .def("foo", sfoo, tfoo);
}
#endif
