// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/bind.hpp>

using namespace boost::python;

double multiply(double x, double y) { return x * y; }

BOOST_PYTHON_MODULE( bind_ext )
{
  def("times3", as<double(double)>(boost::bind(multiply, _1, 3)));
}


