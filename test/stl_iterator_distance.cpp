// Copyright Troy D. Straszheim 2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <vector>

#include <boost/detail/lightweight_test.hpp>

namespace bp = boost::python;

//
//  Ensure std::distance doesn't mutate begin,
//  IOW ensure stl_input_iterator's copy constructor
//  provides value semantics.
//

int main()
{
  try {

    Py_Initialize();

    bp::tuple t = bp::make_tuple(1,2,3,4,5,6,7);

    bp::stl_input_iterator<int> begin(t), end;

    // first time
    std::size_t dist = std::distance(begin, end);
    std::cout << "distance == " << dist << "\n";

    BOOST_TEST(std::distance(begin, end) == 7);
    // make sure distance doesn't mutate iterators
    BOOST_TEST(std::distance(begin, end) == 7);

    BOOST_TEST(*begin == 1);
    begin++;
    
    BOOST_TEST(std::distance(begin, end) == 6);

    // make sure distance doesn't mutate iterators
    BOOST_TEST(std::distance(begin, end) == 6);
    BOOST_TEST(*begin == 2);

    begin++;
    
    BOOST_TEST(std::distance(begin, end) == 5);

    // make sure distance doesn't mutate iterators
    BOOST_TEST(std::distance(begin, end) == 5);
    BOOST_TEST(*begin == 3);

    std::cout << "via BOOST_FOREACH";
    int j = 1;
    BOOST_FOREACH(int i, std::make_pair(bp::stl_input_iterator<int>(t),
					bp::stl_input_iterator<int>()))
      {
	std::cout << "--> " << i << "\n";
	BOOST_TEST(i == j);
	j++;
      }

    //
    //  Check assignment operator
    //
    bp::stl_input_iterator<int> iter(t), iter2;
    iter2 = iter;

    std::cout << "~~> "<< *iter << " " << *iter2 << "\n";;
    BOOST_TEST(*iter == 1);
    BOOST_TEST(*iter2 == 1);

    iter++;
    std::cout << "~~> "<< *iter << " " << *iter2 << "\n";;
    BOOST_TEST(*iter == 2);
    BOOST_TEST(*iter2 == 1);

    iter2++;
    std::cout << "~~> "<< *iter << " " << *iter2 << "\n";;
    BOOST_TEST(*iter == 2);
    BOOST_TEST(*iter2 == 2);
  } 
  catch (boost::python::error_already_set)
    {
      PyErr_Print();
      throw;
    }

  return boost::report_errors();

}
