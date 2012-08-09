// Copyright Haoyu Bai 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/python.hpp>
#include <boost/python/detail/name_of.hpp>
#include <string>

long long return_longlong()
{
    return 1099511627776LL;
}

int return_int()
{
  return 14;
}

std::wstring return_wstring()
{
    return L"abcde";
}

std::string return_string()
{
  return "hi";
}

BOOST_PYTHON_MODULE(haoyu_bai0_ext)
{
    using namespace boost::python;
    def("return_longlong", return_longlong);
    def("return_int", return_int);
    def("return_wstring", return_wstring);
    def("return_string", return_string);
}
