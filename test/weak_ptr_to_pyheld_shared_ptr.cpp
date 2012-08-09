#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>

using namespace boost;

struct T 
{
  T() { std::cout << __PRETTY_FUNCTION__<< "("<< this <<")\n";}
  ~T() { std::cout << __PRETTY_FUNCTION__<< "("<< this <<")\n";}
  
  std::string name;
};

shared_ptr<T> make_T() 
{ 
  shared_ptr<T> sp(new T);
  std::cout << "make_T returning " << sp << "\n";
  return sp;
}


struct U 
{
  shared_ptr<T> obj_shared_ptr;
  weak_ptr<T> obj_weak_ptr;

  void set_weak_ptr(shared_ptr<T> obj_ptr) 
  {
    std::cout << "setting " << obj_ptr << "\n";
    obj_weak_ptr = weak_ptr<T>(obj_ptr);
	
    shared_ptr<T> spt = obj_weak_ptr.lock();
    std::cout << "set_weak_ptr: " << obj_weak_ptr.expired() << "\n";
    std::cout << "locked gives us: " << spt.get() << "\n";
  }

  shared_ptr<T> get_weak_ptr() {
    return obj_weak_ptr.lock();
  }

  std::string name_via_weak_ptr() { 
    if (shared_ptr<T> obj_ptr = obj_weak_ptr.lock())
      return obj_ptr.get()->name;
    else
      return "NULL";
  }

  /*
  void set_shared_ptr(shared_ptr<T> obj_ptr) {
    obj_shared_ptr = obj_ptr;
  }

  shared_ptr<T> get_shared_ptr() {
    return obj_shared_ptr;
  }

  void print_shared_ptr() {
    if (obj_shared_ptr.get()!=0)
      std::cout << obj_shared_ptr.get()->name
		<< std::endl;
    else
      std::cout << "NO OBJECT" << std::endl;
  }
  */
};

// PYTHON MODULE EXPORT ----------------------------------------------

BOOST_PYTHON_MODULE(jahnfuchs_ext)
{
  using namespace python;

  class_<T, shared_ptr<T> >("T")
    .def_readwrite("name", &T::name)
    ;

  def("make_T", &make_T);

  class_<U>("U")
    //.def("set_shared_ptr", &U::set_shared_ptr)
    .def("set_weak_ptr", &U::set_weak_ptr)
    //.def("get_weak_ptr", &U::get_weak_ptr)
    //.def("get_shared_ptr", &U::get_shared_ptr)
    .def("name_via_weak_ptr", &U::name_via_weak_ptr)
    //.def("print_shared_ptr", &U::print_shared_ptr)
    ;
} 
