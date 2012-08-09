// Copyright David Abrahams 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python/docstring_options.hpp>
#include <boost/python/object/function_object.hpp>
#include <boost/python/object/function_handle.hpp>
#include <boost/python/object/function_doc_signature.hpp>
#include <boost/python/errors.hpp>
#include <boost/python/str.hpp>
#include <boost/python/object_attributes.hpp>
#include <boost/python/args.hpp>
#include <boost/python/refcount.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/list.hpp>
#include <boost/python/ssize_t.hpp>

#include <boost/python/detail/signature.hpp>
#include <boost/python/detail/none.hpp>
#include <boost/mpl/vector/vector10.hpp>

#include <boost/bind.hpp>
#include <boost/none.hpp>

#include <algorithm>
#include <cstring>

#if BOOST_PYTHON_DEBUG_ERROR_MESSAGES
# include <cstdio>
#endif

void drop_to_debugger() { 
  std::cout << "DEBUGNOW\n";
}

namespace boost { namespace python {
    volatile bool docstring_options::show_user_defined_ = true;
    volatile bool docstring_options::show_cpp_signatures_ = true;
#ifndef BOOST_PYTHON_NO_PY_SIGNATURES
    volatile bool docstring_options::show_py_signatures_ = true;
#else
    volatile bool docstring_options::show_py_signatures_ = false;
#endif
  }}

namespace boost { namespace python { namespace objects { 

      namespace {
	handle<function> not_implemented_function();
      }


      void log(const char* what, boost::python::object obj)
      {
	logobj(what, obj.ptr());
      }

      py_function_impl_base::~py_function_impl_base()
      {
      }

      unsigned py_function_impl_base::max_arity() const
      {
	return this->min_arity();
      }

      extern PyTypeObject function_type;

      function::function(py_function const& implementation,
			 python::detail::keyword const* const names_and_defaults,
			 unsigned num_keywords)
	: m_fn(implementation)
	, m_nkeyword_values(0)
      {
	if (names_and_defaults != 0)
	  {
	    unsigned int max_arity = m_fn.max_arity();
	    unsigned int keyword_offset
	      = max_arity > num_keywords ? max_arity - num_keywords : 0;


	    ssize_t tuple_size = num_keywords ? max_arity : 0;

	    m_arg_names = object(handle<>(PyTuple_New(tuple_size)));

	    if (num_keywords != 0)
	      {
		for (unsigned j = 0; j < keyword_offset; ++j)
		  PyTuple_SET_ITEM(m_arg_names.ptr(), j, incref(Py_None));
	      }
        
	    tuple kv;

	    for (unsigned i = 0; i < num_keywords; ++i)
	      {
		python::detail::keyword const* const p = names_and_defaults + i;
		if (p->default_value)
		  {
		    kv = make_tuple(p->name, p->default_value);
		    ++m_nkeyword_values;
		  }
		else
		  {
		    kv = make_tuple(p->name);
		  }

		keyword_arg_positions[std::string(p->name)] = i+keyword_offset;

		//
		// These 'kv' are tuples: 
		//   ('x',)       [w/o default]   
		//   ('y', 17.0)  [w/ default]
		//
		// log("kv", kv.ptr());
		PyTuple_SET_ITEM(m_arg_names.ptr(), 
				 i + keyword_offset, 
				 incref(kv.ptr()));
	      }
	  }
	// log("m_arg_names", m_arg_names);
    
	PyObject* p = this;
	if (Py_TYPE(&function_type) == 0)
	  {
	    Py_TYPE(&function_type) = &PyType_Type;
	    ::PyType_Ready(&function_type);
	  }
    
	(void)     // warning suppression for GCC
	  (PyObject_INIT(p, &function_type));
      }

      function::~function()
      {
      }

