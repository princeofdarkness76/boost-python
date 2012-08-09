// Copyright Eric Niebler 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Credits:
//   Andreas Kl\:ockner for fixing increment() to handle
//   error conditions.
//
//   Value semantics 2009 Troy D. Straszheim
//
#include <boost/python/object.hpp>
#include <boost/python/handle.hpp>
#include <boost/python/object/stl_iterator_core.hpp>

namespace boost { namespace python { namespace objects
{ 

stl_input_iterator_impl::stl_input_iterator_impl()
  : iterable_()
  , iter_()
  , ob_()
  , position(0)
{
}

stl_input_iterator_impl::stl_input_iterator_impl(boost::python::object const &ob)
  : iterable_(ob)
  , iter_(iterable_.attr("__iter__")())
  , ob_()
  , position(0)
{ 
    this->increment();
}

//
//  Our copy constructor has to make a new iterator from the original object.
//
stl_input_iterator_impl::stl_input_iterator_impl(const stl_input_iterator_impl &rhs)
  : iterable_(rhs.iterable_)
  , iter_(iterable_ ? 
	  iterable_.attr("__iter__")() 
	  : boost::python::object())
  , ob_()
  , position(0)
{
  if (iter_ != boost::python::object())
    this->increment();

  while(position < rhs.position)
    this->increment();
}

//
//  Assignment operator has to make a new iterator from the original object.
//
stl_input_iterator_impl&
stl_input_iterator_impl::operator=(const stl_input_iterator_impl &rhs)
{
  if (&rhs == this)
    return *this;

  iterable_ = rhs.iterable_;
  iter_ = iterable_ ? iterable_.attr("__iter__")() : boost::python::object();

  if (iter_ != boost::python::object())
    this->increment();

  while(position < rhs.position)
    this->increment();

  return *this;
}

void stl_input_iterator_impl::increment()
{
    this->ob_ = boost::python::handle<>(
        boost::python::allow_null(PyIter_Next(this->iter_.ptr())));
    if (PyErr_Occurred())
        throw boost::python::error_already_set();
    ++position;
}

bool stl_input_iterator_impl::equal(stl_input_iterator_impl const &that) const
{
    return !this->ob_ == !that.ob_;
}

boost::python::handle<> const &stl_input_iterator_impl::current() const
{
    return this->ob_;
}

}}} // namespace boost::python::objects
