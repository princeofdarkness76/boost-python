// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/function.hpp>

using namespace boost::python;

struct X
{
  int y;
};

int timesthree(boost::shared_ptr<X> xp)
{
  return xp->y * 3;
}

boost::function<int(boost::shared_ptr<X>)> timesthree_prime(timesthree);

BOOST_PYTHON_MODULE(function_objects4_ext)
{
  class_<X>("X")
    .def_readwrite("y", &X::y)
    .def("timesthree", timesthree_prime)
    ;
}

//#include "module_tail.cpp"
