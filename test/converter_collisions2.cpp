// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Basic exercise of userdefined secondary conversions
// 
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <vector>

using namespace boost::python;

BOOST_PYTHON_MODULE(converter_collisions2_ext)
{
  class_<std::vector<double> >("std_vector_double")
    .def(vector_indexing_suite<std::vector<double> >())
    ;
};

