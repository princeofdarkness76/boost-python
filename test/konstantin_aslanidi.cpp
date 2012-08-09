// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp> 

#include <string> 
#include <fstream> 
#include <boost/shared_ptr.hpp> 

using namespace boost::python;

std::string go(object& input) 
{
  object o = input.attr("__str__")(); 
  extract<std::string> ex(o); 
  if(ex.check())
    {
      std::cout << static_cast<std::string>(ex()) << "\n";
      return static_cast<std::string>(ex());
    }
  else
    return "conversion failure";
}

BOOST_PYTHON_MODULE(konstantin_aslanidi_ext) 
{
  def("go",go);
} 
