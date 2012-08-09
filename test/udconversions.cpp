// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Basic exercise of userdefined secondary conversions
// 
#include <boost/python.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using boost::optional;
using namespace boost::python;

//
//  types hidden from python
//

struct A
{
  double value;
};

struct B
{
  double value;
};

//
//  c++ interface that uses them
//
std::string takes_a(A a)
{
  return std::string("Got an A containing ") 
    + boost::lexical_cast<std::string>(a.value);
}

std::string takes_b(B b)
{
  return std::string("Got a B containing ") 
    + boost::lexical_cast<std::string>(b.value); 
}

//
//  conversion functions
//
struct double_to_A
{
  typedef A result_type;

  result_type operator()(double v) const
  {
    A a;
    a.value = v;
    return a;
  }
};

struct A_to_B
{
  typedef B result_type;

  result_type operator()(A a) const
  {
    B b;
    b.value = a.value;
    return b;
  }
};

BOOST_PYTHON_MODULE(udconversions_ext)
{
  def("takes_a", as<std::string(double_to_A(double))>(&takes_a));

  //  Chained conversions: not working
  //  def("takes_b", 
  //      as<std::string(A_to_B(double_to_A(double)))>(&takes_b));
};

