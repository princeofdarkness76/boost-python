// Copyright Troy D. Straszheim 2010
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include "bpl_test.hpp"

using namespace boost::python;

struct X
{
  int x;
};

object a(13);

BPL_TEST_MODULE()
{
  log_trace("%s", "setattr import start");
  class_<X>("X")
    .add_property("x", &X::x, &X::x)
    //    .def_readwrite("x", &X::x, &X::x)
    .setattr("a", a);
    ;
  log_trace("%s", "setattr import end");
}
