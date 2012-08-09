// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>

using namespace boost::python;

struct S 
{ 
  virtual void foo() { } 
};
struct SWrap : S
{ 
  virtual void foo() { } 
  virtual void default_foo() { }
};

void fail()
{
  class_<SWrap>("S").def("foo", &S::foo, &SWrap::default_foo);
}
