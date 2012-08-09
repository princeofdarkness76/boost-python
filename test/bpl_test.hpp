//
// Copyright Troy D. Straszheim 2009. 
// Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BPL_TEST_HPP_INCLUDED
#define BPL_TEST_HPP_INCLUDED

#include <boost/python.hpp>
#include <boost/preprocessor/expand.hpp>

#define BPL_MODULE_NAME() (BPL_EXTENSION_MODULE)

//
//  BPL_TEST_MODULE() is a little wrapper,
//  expands to BOOST_PYTHON_MODULE(BPL_EXTENSION_MODULE)
//  where BPL_EXTENSION_MODULE is supplied on the compile
//  line.
//
#define BPL_TEST_MODULE() \
  BOOST_PP_EXPAND(BOOST_PYTHON_MODULE BPL_MODULE_NAME())

#endif
