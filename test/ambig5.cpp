// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>

#include "bpl_test.hpp"
#include "overloads.hpp"

BPL_TEST_MODULE()
{
  // these not ambiguous

  r<bool>();
  r<int>();
  r<float>();
  r<std::string>();
  r<int, float>();

  r<float, bool>();
  r<float, int>();
  r<float, float>();
  r<float, std::string>();

  r<float,float,bool>();
  r<float,float,int>();
  r<float,float,double>();
  r<float,float,std::string>();
}


