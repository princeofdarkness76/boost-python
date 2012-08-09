// Copyright David Abrahams 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//
// TDS 20091218:  updates for new overloading stuff, you have to 
//                shuffle things around manually.
//

#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include <boost/python/class.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/module.hpp>

using namespace boost::python;

class Foo
{
 public:
  Foo(tuple args, dict kw)
    : args(args), kw(kw) {}
    
    tuple args;
    dict kw;
};

object init_foo(tuple args, dict kw)
{
    tuple rest(args.slice(1,_));
    return args[0].attr("__real_init__")(rest, kw);
}

BOOST_PYTHON_MODULE(raw_ctor_ext)
{
  //
  // previously we could depend on overloads being chosen
  // based on registration order.  Now, we have to register
  // these things and then shuffle them around.  
  // __init__ should be our customized factory function,
  // and __real_init__ should be the actual constructor.
  //
  class_<Foo> c("Foo", init<tuple, dict>());
  c
    .def("__raw_init__", raw_function(&init_foo))
    .def_readwrite("args", &Foo::args)
    .def_readwrite("kw", &Foo::kw)
    ;

  //
  //  swap 'em
  //
  object real_constructor = getattr(c, "__init__");
  object raw_constructor = getattr(c, "__raw_init__");

  setattr(c, "__init__", raw_constructor);
  delattr(c, "__raw_init__");
  setattr(c, "__real_init__", real_constructor);

}

#include "module_tail.cpp"
