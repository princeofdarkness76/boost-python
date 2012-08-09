// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef ARG_FROM_PYTHON_DWA2002128_HPP
# define ARG_FROM_PYTHON_DWA2002128_HPP

# include <boost/python/detail/prefix.hpp>
# include <boost/python/converter/arg_from_python.hpp>
# if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400)) \
    || BOOST_WORKAROUND(BOOST_INTEL_WIN, BOOST_TESTED_AT(800))
# include <boost/type_traits/remove_cv.hpp>
#endif
#include <boost/utility/result_of.hpp>

namespace boost { namespace python { 

template <class T>
struct arg_from_python
    : converter::select_arg_from_python<
# if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400)) \
    || BOOST_WORKAROUND(BOOST_INTEL_WIN, BOOST_TESTED_AT(800))
          typename boost::remove_cv<T>::type
# else
          T
# endif 
      >::type
{
    typedef typename converter::select_arg_from_python<
# if BOOST_WORKAROUND(BOOST_MSVC, BOOST_TESTED_AT(1400)) \
    || BOOST_WORKAROUND(BOOST_INTEL_WIN, BOOST_TESTED_AT(800))
          typename boost::remove_cv<T>::type
# else
          T
# endif 
        >::type base;
    
    arg_from_python(PyObject*);
};

    template <class U, class A>
    struct arg_from_python<U(*)(A)>
    {
      typedef typename converter::select_arg_from_python<A>::type pyconv_t;
      typedef typename pyconv_t::result_type pyconv_result;
      pyconv_t pyconv;

      typedef typename boost::result_of<U(pyconv_result)>::type result_type;

      arg_from_python(PyObject* p) : pyconv(p) { }
      bool convertible() const { return pyconv.convertible(); }
      result_type operator()() 
      {
	return U()(pyconv());
      }
    };

// specialization for PyObject*
template <>
struct arg_from_python<PyObject*>
{
    typedef PyObject* result_type;
    
    arg_from_python(PyObject* p) : m_source(p) {}
    bool convertible() const { return true; }
    PyObject* operator()() const { return m_source; }
 private:
    PyObject* m_source;
};

template <>
struct arg_from_python<PyObject* const&>
{
    typedef PyObject* const& result_type;
    
    arg_from_python(PyObject* p) : m_source(p) {}
    bool convertible() const { return true; }
    PyObject*const& operator()() const { return m_source; }
 private:
    PyObject* m_source;
};

//
// implementations
//
template <class T>
inline arg_from_python<T>::arg_from_python(PyObject* source)
    : base(source)
{
}

}} // namespace boost::python

#endif // ARG_FROM_PYTHON_DWA2002128_HPP