      PyObject* function::call(PyObject* args, PyObject* keywords) const
      {
	std::size_t n_unnamed_actual = PyTuple_GET_SIZE(args);

// #define BOOST_PYTHON_NO_MULTIPLE_VALS_FOR_KEYWORD_ARGUMENT_CHECK
#ifndef BOOST_PYTHON_NO_MULTIPLE_VALS_FOR_KEYWORD_ARGUMENT_CHECK
	// check the 'multiple values for keyword argument' case:
	// foreach keyword, if n_actual 
	// is greater than 
	PyObject* keys = keywords ? PyDict_Keys(keywords) : 0;
	
	Py_ssize_t keysize = keys ? PyList_Size(keys) : 0;
	for (Py_ssize_t i=0; i<keysize; i++)
	  {
	    PyObject* item = PyList_GetItem(keys, i); // borrowed
	    char* c_str = PyString_AsString(item);    // assume string
	    std::string k(c_str);
	    unordered_map<std::string, unsigned>::const_iterator citer;
	    citer = keyword_arg_positions.find(k);
	    if (citer != keyword_arg_positions.end()
		&& citer->second < n_unnamed_actual)
	      multiple_values_for_keyword_argument_error(k);
	  }

	Py_XDECREF(keys);
#endif

	// for all overloads, assemble argpack and get score.
	// keep the best (lowest) scoring 'n' overloads in the best_overloads
	// vector.
	best_score = boost::none;
	best_overloads.clear();
	assert(!best_score);
	assert(best_overloads.size() == 0);

	std::size_t n_keyword_actual = keywords ? PyDict_Size(keywords) : 0;
	std::size_t n_actual = n_unnamed_actual + n_keyword_actual;

	function const* f = this;
	do 
	  {
	    // Check for a plausible number of arguments
	    unsigned min_arity = f->m_fn.min_arity();
	    unsigned max_arity = f->m_fn.max_arity();

	    if (n_actual + f->m_nkeyword_values >= min_arity
		&& n_actual <= max_arity)
	      {
		// This will be the args that actually get passed
		handle<> inner_args(allow_null(borrowed(args)));

		if (n_keyword_actual > 0      // Keyword arguments were supplied
		    || n_actual < min_arity) // or default keyword values are needed
		  {                            
		    if (f->m_arg_names.ptr() == Py_None) 
		      {
			// this overload doesn't accept keywords
			inner_args = handle<>();
		      }
		    else
		      {
			// "all keywords are none" is a special case
			// indicating we will accept any number of keyword
			// arguments
			if (PyTuple_Size(f->m_arg_names.ptr()) == 0)
			  {
			    // no argument preprocessing
			  }
			else if (n_actual > max_arity)
			  {
			    // too many arguments
			    inner_args = handle<>();
			  }
			else
			  {
			    // build a new arg tuple, will adjust its size later
			    assert(max_arity <= static_cast<std::size_t>(ssize_t_max));
			    inner_args = handle<>(PyTuple_New(static_cast<ssize_t>(max_arity)));

			    // Fill in the positional arguments
			    for (std::size_t i = 0; i < n_unnamed_actual; ++i)
			      PyTuple_SET_ITEM(inner_args.get(), i, incref(PyTuple_GET_ITEM(args, i)));

			    // Grab remaining arguments by name from the keyword dictionary
			    std::size_t n_actual_processed = n_unnamed_actual;
                
			    for (std::size_t arg_pos = n_unnamed_actual; arg_pos < max_arity ; ++arg_pos)
			      {
				// Get the keyword[, value pair] corresponding
				PyObject* kv = PyTuple_GET_ITEM(f->m_arg_names.ptr(), arg_pos);
				//log("kv", kv);
				// If there were any keyword arguments,
				// look up the one we need for this
				// argument position
				PyObject* key = PyTuple_GET_ITEM(kv, 0);
				//log("key", key);

				PyObject* value = n_keyword_actual
				  ? PyDict_GetItem(keywords, PyTuple_GET_ITEM(kv, 0))
				  : 0;
				//log("value", value);
				if (!value)
				  {
				    // Not found; check if there's a default value
				    if (PyTuple_GET_SIZE(kv) > 1)
				      value = PyTuple_GET_ITEM(kv, 1);

				    if (!value)
				      {
					if(PyErr_Occurred())
					  {
					    PyErr_Print();
					  }

					// still not found; matching fails
					PyErr_Clear();
					inner_args = handle<>();
					break;
				      }
				  }
				else
				  {
				    ++n_actual_processed;
				  }

				PyTuple_SET_ITEM(inner_args.get(), arg_pos, incref(value));
				//log("now inner_args", inner_args.get());
			      }

			    if (inner_args.get())
			      {
				//check if we proccessed all the arguments
				if(n_actual_processed < n_actual)
				  inner_args = handle<>();
			      }
			  }
		      }
		  }
            
		if(!is_overloaded)
		  {
		    best_score = 0u;
		    best_overloads.push_back(std::make_pair(f, inner_args));
		    goto do_call; //  \o/
		  }

		boost::optional<unsigned> this_score;
		// if the overload is 'not implemented' give it the worst
		// possible score so that any other overloads are chosen
		// first
		if(f == not_implemented_function().get())
		  this_score = std::numeric_limits<unsigned>::max();
		else if(inner_args)
		  this_score = f->m_fn.score(inner_args.get());
		else
		  this_score = 0u;

		if (this_score) // this one would work
		  {
		    if (best_score)
		      {
			// and we've got something strictly better
			if (*this_score < *best_score) 
			  {
			    best_overloads.clear();
			    best_score = this_score;
			    best_overloads.push_back(std::make_pair(f, inner_args));
			  }
			else if (*best_score == *this_score)
			  {
			    best_overloads.push_back(std::make_pair(f, inner_args));
			  }
		      }
		    else
		      {
			best_score = this_score;
			best_overloads.push_back(std::make_pair(f, inner_args));
		      }
		  }
	      }
	    f = f->m_overloads.get();

	    logobj("about to loop, f", f);
	  }
	while (f && (!best_score || *best_score != 0));

	if (best_overloads.size() > 1)
	  {
	    ambiguous_call_error();
	    best_overloads.clear();
	    return 0;
	    // error: ambiguous
	  }
	else if (best_overloads.size() == 0)
	  {
	    // None of the overloads matched; time to generate the error message
	    argument_error(args, keywords);
	    return 0;
	  }	  

      do_call:

	// Call the function.  Pass keywords in case it's a
	// function accepting any number of keywords
	PyObject* result 
	  = best_overloads[0].first->m_fn(best_overloads[0].second.get(), keywords);
	best_overloads.clear();

	// If the result is NULL but no error was set, m_fn failed
	// the argument-matching test.

	// This assumes that all other error-reporters are
	// well-behaved and never return NULL to python without
	// setting an error.
	if (result != 0 || PyErr_Occurred())
	    return result;

	argument_error(args, keywords);
	return 0;
      }

