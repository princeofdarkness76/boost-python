#include <boost/python.hpp>
#include <boost/python/detail/logging.hpp>
#include <iostream>

namespace bp = boost::python;

struct LeftClass { };
struct RightClass { };

std::string test_left(const LeftClass &) 
{ 
  return "Successfully converted to LeftClass";
}
std::string test_right(const RightClass &) 
{ 
  return "Successfully converted to RightClass";
}

BOOST_PYTHON_MODULE(multiple_inheritance_ext)
{
    bp::def("test_left", &test_left);
    bp::def("test_right", &test_right);

    bp::class_<LeftClass>("LeftClass", bp::init<>());
    bp::class_<RightClass>("RightClass", bp::init<>());
}
