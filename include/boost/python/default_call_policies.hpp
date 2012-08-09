// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef DEFAULT_CALL_POLICIES_DWA2002131_HPP
# define DEFAULT_CALL_POLICIES_DWA2002131_HPP

# include <boost/python/detail/prefix.hpp>
# include <boost/mpl/if.hpp>
# include <boost/python/to_python_value.hpp>
# include <boost/python/detail/value_arg.hpp>
# include <boost/type_traits/transform_traits.hpp>
# include <boost/type_traits/is_pointer.hpp>
# include <boost/type_traits/is_reference.hpp>
# include <boost/mpl/or.hpp>
# include <boost/mpl/front.hpp>

namespace boost { namespace python { 

template <class T> struct to_python_value;

namespace detail
{
// for "readable" error messages
  template <class T> 
  struct specify_a_return_value_policy_to_wrap_functions_returning
# if defined(__GNUC__) && __GNUC__ >= 3 || defined(__EDG__)
  { }
# endif 
  ;
}

  template <int N>
  inline PyObject* get(mpl::int_<N>, PyObject* const& args_)
  {
    return PyTuple_GET_ITEM(args_,N);
  }

  inline unsigned arity(PyObject* const& args_)
  {
    return PyTuple_GET_SIZE(args_);
  }

  template <class BaseArgs, class Offset>
  struct offset_args
  {
      offset_args(BaseArgs base_) : base(base_) {}
      BaseArgs base;
  };

  template <int N, class BaseArgs, class Offset>
  inline PyObject* get(mpl::int_<N>, offset_args<BaseArgs,Offset> const& args_)
  {
      return get(mpl::int_<(N+Offset::value)>(), args_.base);
  }
  
  template <class BaseArgs, class Offset>
  inline unsigned arity(offset_args<BaseArgs,Offset> const& args_)
  {
      return arity(args_.base) - Offset::value;
  }

struct default_result_converter;

struct default_call_policies
{
    // Ownership of this argument tuple will ultimately be adopted by
    // the caller.
    template <class ArgumentPackage>
    static bool precall(ArgumentPackage const&)
    {
        return true;
    }

    // Pass the result through
    template <class ArgumentPackage>
    static PyObject* postcall(ArgumentPackage const&, PyObject* result)
    {
        return result;
    }

    typedef default_result_converter result_converter;

    typedef offset_args<PyObject*, mpl::int_<0> > argument_package;

    template <class Sig> 
    struct extract_return_type : mpl::front<Sig>
    {
    };

};

struct default_result_converter
{
    template <class R>
    struct apply
    {
        typedef typename mpl::if_<
            mpl::or_<is_pointer<R>, is_reference<R> >
          , detail::specify_a_return_value_policy_to_wrap_functions_returning<R>
          , boost::python::to_python_value<
                typename detail::value_arg<R>::type
            >
        >::type type;
    };
};

// Exceptions for c strings an PyObject*s
template <>
struct default_result_converter::apply<char const*>
{
    typedef boost::python::to_python_value<char const*const&> type;
};

template <>
struct default_result_converter::apply<PyObject*>
{
    typedef boost::python::to_python_value<PyObject*const&> type;
};

}} // namespace boost::python

#endif // DEFAULT_CALL_POLICIES_DWA2002131_HPP
