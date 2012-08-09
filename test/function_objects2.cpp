// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <boost/utility/result_of.hpp>

using namespace boost::phoenix::arg_names;
using namespace boost::python;
using namespace boost::phoenix;
namespace fusion = boost::fusion;
using boost::result_of;
using boost::ref;


struct X 
{ 
  int y; 
  template <typename T>
  void accept(T& t)
  {
    std::cout << "accepting...\n";
    t.visit(*this);
  }

  template <typename T>
  T add_to(T what)
  {
    return what + y;
  }

  void blangvalue(int, X x) { std::cout << "blangvalue\n"; }
  void blangref(int, X& x) { std::cout << "blangref\n"; }
  void blangconstref(int, const X& x) { std::cout << "blangconstref\n"; }

};

struct Visitor1
{
  void visit(X& x)
  {
    std::cout << "Visitor1 visiting X\n";
    x.y *= 3;
  }
};

struct Visitor2
{
  void visit(X& x)
  {
    std::cout << "Visitor2 visiting X\n";
    x.y -= 7;
  }

  bool foo(double d)
  {
    std::cout << "Visitor2::foo(" << d << ") called\n";
    return d > 0;
  }
};

struct add_to
{
  template <typename T>
  struct result;

  template <typename Self, typename T>
  struct result<Self(X&, T&)> // these are kinda irritating
  {
    typedef T type;
  };

  template <typename T>
  T operator()(X& x, const T& t) const
  {
    return t + x.y;
  } 

  std::string operator()(X& x, const std::string& s)
  {
    return s + boost::lexical_cast<std::string>(x.y);
  }
};

BOOST_PYTHON_MODULE(function_objects2_ext)
{
  add_to add_to_inst;

  boost::function<int(X&, int)> f_int(add_to_inst);
  boost::function<std::string(X&, std::string)> f_string(add_to_inst);
  
  boost::python::class_<Visitor1>("Visitor1");
  boost::python::class_<Visitor2>("Visitor2");

  boost::python::class_<X>("X")
    .def_readwrite("y", &X::y)
    //
    //  boost::function needs no "as"
    //
    .def( "add_to", f_int)
    .def( "add_to", f_string)
    //
    //  "as", or specify the signature of a function object
    //
    .def( "add_to_via_as", as<double(X&, double)>(add_to_inst))
    //
    //  "unload", or specify a particular overload/instantiation
    //  
    .def( "accept", unload<void(X::*)(Visitor1&)>(&X::accept))
    .def( "accept", unload<void(X::*)(Visitor2&)>(&X::accept))
    ;

  //
  //  std binary_function works when wrapped with "as",
  //  but this could be removed given overloads for 
  //
  def("std_plus_int", std::plus<int>());
  
  //
  //  phoenix expressions
  //
  def("three", as<int()>(val(3)));

  def("plus", as<double(double, double)>(arg1 + arg2));

  //  def("concatenate", as<std::string(const std::string&)>(std::cout << arg1, 
  //			 arg1 + "hi"));

  //  factorial_impl factorial_fnobject;
  //  def("factorial", as<int(int)>(factorial_fnobject));
  //  def("factorial_of_n_plusone", as<double(double)>(factorial(arg1 + 1)));

};

//#include "module_tail.cpp"