      object function::signature(bool show_return_type) const
      {
	py_function const& impl = m_fn;
    
	python::detail::signature_element const* return_type = impl.signature();
	python::detail::signature_element const* s = return_type + 1;
    
	list formal_params;
	if (impl.max_arity() == 0)
	  formal_params.append("void");

	for (unsigned n = 0; n < impl.max_arity(); ++n)
	  {
	    if (s[n].basename == 0)
	      {
		formal_params.append("...");
		break;
	      }

	    str param(s[n].basename);
	    if (s[n].lvalue)
	      param += " {lvalue}";
        
	    if (m_arg_names) // None or empty tuple will test false
	      {
		object kv(m_arg_names[n]);
		if (kv)
		  {
		    char const* const fmt = len(kv) > 1 ? " %s=%r" : " %s";
		    param += fmt % kv;
		  }
	      }
        
	    formal_params.append(param);
	  }

	if (show_return_type)
	  return "%s(%s) -> %s" % make_tuple(m_name, 
					     str(", ").join(formal_params), 
					     return_type->basename);
	return "%s(%s)" % make_tuple(m_name, 
				     str(", ").join(formal_params));
      }

      object function::signatures(bool show_return_type) const
      {
	list result;
	for (function const* f = this; f; f = f->m_overloads.get()) {
	  result.append(f->signature(show_return_type));
	}
	return result;
      }

