// Copyright Maximilian Matthe 2008.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python/return_pointee_value.hpp>
#include <boost/python/module.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/def.hpp>
#include <boost/python/to_python_converter.hpp>

// Out returned, unkown type
struct IntWrapper
{
  IntWrapper(int v) : val(v) {}
  int val;
};

// the custom converter
struct convert_IntWrapper
{
  static PyObject* convert(IntWrapper const& w)
  {
    return boost::python::incref(boost::python::object(w.val).ptr());
  }
};

IntWrapper* returnIntWrapper()
{
  static IntWrapper w(42);
  return &w;
}

//////////////////////////////////////////////////////////
float* get_value()
{
  static float value = 0.5f;
  return &value;
}

float* get_null_value()
{
  return NULL;
}

BOOST_PYTHON_MODULE(return_pointee_value_ext)
{
  using namespace boost::python;
  def("returnIntWrapper", returnIntWrapper, return_value_policy<return_pointee_value>());
	
  def("get_value", get_value, return_value_policy<return_pointee_value>());
  def("get_null_value", get_null_value, return_value_policy<return_pointee_value>());
	
  to_python_converter<IntWrapper, convert_IntWrapper, false>();
}
