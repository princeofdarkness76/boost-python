// Copyright 2009 Troy D. Straszheim
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Big thanks to Roman Yakovenko for this test case
//

#include <boost/python.hpp>
#include <boost/python/extract.hpp>

struct base
{
  virtual std::string f() { return "base::f()"; }
  virtual std::string g() { return "base::g()"; }
};
	
struct derived : virtual base
{ 
  virtual std::string f() { return "derived::f()"; }
  virtual std::string g() { return "derived::g()"; }
};    

namespace bp = boost::python;

struct base_wrapper : base, bp::wrapper<base> 
{
  base_wrapper(base const& arg)
    : base(arg), bp::wrapper<base>()
  { }

  base_wrapper()
    : base(), bp::wrapper<base>()
  { }

  //
  //  Here's one way to do it
  //
  virtual std::string f() 
  {
    if(bp::override func_f = this->get_override("f"))
      return func_f();
    else
      return this->base::f();
  }

  //
  //  Here's another
  //
  virtual std::string g()
  {
    if(bp::override func_g = this->get_override("g"))
      return func_g();
    else
      return base::g();
  }
  std::string default_g()
  {
    return base::g();
  }
};

struct derived_wrapper 
  : derived, bp::wrapper<derived> 
{
  derived_wrapper(derived const& arg) 
    : derived(arg), bp::wrapper<derived>()
  { }

  derived_wrapper()
    : derived(), bp::wrapper<derived>()
  { }

  virtual std::string f() 
  {
    if( bp::override func_f = this->get_override("f"))
      return func_f();
    else
      return this->derived::f();
  }

  virtual std::string g()
  {
    if(bp::override func_g = this->get_override("g"))
      return func_g();

    return derived::g();
  }
  std::string default_g()
  {
    return std::string("derived_wrapper::default_g() ") + derived::g();
  }

};

std::string call_base_f(base& b) { return b.f(); }
std::string call_base_g(base& b) { return b.g(); }

std::string call_derived_f(derived& d) { return d.f(); }
std::string call_derived_g(derived& d) { return d.g(); }

BOOST_PYTHON_MODULE(virtual_inheritance_ext)
{
  bp::class_<base_wrapper>("base")    
    .def("f", &base_wrapper::f)
    .def("g", &base::g, &base_wrapper::g)
    ;

  bp::class_<derived_wrapper, bp::bases<base> >("derived")    
    .def("f", &derived_wrapper::f)
    .def("g", &base::g, &derived_wrapper::default_g)
    ;

  bp::def("call_base_f", call_base_f);
  bp::def("call_base_g", call_base_g);

  bp::def("call_derived_f", call_derived_f);
  bp::def("call_derived_g", call_derived_g);
}
