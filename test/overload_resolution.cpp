// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//
//  example that shows problems with overloading and automatic conversion.
//  if you call one of the below functions from python with bool/int/double, 
//  you'll see that the overload called is first match, not best match.
//  See overload matching in luabind for an example of how to do this better.
//
//  see this mail:
//  http://mail.python.org/pipermail/cplusplus-sig/2009-March/014362.html
//
//  This test isn't called by the cmake/jamfiles.  For future use.
//
#include "bpl_test.hpp"
#include <string>

using boost::python::def;

std::string takes_bool(bool b) { return "bool"; }
std::string takes_int(int b) { return "int"; }
std::string takes_float(float b) { return "float"; }
std::string dbl(double) { return "double"; }
std::string bol(bool) { return "bool"; }

BPL_TEST_MODULE()
{    

  def("f", dbl);
  def("f", bol);

  def("bid", takes_bool);
  def("bid", takes_int);
  def("bid", takes_float);

  def("dib", takes_float);
  def("dib", takes_int);
  def("dib", takes_bool);

  def("idb", takes_int);
  def("idb", takes_float);
  def("idb", takes_bool);

  def("bdi", takes_bool);
  def("bdi", takes_float);
  def("bdi", takes_int);
}

