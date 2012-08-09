///////////////////////////////////////////////////////////////////////////////
//
// Copyright David Abrahams 2002, Joel de Guzman, 2002.
// Copyright Troy D. Straszheim 2009
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_PYTHON_SIGNATURE_HPP_INCLUDED
#define BOOST_PYTHON_SIGNATURE_HPP_INCLUDED

#include <boost/python/detail/prefix.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/print.hpp>

#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/function_types/result_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <functional>

namespace boost { 
  namespace python { 

    template <typename Sig, typename T>
    struct callable_holder
    {
      T t;
      callable_holder(T t_) : t(t_) { }
      callable_holder(const callable_holder& rhs) : t(rhs.t) { }
    };

    template <typename Sig, typename T>
    callable_holder<Sig, T> as(const T& t)
    {
      return callable_holder<Sig, T>(t);
    }

    template <typename Sig>
    callable_holder<Sig, Sig> unload(Sig t)
    {
      return callable_holder<Sig, Sig>(t);
    }

    //
    //  These guys return a const ref so that fusion::invoke doesn't
    //  copy the function object unnecessarily
    //
    template <typename T>
    const T& 
    get_callable(T& t)
    {
      return t;
    }

    template <typename Sig, typename T>
    const T& 
    get_callable(callable_holder<Sig, T>& t)
    {
      return t.t;
    }

    template <typename T>
    struct is_callable_holder : mpl::false_ { };
    
    template <typename Sig, typename T>
    struct is_callable_holder<callable_holder<Sig, T> > : mpl::true_ { };
    
    namespace detail {

      //
      //  In the second use of get_signature, will swap out the 'this' member 
      //  for Target, if Target is convertible to the type of the this
      //
      template <typename F, typename Target>
      struct get_signature_impl
      {
	// A metafunction returning C1 if C1 is derived from C2, and C2
	// otherwise
	template <class C1, class C2>
	struct most_derived
	{
	  typedef typename boost::remove_reference<C1>::type C1_refless;
	  typedef typename boost::remove_reference<C2>::type C2_refless;
	  //typedef typename boost::enable_if<is_convertible<C2_refless, 
	  // C1_refless> >::type enabled;
	  typedef typename mpl::if_<
	    is_convertible<C1_refless*,C2_refless*>
	    , C1
	    , C2
          >::type type;
	};
      
	typedef typename boost::function_types::result_type<F>::type result_type;
	typedef typename mpl::at_c<typename boost::function_types::components<F>::type, 1>::type this_type;
	typedef typename mpl::pop_front<typename boost::function_types::parameter_types<F>::type>::type parameter_types;
	typedef typename most_derived<Target, this_type>::type most_derived_this_t;
	typedef typename mpl::push_front<parameter_types, most_derived_this_t>::type class_and_params;
	typedef typename mpl::push_front<class_and_params, result_type>::type type;
      };

      template <typename F>
      struct get_signature_impl<F, void>
      {
	typedef typename boost::function_types::components<F>::type type;
      };
	
      //
      //  There are two forms for invoking metafunction get_signature::
      //
      //      get_signature<F>
      //
      //  and ::
      //
      //      get_signature<F, Target>
      //
      //  These functions extract the return type, class (for member
      //  functions) and arguments of the input signature and stuff them in
      //  an mpl type sequence.  Note that cv-qualification is dropped from
      //  the "hidden this" argument of member functions; that is a
      //  necessary sacrifice to ensure that an lvalue from_python converter
      //  is used.  A pointer is not used so that None will be rejected for
      //  overload resolution.
      //
      //  The second form of get_signature essentially downcasts the "hidden
      //  this" argument of member functions to Target, because the function
      //  may actually be a member of a base class which is not wrapped, and
      //  in that case conversion from python would fail.
      //
      template <typename F, typename Target = void>
      struct get_signature
      {
	typedef typename get_signature_impl<F, Target>::type type;
      };

      template <typename F, typename Target>
      struct get_signature<boost::function<F>, Target>
      {
	typedef typename get_signature_impl<F, Target>::type type;
      };

      template <typename T, typename Target>
      struct get_signature</*std::binary_function<T, T, T>*/std::plus<T>, Target>
      {
	typedef typename mpl::vector<T, const T&, const T&> type;
      };

      template <typename F, typename T, typename Target>
      struct get_signature<callable_holder<F, T>, Target>
      {
	typedef typename get_signature_impl<F, Target>::type type;
      };

    }
  }
}

#endif