      void function::argument_error(PyObject* args, PyObject* /*keywords*/) const
      {
	static handle<> exception(PyErr_NewException(const_cast<char*>("Boost.Python.ArgumentError"), 
						     PyExc_TypeError, 0));

	object message = "Python argument types in\n    %s.%s("
	  % make_tuple(this->m_namespace, this->m_name);
    
	list actual_args;
	for (ssize_t i = 0; i < PyTuple_Size(args); ++i)
	  {
	    char const* name = PyTuple_GetItem(args, i)->ob_type->tp_name;
	    actual_args.append(str(name));
	  }
	message += str(", ").join(actual_args);
	message += ")\ndid not match C++ signature:\n    ";
	message += str("\n    ").join(signatures());

#if BOOST_PYTHON_DEBUG_ERROR_MESSAGES
	std::printf("\n--------\n%s\n--------\n", extract<const char*>(message)());
#endif 
	PyErr_SetObject(exception.get(), message.ptr());
	throw_error_already_set();
      }

      void function::ambiguous_overload_error(function* other)
      {
	static handle<> exception(PyErr_NewException(const_cast<char*>("Boost.Python.AmbiguousOverload"), 
						     PyExc_TypeError, 0));

	this->m_name = other->m_name;

	object message = "Boost.Python function %s.%s\nhas ambiguous overloads.  C++ signatures\n  %s\nand\n  %s\nare indistinguishable to python."
	  % make_tuple(other->m_namespace, other->m_name, 
		       other->signature(true),
		       this->signature(true));
    
	PyErr_SetObject(exception.get(), message.ptr());
	throw_error_already_set();
      }

      void function::ambiguous_call_error() const
      {
	static handle<> exception(PyErr_NewException(const_cast<char*>("Boost.Python.AmbiguousCall"), 
						     PyExc_TypeError, 0));

	str oloads;
	for (int i=0; i<best_overloads.size(); i++)
	  oloads += "\n    %s" % best_overloads[i].first->signature();

	object message = "Ambiguous call to Boost.Python function %s.%s\nC++ signatures:%s"
	  % make_tuple(m_namespace, m_name, oloads);

	PyErr_SetObject(exception.get(), message.ptr());
	throw_error_already_set();
      }

      void function::multiple_values_for_keyword_argument_error(std::string keyword) const
      {
	static handle<> exception(PyErr_NewException(const_cast<char*>("Boost.Python.TypeError"), 
						     PyExc_TypeError, 0));

	object message = "%s.%s() got multiple values for keyword argument '%s'"
	  % make_tuple(this->m_namespace, this->m_name, keyword);
    
	PyErr_SetObject(exception.get(), message.ptr());
	throw_error_already_set();
      }


      bool function::has_same_signature_as(function* rhs) const
      {
	if (m_fn.min_arity() != rhs->m_fn.min_arity())
	  return false;

	if (m_fn.max_arity() != rhs->m_fn.max_arity())
	  return false;

	python::detail::signature_element const* our_args = m_fn.signature();
	python::detail::signature_element const* their_args = rhs->m_fn.signature();
	assert(our_args);
	assert(their_args);
	
	for (int i=1; // start from one... zero is return type
	     i<=m_fn.max_arity(); i++)
	  {
	    if (our_args[i].pytype_f() != their_args[i].pytype_f())
	      return false;

	    if (our_args[i].lvalue != their_args[i].lvalue)
	      return false;
	  }
	return true;
      }

      //
      // ambigcheck here is turned off when adding a not_implemented_function
      // overload when adding operators.
      //
      void function::add_overload(handle<function> const& overload_, 
				  bool ambigcheck = true)
      {
	function* parent = this;
	parent->is_overloaded = true;

	if (ambigcheck && has_same_signature_as(overload_.get()))
	  ambiguous_overload_error(overload_.get());

	while (parent->m_overloads)
	  parent = parent->m_overloads.get();

	parent->m_overloads = overload_;

	// If we have no documentation, get the docs from the overload
	if (!m_doc)
	  m_doc = overload_->m_doc;
      }

      namespace
      {
	char const* const binary_operator_names[] =
	  {
	    "add__",
	    "and__",
	    "div__",
	    "divmod__",
	    "eq__",
	    "floordiv__",
	    "ge__",
	    "gt__",
	    "le__",
	    "lshift__",
	    "lt__",
	    "mod__",
	    "mul__",
	    "ne__",
	    "or__",
	    "pow__",
	    "radd__",
	    "rand__",
	    "rdiv__",
	    "rdivmod__", 
	    "rfloordiv__",
	    "rlshift__",
	    "rmod__",
	    "rmul__",
	    "ror__",
	    "rpow__", 
	    "rrshift__",
	    "rshift__",
	    "rsub__",
	    "rtruediv__",
	    "rxor__",
	    "sub__",
	    "truediv__", 
	    "xor__"
	  };

