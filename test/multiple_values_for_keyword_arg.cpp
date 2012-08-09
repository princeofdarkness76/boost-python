// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>
#include "bpl_test.hpp"

int f(int x, int y, int z) { return x*100 + y*10 + z; }

using namespace boost::python;

BPL_TEST_MODULE()
{
  def("f", &f, (arg("x"), arg("y"), arg("z")));
}

