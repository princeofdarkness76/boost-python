// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python/numeric.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/str.hpp>

using namespace boost::python;
namespace py = boost::python;

// A separate function to invoke the info() member. Must happen
// outside any doctests since this prints directly to stdout and the
// result text includes the address of the 'self' array.
void info(numeric::array const& z)
{
    z.info();
}

namespace
{
  object handle_error()
  {
      PyObject* type, *value, *traceback;                                 
      PyErr_Fetch(&type, &value, &traceback);                             
      handle<> ty(type), v(value), tr(traceback);
      return object("exception");
      str format("exception type: %sn");                                 
      format += "exception value: %sn";                                  
      format += "traceback:n%s" ;                                        
      object ret = format % py::make_tuple(ty, v, tr);
      return ret;
  }
}

#define CHECK(expr)                                                         \
{                                                                           \
    object result;                                                          \
    try { result = object(expr); }                                          \
    catch(error_already_set)                                                \
    {                                                                       \
        result = handle_error();                                            \
    }                                                                       \
    check(result);                                                          \
}

void blah(numeric::array& ids)
{
  ids.astype('i')[make_tuple(4)];
}


BOOST_PYTHON_MODULE(numpy2_ext)
{
  def("blah", &blah);
}

#include "module_tail.cpp"
