// Copyright David Abrahams 2002.
// Copyright Troy D. Straszheim 2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

# ifndef CALLER_TDS20090921_HPP
#  define CALLER_TDS20090921_HPP

#  include <boost/python/type_id.hpp>
#  include <boost/python/handle.hpp>

#  include <boost/detail/indirect_traits.hpp>

#  include <boost/python/detail/signature.hpp>
#  include <boost/python/signature.hpp>

#  include <boost/python/arg_from_python.hpp>
#  include <boost/python/converter/context_result_converter.hpp>
#  include <boost/python/converter/builtin_converters.hpp>
#  include <boost/python/converter/overload_score.hpp>

#  include <boost/python/detail/name_of.hpp>

#  include <boost/compressed_pair.hpp>
#  include <boost/optional.hpp>

#  include <boost/type_traits/is_same.hpp>
#  include <boost/type_traits/is_convertible.hpp>
#  include <boost/type_traits/remove_cv.hpp>

#  include <boost/mpl/apply.hpp>
#  include <boost/mpl/eval_if.hpp>
#  include <boost/mpl/identity.hpp>
#  include <boost/mpl/size.hpp>
#  include <boost/mpl/at.hpp>
#  include <boost/mpl/int.hpp>
#  include <boost/mpl/next.hpp>
#  include <boost/mpl/print.hpp>

#  include <iostream>
#  include <boost/fusion/container/generation/make_vector.hpp>
#  include <boost/fusion/container/vector/convert.hpp>
#  include <boost/fusion/include/as_vector.hpp>
#  include <boost/fusion/adapted/mpl.hpp>
#  include <boost/fusion/include/mpl.hpp>
#  include <boost/fusion/include/cons.hpp>
#  include <boost/fusion/iterator/deref.hpp>
#  include <boost/fusion/include/deref.hpp>
#  include <boost/fusion/sequence/intrinsic/begin.hpp>
#  include <boost/fusion/include/begin.hpp>

#  include <boost/fusion/functional/invocation/invoke.hpp>
#  include <boost/fusion/algorithm/transformation/transform.hpp>
#  include <boost/fusion/algorithm/transformation/push_back.hpp>
#  include <boost/fusion/include/push_back.hpp>
#  include <boost/fusion/include/transform.hpp>
#  include <boost/fusion/functional/generation/make_fused_function_object.hpp>
#  include <boost/fusion/include/make_fused_function_object.hpp>

namespace boost { namespace python { namespace detail { 

// This "result converter" is really just used as
// a dispatch tag to invoke(...), selecting the appropriate
// implementation
typedef int void_result_to_python;

// Given a model of CallPolicies and a C++ result type, this
// metafunction selects the appropriate converter to use for
// converting the result to python.
template <class Policies, class Result>
struct select_result_converter
  : mpl::eval_if<
        is_same<Result,void>
      , mpl::identity<void_result_to_python>
      , mpl::apply1<typename Policies::result_converter,Result>
    >
{
};

template <class ArgPackage, class ResultConverter>
inline ResultConverter create_result_converter(
    ArgPackage const& args_
  , ResultConverter*
  , converter::context_result_converter*
)
{
    return ResultConverter(args_.base);
}
    
template <class ArgPackage, class ResultConverter>
inline ResultConverter create_result_converter(
    ArgPackage const&
  , ResultConverter*
  , ...
)
{
    return ResultConverter();
}

#ifndef BOOST_PYTHON_NO_PY_SIGNATURES
template <class ResultConverter>
struct converter_target_type 
{
    static PyTypeObject const *get_pytype()
    {
        return create_result_converter((PyObject*)0, 
				       (ResultConverter *)0, 
				       (ResultConverter *)0)
	  .get_pytype();
    }
};

template < >
struct converter_target_type <void_result_to_python >
{
    static PyTypeObject const *get_pytype()
    {
        return 0;
    }
};
#endif

// A function object type which wraps C++ objects as Python callable
// objects.
//
// Template Arguments:
//
//   F -
//      the C++ `function object' that will be called. Might
//      actually be any data for which an appropriate invoke_tag() can
//      be generated. invoke(...) takes care of the actual invocation syntax.
//
//   CallPolicies -
//      The precall, postcall, and what kind of resultconverter to
//      generate for mpl::front<Sig>::type
//
//   Sig -
//      The `intended signature' of the function. An MPL sequence
//      beginning with a result type and continuing with a list of
//      argument types.
//
template <class F, class Policies, class Sig,
	  unsigned N = mpl::size<Sig>::value - 1>
struct caller
{
  typedef PyObject* result_type;
  
