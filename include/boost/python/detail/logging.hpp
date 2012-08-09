// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_PYTHON_LOGGING_HPP_INCLUDED
#define BOOST_PYTHON_LOGGING_HPP_INCLUDED

#include <stdexcept>
#include <ostream>
#include <boost/format.hpp>
#include <boost/python/detail/name_of.hpp>

#define LOG_TRACE 0
#define LOG_DEBUG 1
#define LOG_INFO  2
#define LOG_WARN  3
#define LOG_ERROR 4
#define LOG_FATAL 5

#if BOOST_PYTHON_DEBUG
#define BOOST_PYTHON_LOGGING_LEVEL LOG_TRACE
#else
#define BOOST_PYTHON_LOGGING_LEVEL LOG_INFO
#endif

namespace boost {
  namespace python {

    std::ostream& ofs();
    void logobj(const char* what, const PyObject* obj);

  }
}

// implmentation macro of macros visible to user.
#define LOG_IMPL(LEVEL, ufmt, ...)					\
  ::boost::python::ofs() << ::boost::format("%s:%d: " ufmt "\n") % __FILE__ % __LINE__ % __VA_ARGS__, ::boost::python::ofs().flush()
  
#if BOOST_PYTHON_LOGGING_LEVEL <= LOG_TRACE
#define log_trace(format, ...) LOG_IMPL(LOG_TRACE, format, ##__VA_ARGS__)
#else
#define log_trace(format, ...) 
#endif

#if BOOST_PYTHON_LOGGING_LEVEL <= LOG_DEBUG
#define log_debug(format, ...) LOG_IMPL(LOG_DEBUG, format, ##__VA_ARGS__)
#else
#define log_debug(format, ...) 
#endif

#if BOOST_PYTHON_LOGGING_LEVEL <= LOG_INFO
#define log_info(format, ...) LOG_IMPL(LOG_INFO, format, ##__VA_ARGS__)
#else
#define log_info(format, ...) 
#endif

#if BOOST_PYTHON_LOGGING_LEVEL <= LOG_WARN
#define log_warn(format, ...) LOG_IMPL(LOG_WARN, format, ##__VA_ARGS__)
#else
#define log_warn(format, ...) 
#endif

#if BOOST_PYTHON_LOGGING_LEVEL <= LOG_ERROR
#define log_error(format, ...) LOG_IMPL(LOG_ERROR, format, ##__VA_ARGS__)
#else
#define log_error(format, ...) 
#endif

#define log_fatal(format, ...) LOG_IMPL(LOG_FATAL, format, ##__VA_ARGS__), throw std::runtime_error("log_fatal")


#endif 

