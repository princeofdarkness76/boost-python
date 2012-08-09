// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef OVERLOAD_SCORE_HPP_TDS20091207_HPP
# define OVERLOAD_SCORE_HPP_TDS20091207_HPP

# include <boost/python/detail/prefix.hpp>
# include <boost/python/converter/rvalue_from_python_data.hpp>
# include <boost/python/arg_from_python.hpp>
# include <boost/optional.hpp>

namespace boost { namespace python { namespace converter { 

      template <typename T>
      struct overload_score
      {
	boost::optional<unsigned> operator()(PyObject* type)
	{
	  if(arg_from_python<T>(type).convertible())
	    return boost::optional<unsigned>(0);
	  else
	    return boost::optional<unsigned>();
	}
      };

      template <>
      struct overload_score<bool>
      {
	boost::optional<unsigned> operator()(PyObject* type)
	{
	  if(PyBool_Check(type))
	      return 0u;
	  else if (PyInt_CheckExact(type))
	      return 1u;
	  else if (PyFloat_CheckExact(type))
	    return 1u;
	  else if(arg_from_python<bool>(type).convertible())
	    return 10u;
	  else
	    return boost::none;
	}
      };

      template <>
      struct overload_score<int>
      {
	boost::optional<unsigned> operator()(PyObject* type)
	{
	  if(PyInt_CheckExact(type))
	      return 0u;
	  else if (PyBool_Check(type))
	      return 1u;
	  else if (PyFloat_CheckExact(type))
	    return 1u;
	  else if(arg_from_python<int>(type).convertible())
	    return 10u;
	  else
	    return boost::none;
	}
      };

      template <>
      struct overload_score<float>
      {
	boost::optional<unsigned> operator()(PyObject* type)
	{
	  if(PyFloat_CheckExact(type))
	      return 0u;
	  else if (PyInt_CheckExact(type))
	      return 1u;
	  else if (PyBool_Check(type))
	    return 1u;
	  else if(arg_from_python<float>(type).convertible())
	    return 10u;
	  else
	    return boost::none;
	}
      };

      template <>
      struct overload_score<double>
      {
	boost::optional<unsigned> operator()(PyObject* type)
	{
	  if(PyFloat_CheckExact(type))
	      return 0u;
	  else if (PyInt_CheckExact(type))
	      return 1u;
	  else if (PyBool_Check(type))
	    return 1u;
	  else if(arg_from_python<float>(type).convertible())
	    return 10u;
	  else
	    return boost::none;
	}
      };


}}} // namespace boost::python::converter

#endif // OVERLOAD_SCORE_HPP_TDS20091207_HPP
