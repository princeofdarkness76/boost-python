// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/mpl/vector.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>

using namespace boost::python;

struct X 
{ 
  int y; 
};

struct fnobj
{
  typedef int result_type;
  int operator()(X* x, int i) 
  { 
    return x->y *= i; 
  }
};


BOOST_PYTHON_MODULE( function_objects_ext )
{
  boost::python::class_<X>( "X" )
    .def_readwrite( "y", &X::y )
    .def( "f", as<int(const X*, int)>(fnobj()))
    ;
}

//#include "module_tail.cpp"
