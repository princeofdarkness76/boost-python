// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/function.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>

namespace mpl = boost::mpl;

struct X { int y; };

struct FnObject
{
  typedef int result_type;

  int operator()(X *x, int z) const
  {
    return z + x->y;
  }
};

struct add_to
{
  template <typename T>
  T operator()(X* x, T t) const
  {
    return t + x->y;
  } 
};

int f1(X* x) { return x->y ; }
int f2(X* x, int i) { return x->y * i; }
int f3(X* x, int i, int j) { return x->y * i + j; }

namespace boost {
  namespace python {
    namespace detail {

      template<>
      struct get_signature<FnObject, X>
      {
	typedef mpl::vector<int, X*, int> type; 
      };

    }
  }
}

using namespace boost::python;

BOOST_PYTHON_MODULE( function_objects_ext )
{
  FnObject fnobj;
  boost::function<int(X*, int)> bf0(fnobj);
  
  boost::function<int(X*)> bf1(f1);
  boost::function<int(X*, int)> bf2(f2);
  boost::function<int(X*, int, double)> bf3(f3);

  add_to add_to_inst;
  boost::function<int(X*, int)> add_to_int_bfn(add_to_inst);
  boost::function<double(X*, double)> add_to_double_bfn(add_to_inst);
  
  boost::python::class_<X>( "X" )
    .def( "fobj", fnobj)
    .def( "bf0",  bf0)
    .def( "bf1",  bf1)
    .def( "bf2",  bf2)
    .def( "bf3",  bf3)
    .def( "add_to_int", add_to_int_bfn)
    .def( "add_to_double", add_to_double_bfn)
    .def_readwrite( "y", &X::y )
    ;
}

//#include "module_tail.cpp"