	struct less_cstring
	{
	  bool operator()(char const* x, char const* y) const
	  {
	    return BOOST_CSTD_::strcmp(x,y) < 0;
	  }
	};
  
	inline bool is_binary_operator(char const* name)
	{
	  return name[0] == '_'
	    && name[1] == '_'
	    && std::binary_search(&binary_operator_names[0], 
				  binary_operator_names 
				  + sizeof(binary_operator_names)/sizeof(*binary_operator_names), 
				  name + 2, 
				  less_cstring());
	}

	// Something for the end of the chain of binary operators
	PyObject* not_implemented(PyObject*, PyObject*)
	{
	  Py_INCREF(Py_NotImplemented);
	  return Py_NotImplemented;
	}
  
	handle<function> not_implemented_function()
	{
      
	  static object keeper(function_object(py_function(&not_implemented, 
							   mpl::vector1<void>(), 2), 
					       python::detail::keyword_range()));
	  return handle<function>(borrowed(downcast<function>(keeper.ptr())));
	}
      }

      void function::add_to_namespace(object const& name_space, 
				      char const* name_, 
				      object const& attribute)
      {
	log_trace("%s", __PRETTY_FUNCTION__);
	add_to_namespace(name_space, name_, attribute, 0);
      }

      namespace detail
      {
	extern char py_signature_tag[];
	extern char cpp_signature_tag[];
      }

      void function::add_to_namespace(object const& name_space, 
				      char const* name_, 
				      object const& attribute, 
				      char const* doc)
      {
	log_trace("%s", __PRETTY_FUNCTION__);
	str const name(name_);
	PyObject* const ns = name_space.ptr();
    
	if (attribute.ptr()->ob_type == &function_type)
	  {
	    function* new_func = downcast<function>(attribute.ptr());
	    PyObject* dict = 0;
        
#if PY_VERSION_HEX < 0x03000000
	    // Old-style class gone in Python 3
	    if (PyClass_Check(ns))
	      dict = ((PyClassObject*)ns)->cl_dict;
	    else
#endif        
	      if (PyType_Check(ns))
		dict = ((PyTypeObject*)ns)->tp_dict;
	      else    
		dict = PyObject_GetAttrString(ns, const_cast<char*>("__dict__"));

	    if (dict == 0)
	      throw_error_already_set();

	    handle<> existing(allow_null(::PyObject_GetItem(dict, name.ptr())));
        
	    if (existing)
	      {
		log_trace("%s", "yes existing");
		if (existing->ob_type == &function_type)
		  {
		    new_func->add_overload
		      (handle<function>
		       (borrowed
			(downcast<function>(existing.get()))));
		  }
		else if (existing->ob_type == &PyStaticMethod_Type)
		  {
		    char const* name_space_name 
		      = extract<char const*>(name_space.attr("__name__"));
                
		    ::PyErr_Format(PyExc_RuntimeError, 
				   "Boost.Python - All overloads must be exported "
				   "before calling \'class_<...>(\"%s\").staticmethod(\"%s\")\'", 
				   name_space_name, 
				   name_);

		    throw_error_already_set();
		  }
	      }
	    else if (is_binary_operator(name_))
	      {
		// Binary operators need an additional overload which
		// returns NotImplemented, so that Python will try the
		// __rxxx__ functions on the other operand. We add this
		// when no overloads for the operator already exist.
		new_func->add_overload(not_implemented_function(),
				       false /* no ambig_check */);
	      }

	    // A function is named the first time it is added to a namespace.
	    if (new_func->name().ptr() == Py_None)
	      new_func->m_name = name;

	    handle<> name_space_name
	      (allow_null
	       (::PyObject_GetAttrString
		(name_space.ptr(), const_cast<char*>("__name__"))));
        
	    if (name_space_name)
	      new_func->m_namespace = object(name_space_name);
	  }

	// The PyObject_GetAttrString() or PyObject_GetItem calls above may
	// have left an active error
	PyErr_Clear();
	if (PyObject_SetAttr(ns, name.ptr(), attribute.ptr()) < 0)
	  throw_error_already_set();

	object mutable_attribute(attribute);
	str _doc;

	if (docstring_options::show_py_signatures_)
	  {
	    _doc += str(const_cast<const char*>(detail::py_signature_tag));
	  }
	if (doc != 0 && docstring_options::show_user_defined_)
	  _doc += doc;

	if (docstring_options::show_cpp_signatures_)
	  {
	    _doc += str(const_cast<const char*>(detail::cpp_signature_tag));
	  }
	if(_doc)
	  {    
	    object mutable_attribute(attribute);
	    mutable_attribute.attr("__doc__")= _doc;
	  }
      }

