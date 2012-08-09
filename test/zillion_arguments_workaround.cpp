// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# include <boost/python.hpp>

using namespace boost::python;

// here's a function that takes a zillion arguments
// (obviously it actually takes only two, but this test case should
// demonstrate the technique).  This is the function that is in the library
// we're wrapping, ie we can't change it.
int f(int i, int j)
{
  return i * j;
}

//
//  Here's the thunk to takes_a_zillion that unpacks a tuple and
//  forwards to the unchangeable function, which Boost.python,
//  type_traits and friends don't have the opportunity to choke on.
//
int f_thunk(boost::python::tuple t)
{
  extract<int> arg1(t[0]), arg2(t[1]);

  return f(arg1(), arg2());
}

BOOST_PYTHON_MODULE(zillion_arguments_workaround_ext)
{
  // from the python side it would seem more natural to call this an
  // _impl, as the thunking is hidden
  def("f_impl", &f_thunk);
}
