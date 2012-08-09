// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Basic exercise of userdefined secondary conversions
// 
#include <boost/python.hpp>
#include <boost/optional.hpp>
#include <iostream>

using boost::optional;
namespace bp = boost::python;

struct S
{
  void set(optional<int> v) 
  { 
    i = v; 
  }

  void add(boost::optional<int> oi)
  {
    if (i && oi) 
      (*i) += (*oi);
  }

  std::string show()
  {
    if (i)
      return boost::lexical_cast<std::string>(*i);
    else
      return "<unset>";
  }

  optional<int> i;
};

template <typename T>
struct to_optional
{
  typedef optional<T> result_type;

  result_type operator()(const bp::object& o)
  {
    bp::extract<T> ex(o);
    if (ex.check())
      return optional<T>(ex());
    else
      return optional<T>();
  }
};

BOOST_PYTHON_MODULE(ravi_ext)
{

  bp::class_<S>("S")
    .def("set", bp::as<void(S*, to_optional<int>(bp::object))>(&S::set))
    .def("add", bp::as<void(S*, to_optional<int>(bp::object))>(&S::add))
    .def("show", &S::show)
    ;
};

