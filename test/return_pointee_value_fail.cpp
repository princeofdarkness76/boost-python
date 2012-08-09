// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/function.hpp>
#include <boost/python.hpp>

using namespace boost::python;

int foo() { return 3; }

BOOST_PYTHON_MODULE( function_objects_ext )
{
  def( "foo", foo, return_value_policy<return_pointee_value>())
}

//#include "module_tail.cpp"