      BOOST_PYTHON_DECL void add_to_namespace(object const& name_space, 
					      char const* name, 
					      object const& attribute)
      {
	log_trace("%s", __PRETTY_FUNCTION__);
	logobj("attribute", attribute.ptr());
	function::add_to_namespace(name_space, name, attribute, 0);
      }

      BOOST_PYTHON_DECL void add_to_namespace(object const& name_space, 
					      char const* name, 
					      object const& attribute, 
					      char const* doc)
      {
	log_trace("add_to_namespace name=%s doc=%s", name % doc);
	logobj("attribute", attribute.ptr());
	function::add_to_namespace(name_space, name, attribute, doc);
      }


      namespace
      {
	struct bind_return
	{
	  bind_return(PyObject*& result, function const* f, 
		      PyObject* args, PyObject* keywords)
	    : m_result(result)
            , m_f(f)
            , m_args(args)
            , m_keywords(keywords)
	  {}

	  void operator()() const
	  {
	    log_trace("%s", __PRETTY_FUNCTION__);
	    m_result = m_f->call(m_args, m_keywords);
	  }
      
	private:
	  PyObject*& m_result;
	  function const* m_f;
	  PyObject* m_args;
	  PyObject* m_keywords;
	};
      }

      extern "C"
      {
	// Stolen from Python's funcobject.c
	static PyObject *
	function_descr_get(PyObject *func, PyObject *obj, PyObject *type_)
	{
#if PY_VERSION_HEX >= 0x03000000
	  // The implement is different in Python 3 because of the removal of unbound method
	  if (obj == Py_None || obj == NULL) {
            Py_INCREF(func);
            return func;
	  }
	  return PyMethod_New(func, obj);
#else
	  if (obj == Py_None)
            obj = NULL;
	  return PyMethod_New(func, obj, type_);
#endif
	}

	static void
	function_dealloc(PyObject* p)
	{
	  delete static_cast<function*>(p);
	}

	static PyObject *
	function_call(PyObject *func, PyObject *args, PyObject *kw)
	{
	  log_trace("%s func=%p args=%p kw=%p", 
		    __PRETTY_FUNCTION__ % func % args % kw);
	  logobj("func", func);
	  logobj("args", args);
	  logobj("kw", kw);
	  PyObject* result = 0;
	  handle_exception(bind_return(result, static_cast<function*>(func), args, kw));
	  return result;
	}

	//
	// Here we're using the function's tp_getset rather than its
	// tp_members to set up __doc__ and __name__, because tp_members
	// really depends on having a POD object type (it relies on
	// offsets). It might make sense to reformulate function as a POD
	// at some point, but this is much more expedient.
	//
	static PyObject* function_get_doc(PyObject* op, void*)
	{
	  function* f = downcast<function>(op);
	  list signatures = function_doc_signature_generator::function_doc_signatures(f);
	  if(!signatures) return python::detail::none();
	  signatures.reverse();
	  return python::incref( str("\n").join(signatures).ptr());
	}
    
	static int function_set_doc(PyObject* op, PyObject* doc, void*)
	{
	  function* f = downcast<function>(op);
	  f->doc(doc ? object(python::detail::borrowed_reference(doc)) : object());
	  return 0;
	}
    
	static PyObject* function_get_name(PyObject* op, void*)
	{
	  function* f = downcast<function>(op);
	  if (f->name().ptr() == Py_None)
#if PY_VERSION_HEX >= 0x03000000
            return PyUnicode_InternFromString("<unnamed Boost.Python function>");
#else
	  return PyString_InternFromString("<unnamed Boost.Python function>");
#endif
	  else
            return python::incref(f->name().ptr());
	}

	// We add a dummy __class__ attribute in order to fool PyDoc into
	// treating these as built-in functions and scanning their
	// documentation
	static PyObject* function_get_class(PyObject* /*op*/, void*)
	{
	  return python::incref(upcast<PyObject>(&PyCFunction_Type));
	}
      }

