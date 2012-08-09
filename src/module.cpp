// (C) Copyright David Abrahams 2000.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The author gratefully acknowleges the support of Dragon Systems, Inc., in
// producing this work.
//
// Troy D. Straszheim and Haoyu Bai, 2009: py3 support
//
#include <boost/python/scope.hpp>
#include <boost/python/object/add_to_namespace.hpp>

namespace boost { namespace python { namespace detail {

BOOST_PYTHON_DECL void scope_setattr_doc(char const* name, object const& x, char const* doc)
{
    // Use function::add_to_namespace to achieve overloading if
    // appropriate.
    scope current;
    objects::add_to_namespace(current, name, x, doc);
}

namespace
{
  PyMethodDef initial_methods[] = { { 0, 0, 0, 0 } };
}

BOOST_PYTHON_DECL PyObject* 
init_module(char const* name, void(*init_function)())
{
#if PY_VERSION_HEX >= 0x03000000
  //
  // TDS FIXME: Py_Finalize and friends.
  //
  // See the python 3 docs about module initialization
  // for supporting m_clear m_free m_reload, etc.
  //
  // statically initialized generic module def struct,
  // to be copied and tweaked shortly...
  //
  static struct PyModuleDef generic_moduledef = {
    PyModuleDef_HEAD_INIT,
    name,
    0,                //  m_doc
    -1,               //  m_size
    initial_methods,
    0,                //  m_reload
    0,                //  m_traverse
    0,                //  m_clear
    0                 //  m_free
  };

  //
  // TDS: copy the moduledef and set our name.  Unlike py2, we can't use a
  // static, there has to be one of these per module.  (But whether
  // that is per-module-per-interpreter or not isn't clear ATM).
  //
  struct PyModuleDef* this_moduledef = new PyModuleDef(generic_moduledef);
  this_moduledef->m_name = name;

    PyObject* m = PyModule_Create(this_moduledef);
#else
    PyObject* m
        = Py_InitModule(const_cast<char*>(name), initial_methods);
#endif

    if (m != 0)
    {
        // Create the current module scope
        object m_obj(((borrowed_reference_t*)m));
        scope current_module(m_obj);
        
        handle_exception(init_function);
    }
    return m;
}

}}} // namespace boost::python::detail

namespace boost { namespace python {

namespace detail
{
  BOOST_PYTHON_DECL PyObject* current_scope = 0;
}

}}
