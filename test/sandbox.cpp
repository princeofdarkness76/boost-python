// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>
#include <iostream>
#include <vector>

using namespace boost::python;

int f(object i, object j, object k) 
{ 
  return extract<int>(j) * extract<int>(k);
}

BOOST_PYTHON_MODULE(sandbox_ext)
{
  def("f", f, (arg("i"), arg("j"), arg("k")));
}
