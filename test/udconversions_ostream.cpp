// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Basic exercise of userdefined secondary conversions
// 
#include <boost/python.hpp>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using boost::optional;
using namespace boost::python;

void sayhello(std::ostream& os)
{
  os << "HI!";
}

//
//  conversion functions
//
struct ostream_converter
{
  typedef boost::reference_wrapper<std::ostream> result_type;

  result_type operator()(PyObject* obj) const
  {
    
    
  }
};



BOOST_PYTHON_MODULE(udconversions_ostream_ext)
{
  def("sayhello", as<void(ostream_converter(PyObject*))>(&sayhello));
};

