// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/spirit/home/phoenix.hpp>
#include <iostream>

using namespace boost::python;
using namespace boost::phoenix::arg_names;
using namespace boost::phoenix::local_names;
using namespace boost::phoenix;

struct X 
{ 
  int x; 
};
typedef boost::shared_ptr<X> XPtr;

//
//  From the phoenix examples
//
struct factorial_impl
{
    template <typename Arg>
    struct result
    {
        typedef Arg type;
    };

    template <typename Arg>
    Arg operator()(Arg n) const
    {
        return (n <= 0) ? 1 : n * this->operator()(n-1);
    }
};

function<factorial_impl> factorial;

BOOST_PYTHON_MODULE(phoenix_ext)
{
  def("three", as<int()>(val(3)));

  def("shazam", as<std::string()>(val("shazam")));

  def("plus", as<double(double, double)>(arg1 + arg2));

  def("argtwo", as<int(int, int, int, int)>(arg2));

  //
  //  This is kinda pointless (you could just write factorial_impl as
  //  a DFO) but it shows the use of a user defined phoenix function.
  //
  def("factorial_of_sum", as<int(int, int)>(factorial(arg1 + arg2)));

  factorial_impl fimpl;
  //  def("factorial_impl", as<int(int)>(fimpl));

  // std::cout << boost::fusion::invoke(fimpl, boost::fusion::make_vector(3));

  def("maybe_throw", 
      as<void(int)>(if_(arg1 > 5) 
		    [ throw_(std::runtime_error("too big!!")) ]
		    .else_        
		    [ std::cout << val("okay")                ]));

  //
  // Danger: args to concatenate have to be by value, or the return
  // type calculation will be a reference... to a temporary.  This is
  // a phoenix issue, not boost::python.  Try:
  //
  // std::string foo("foo"); const std::string& fooref = foo;
  // std::string bar("bar"); const std::string& barref = bar;
  // std::string s = (arg1 + arg2)(fooref, barref);
  // std::cout << ">>>" << s << "<<<\n";
  //
  def("concatenate", 
      as<std::string(std::string, std::string)>(arg1 + "concat" + arg2));

  //
  // here things are just passed around by value, no problems.
  //
  def("add_ints_by_ref", as<int(const int&, const int&)>(arg1 + arg2));

  class_<X, XPtr>("X")
    .def_readwrite("x", &X::x)
    .def("add7", as<void(X*)>  (arg1->*&X::x      += 7))
    .def("add8", as<void(XPtr)>(arg1->*&X::x      += 8))
    .def("add9", as<void(X&)>  (bind(&X::x, arg1) += 9))
  ;

  def("plus_ints_to_double", 
      as<double(int, int)>(std::plus<int>()));

};

