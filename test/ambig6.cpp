// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>

#include "bpl_test.hpp"
#include "overloads.hpp"

BPL_TEST_MODULE()
{
  // these are not fundamentally ambiguous, but are when called
  // with certain arguments

  r<bool>();
  r<int>();
  r<std::string>();

  r<float, bool>();
  r<float, int>();
  r<float, std::string>();

  r<float,float,int>();
  r<float,float,double>();
  r<float,float,std::string>();
}