  typedef typename mpl::begin<Sig>::type first;
  typedef typename first::type result_t;
  typedef typename select_result_converter<Policies, result_t>::type result_converter;
  typedef typename Policies::argument_package argument_package;
  typedef typename boost::compressed_pair<F, Policies> comp_pair_t;
  typedef typename mpl::true_ scorable;
            
  
  caller(F f, Policies p) : m_data(f,p) { }
  
  PyObject* operator()(PyObject* args, PyObject* /*keywords unused*/)
  {
    argument_package inner_args(args);
    return invoker<>::apply(m_data, inner_args, fusion::nil()); 
  }
  
  static inline boost::optional<unsigned> score(PyObject* args)
  {
    argument_package inner_args(args);
    return invoker<>::score(inner_args);
  }

  static inline unsigned min_arity() { return N; }
  
  static py_func_sig_info  signature()
  {
    const signature_element * sig = detail::signature<Sig>::elements();
#ifndef BOOST_PYTHON_NO_PY_SIGNATURES
    
    typedef BOOST_DEDUCED_TYPENAME Policies::template extract_return_type<Sig>::type rtype;
    typedef typename select_result_converter<Policies, rtype>::type result_converter;
    
    static const signature_element ret = {
      (boost::is_void<rtype>::value ? "void" : type_id<rtype>().name())
      , &detail::converter_target_type<result_converter>::get_pytype
      , boost::detail::indirect_traits::is_reference_to_non_const<rtype>::value 
    };
    py_func_sig_info res = {sig, &ret };
#else
    py_func_sig_info res = {sig, sig };
#endif
    
    return  res;
  }

private:

  // 
  //  Internal invoker struct:  this guy uses fusion to 
  //  generate a tuple of converted c++ args and invoke
  //  the actual c++ callable
  //
  template <int ArgN = 1, typename unused = void>
  struct invoker
  {
    typedef typename mpl::at_c<Sig, ArgN>::type arg_type;
    typedef typename boost::remove_cv<arg_type>::type conversion_type;
    typedef typename Policies::argument_package argument_package;

    typedef arg_from_python<arg_type> arg_from_python_t;

    template <typename Converted>
    static inline
    PyObject* apply(comp_pair_t& m_data, 
		    argument_package& argpack, 
		    Converted const& converted)
    {
      arg_from_python_t arg_converter(get(mpl::int_<ArgN-1>(), argpack));
      
      // if we're overloaded, this will have already been checked
      // but if we aren't we need this check.
      if (!arg_converter.convertible())
	return 0; 
      
      typedef typename arg_from_python<arg_type>::result_type result_t;
      result_t this_arg = arg_converter();
      
      return invoker<ArgN+1>
	::apply(m_data, argpack, fusion::push_back(converted, 
						   boost::ref(this_arg)));
    }

    static inline boost::optional<unsigned> score(argument_package& args)
    {
      boost::optional<unsigned> this_score
	= converter::overload_score<conversion_type>()(get(mpl::int_<ArgN-1>(), args));

      if (this_score)
	{
	  boost::optional<unsigned> next = invoker<ArgN+1>::score(args);
	  if (next)
	    *this_score += *next;
	  else
	    return boost::none;
	}

      return this_score;
    }

  };
  
  template <typename unused>
  struct invoker<N+1, unused>
  {
    
    template <class RC, class Argpack> 
    static inline PyObject* 
    go(mpl::false_, RC const& rc, F& f, const Argpack& args)
    {
      return rc(fusion::invoke(get_callable(f), args));
    }
    
    template <class RC, class Argpack>
    static inline PyObject* 
    go(mpl::true_, RC const&, F& f, const Argpack& args)
    {
      fusion::invoke(get_callable(f), args);
      return none();
    }
    
    template <typename Converted>
    static inline PyObject* 
    apply(comp_pair_t& m_data, argument_package& argpack, Converted const& converted)
    {
      log_trace("%s", "invoke! start");
      if (!m_data.second().precall(argpack.base))
	  return 0;
    
      PyObject* result = go(typename is_same<result_t, void>::type(),
			    create_result_converter(argpack.base, 
						    (result_converter*)0, (result_converter*)0),
			    m_data.first(),
			    converted);

      log_trace("%s", "invoke! done");
      return m_data.second().postcall(argpack.base, result);
    }

    static inline boost::optional<unsigned> 
    score(argument_package& args)
    {
      return 0u;
    }

  };
  
  comp_pair_t m_data;
};

template <typename T>
struct score
{
  boost::optional<unsigned> operator()(PyObject*) const
  {
    return 0u;
  }
};

template <class F, class Policies, class Sig, unsigned N>
struct score<caller<F, Policies, Sig, N> >
{
  boost::optional<unsigned> operator()(PyObject* args) const
  {
    return caller<F, Policies, Sig, N>::score(args);
  }
};


      
}}} // namespace boost::python::detail



#endif 