      static PyGetSetDef function_getsetlist[] = {
	{const_cast<char*>("__name__"), (getter)function_get_name, 0, 0, 0 },
	{const_cast<char*>("func_name"), (getter)function_get_name, 0, 0, 0 },
	{const_cast<char*>("__class__"), (getter)function_get_class, 0, 0, 0 },    // see note above
	{const_cast<char*>("__doc__"), (getter)function_get_doc, (setter)function_set_doc, 0, 0},
	{const_cast<char*>("func_doc"), (getter)function_get_doc, (setter)function_set_doc, 0, 0},
	{NULL, 0, 0, 0, 0} /* Sentinel */
      };

      PyTypeObject function_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	const_cast<char*>("Boost.Python.function"),
	sizeof(function),
	0,
	(destructor)function_dealloc,               /* tp_dealloc */
	0,                                  /* tp_print */
	0,                                  /* tp_getattr */
	0,                                  /* tp_setattr */
	0,                                  /* tp_compare */
	0, //(reprfunc)func_repr,                   /* tp_repr */
	0,                                  /* tp_as_number */
	0,                                  /* tp_as_sequence */
	0,                                  /* tp_as_mapping */
	0,                                  /* tp_hash */
	function_call,                              /* tp_call */
	0,                                  /* tp_str */
	0, // PyObject_GenericGetAttr,            /* tp_getattro */
	0, // PyObject_GenericSetAttr,            /* tp_setattro */
	0,                                  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT /* | Py_TPFLAGS_HAVE_GC */,/* tp_flags */
	0,                                  /* tp_doc */
	0, // (traverseproc)func_traverse,          /* tp_traverse */
	0,                                  /* tp_clear */
	0,                                  /* tp_richcompare */
	0, //offsetof(PyFunctionObject, func_weakreflist), /* tp_weaklistoffset */
	0,                                  /* tp_iter */
	0,                                  /* tp_iternext */
	0,                                  /* tp_methods */
	0, // func_memberlist,              /* tp_members */
	function_getsetlist,                /* tp_getset */
	0,                                  /* tp_base */
	0,                                  /* tp_dict */
	function_descr_get,                 /* tp_descr_get */
	0,                                  /* tp_descr_set */
	0, //offsetof(PyFunctionObject, func_dict),      /* tp_dictoffset */
	0,                                      /* tp_init */
	0,                                      /* tp_alloc */
	0,                                      /* tp_new */
	0,                                      /* tp_free */
	0,                                      /* tp_is_gc */
	0,                                      /* tp_bases */
	0,                                      /* tp_mro */
	0,                                      /* tp_cache */
	0,                                      /* tp_subclasses */
	0,                                      /* tp_weaklist */
#if PYTHON_API_VERSION >= 1012
	0                                       /* tp_del */
#endif
      };

      object function_object(py_function const& f, 
			     python::detail::keyword_range const& keywords)
      {
	return python::object
	  (python::detail::new_non_null_reference
	   (new function
	    (f, keywords.first, keywords.second - keywords.first)));
      }

      object function_object(py_function const& f)
      {
	return function_object(f, python::detail::keyword_range());
      }


      handle<> function_handle_impl(py_function const& f)
      {
	return python::handle<>(allow_null(new function(f, 0, 0)));
      }

    } // namespace objects

    namespace detail
    {

      object BOOST_PYTHON_DECL make_raw_function(objects::py_function f)
      {
	log_trace("%s", __PRETTY_FUNCTION__);
	static keyword k;
    
	return objects::function_object(f, keyword_range(&k,&k));
      }

      void BOOST_PYTHON_DECL pure_virtual_called()
      {
	PyErr_SetString(PyExc_RuntimeError, 
			const_cast<char*>("Pure virtual function called"));
	throw_error_already_set();
      }
    }

  }} // namespace boost::python
