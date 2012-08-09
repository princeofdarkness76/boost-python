// Copyright David Abrahams 2003.
// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_PYTHON_PREFIX_TDS20090917_HPP_INCLUDED
# define BOOST_PYTHON_PREFIX_TDS20090917_HPP_INCLUDED

// The rule is that <Python.h> must be included before any system
// headers (so it can get control over some awful macros).
// Unfortunately, Boost.Python needs to #include <limits.h> first, at
// least... but this gets us as close as possible.

# include <boost/python/detail/wrap_python.hpp>
# include <boost/python/detail/logging.hpp>
# include <boost/python/detail/limits.hpp>
# include <boost/python/detail/config.hpp>

#endif // BOOST_PYTHON_PREFIX_TDS20090917_HPP_INCLUDED
