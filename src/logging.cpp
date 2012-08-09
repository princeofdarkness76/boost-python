// Copyright (C) 2009 Troy D. Straszheim
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>
#include <fstream>
#include <iostream>

namespace boost {
  namespace python {

    namespace {
      struct start_message {
	bool start;
	start_message() : start(true) { }
      };
    }

    std::ostream& ofs() 
    {
      static start_message sm;
      //      static std::ofstream ofs_("boostpython.log", std::ios::app);
      static std::ostream& ofs_(std::cerr);

      if (sm.start)
	{
	  sm.start = false;
	  ofs_ << "*****************************************************************\n"
	       << "*****************************************************************\n"
	       << "*****************************************************************\n";
	}

      return ofs_;
    }

    void logobj(const char* what, const PyObject* obj)
    {
      return;
      if (!obj)
	{
	  log_trace("%s = PyObject* to (null)", what);
	  return;
	}
      PyObject* obj_repr = PyObject_Repr((PyObject*)obj);

      if (PyErr_Occurred())
	PyErr_Clear();

      if (!obj_repr)
	{
	  log_trace("%s = [repr() failed]", what); 
	  return;
	}
      const char* strrep = PyString_AS_STRING(obj_repr);
      log_trace("%s = %s", what % strrep);
      Py_DECREF(obj_repr);
    }

  }
}
