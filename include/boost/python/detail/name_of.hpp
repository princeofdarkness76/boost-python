// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_PYTHON_NAME_OF_HPP_INCLUDED
#define BOOST_PYTHON_NAME_OF_HPP_INCLUDED

#include <string>

#ifndef BOOST_MSVC

#include <cxxabi.h>
#include <stdlib.h>

namespace boost {
  namespace python {

    // takes a typename an returns a (hopefully) identical string.  Note
    // name_of<string>() returns "std::string"...

    template <typename T>
    struct reference_to
    { };

    template <typename T>
    struct name_of_impl
    {
      const std::type_info& operator()() {
	return typeid(T);
      }
    };

    template <typename T>
    struct name_of_impl<T&>
    {
      const std::type_info& operator()() 
      {
	return typeid(reference_to<T>);
      }
    };

    template <typename T>
    inline static std::string name_of()
    {
      int status;
      char *realname;
      std::string to_return;
      const static char *const notavailable = "N/A";
      const std::type_info &ti = typeid(T); //name_of_impl<T>()();
      const char* mangled = ti.name();
      if (!mangled)
	to_return = notavailable;
      else
	{
	  realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
	  if (realname)
	    {
	      to_return = realname;
	      free(realname);
	    }
	  else
	    to_return = notavailable;
	}
      return to_return;
    }

    template <typename T>
    inline static std::string name_of(T &t)
    {
      int status;
      char *realname;
      std::string to_return;
      const static char *const notavailable = "N/A";
      const std::type_info &ti = typeid(T);
      const char* mangled = ti.name();
      if (!mangled)
	to_return = notavailable;
      else
	{
	  realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
	  if (realname)
	    {
	      to_return = realname;
	      free(realname);
	    }
	  else
	    to_return = notavailable;
	}
      return to_return;
    }

    template <typename T>
    inline static std::string name_of(const T &t)
    {
      int status;
      char *realname;
      std::string to_return;
      const static char *const notavailable = "";
      const std::type_info &ti = typeid(T);
      const char* mangled = ti.name();
      if (!mangled)
	return "[no ti.name]";
      else
	{
	  realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
	  if (realname)
	    {
	      to_return = realname;
	      free(realname);
	    }
	  else
	    to_return = notavailable;
	}
      return to_return;
    }

    inline static std::string name_of(const std::type_info &ti)
    {
      int status;
      char *realname;
      std::string to_return;

      realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
      to_return = realname;
      free(realname);
      return to_return;
    }
  }
}

#else // msvc

namespace boost {
  namespace python {

    template <typename T>
    inline static std::string name_of(const T &t) { return "N/A"; }

    template <typename T>
    inline static std::string name_of(T &t) {  return "N/A"; }

    inline static std::string name_of(const std::type_info &ti)
    {
      return "N/A";
    }
  }
}
#endif

#endif
