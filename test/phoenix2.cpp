// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <iostream>

using namespace boost::python;
using namespace boost::phoenix::arg_names;
using namespace boost::phoenix;

//
//  From the phoenix examples.  This is a phoenix actor, 
//  not a DFO
//
struct len_impl
{
  template <typename Arg>
  struct result
  {
    typedef int type;
  };

  int
  operator()(const object& obj) const
  {
    return len(obj);
  }
};

function<len_impl> len_;

//
// a lazy extract<T>
//
template <typename T>
struct extract_eval
{
  template <typename Env, typename U>
  struct result
  {
    typedef T type;
  };

  template <typename RT, typename Env, typename U>
  static RT
  eval(Env const& env, U& obj)
  {
    return extract<RT>(obj.eval(env));
  }
};

template <typename T, typename U>
inline actor<typename as_composite<extract_eval<T>, U>::type>
extract_(U const& obj)
{
  return compose<extract_eval<T> >(obj);
}

//
// a lazy check<T>
//
template <typename T>
struct check_eval
{
  template <typename Env, typename U>
  struct result
  {
    typedef bool type;
  };

  template <typename RT, typename Env, typename U>
  static RT
  eval(Env const& env, const U& obj)
  {
    return extract<T>(obj.eval(env)).check();
  }
};

template <typename T, typename U>
inline actor<typename as_composite<check_eval<T>, U>::type>
check_(U const& obj)
{
  return compose<check_eval<T> >(obj);
}

//
//  the module
//

BOOST_PYTHON_MODULE(phoenix2_ext)
{
  def("lazy_len", as<int(const object&)>(len_(arg1)));

  def("extract_string", as<std::string(object)>(extract_<std::string>(arg1)));

  def("check_string", as<bool(const object&)>(check_<std::string>(arg1)));

};

