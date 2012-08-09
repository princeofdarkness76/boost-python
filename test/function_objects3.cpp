// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/function.hpp>
#include <boost/spirit/home/phoenix.hpp>

using namespace boost::python;

struct times_seven
{
  template <typename T> struct result;

  template <typename Self, typename T>
  struct result<Self(T&)>  // annoyance: reference
  {
    typedef T type;
  };

  template <typename T>
  T operator()(const T& t)
  {
    return t * 7;
  }
};

struct stringmunge
{
  template <typename T> struct result;

  template <typename Self, typename T>
  struct result<Self(T&)>  // annoyance: reference
  {
    typedef T type;
  };

  std::string operator()(const std::string& t)
  {
    return t + "munge munge";
  }
};

boost::function<int(int, int)> myplus = std::plus<int>();

namespace f = boost::fusion;

BOOST_PYTHON_MODULE( function_objects3_ext )
{
  def("times_seven", as<int(int)>(times_seven()));
  def("times_seven_to_double", as<double(int)>(times_seven()));

  //  def("stringmunge", as<std::string(std::string)>(stringmunge()));

  def("myplus", myplus);
  std::string s = "hi";
  std::cout << f::invoke(stringmunge(), f::make_vector(std::string("hi")));

}

//#include "module_tail.cpp"
