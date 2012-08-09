// Copyright Eric Niebler 2005.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef STL_ITERATOR_CORE_TDS20090919_HPP
# define STL_ITERATOR_CORE_TDS20090919_HPP

# include <boost/python/object_fwd.hpp>
# include <boost/python/handle_fwd.hpp>

//
//  Unlike C++ iterators, python iterators have shared semantics.  To
//  provide value semantics the c++ implementation here maintains the
//  iterated-over object and the iterator's current position.  The
//  copy constructor creates a new iterator and advances it
//  appropriately.  So this makes copying this iterator an
//  O(size_of_container) operation, though the typical case (begin and
//  end iterators) won't require any incrementing.
//
namespace boost { namespace python { namespace objects {

struct BOOST_PYTHON_DECL stl_input_iterator_impl
{
    stl_input_iterator_impl();
    stl_input_iterator_impl(boost::python::object const &ob);
    stl_input_iterator_impl(stl_input_iterator_impl const &);
    void increment();
    bool equal(stl_input_iterator_impl const &that) const;
    boost::python::handle<> const &current() const;
    stl_input_iterator_impl& operator=(stl_input_iterator_impl const& rhs);

private:
    boost::python::object 
      iterable_, // the object we iterate over
      iter_;     // the python iterator (these things have shraed)
    boost::python::handle<> ob_;

    std::size_t position;
};

}}} // namespace boost::python::object

#endif // STL_ITERATOR_CORE_TDS20090919_HPP
