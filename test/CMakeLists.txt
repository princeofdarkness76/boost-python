#
# Copyright Troy D. Straszheim
#
# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt
#
include_directories(${PYTHON_INCLUDE_PATH})

set(ENV{PYTHONPATH} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(VALGRIND_SUPPRESSIONS_FILE 
  ${CMAKE_SOURCE_DIR}/tools/build/CMake/externals/Python/valgrind-python-${PYTHON_VERSION}.supp)

if (WITH_VALGRIND AND (EXISTS ${VALGRIND_SUPPRESSIONS_FILE}))
  message(STATUS "+-- using valgrind-python suppressions for version ${PYTHON_VERSION}") 
  set(VALGRIND_FLAGS 
    --suppressions=${VALGRIND_SUPPRESSIONS_FILE};${VALGRIND_FLAGS})
endif()

# ryppl_test_run(exec 
#   DEPENDS boost_python STATIC 
#   ARGS "${CMAKE_CURRENT_SOURCE_DIR}/exec.py"
#   LINK_LIBS ${PYTHON_LIBRARIES})

add_executable(exec exec.cpp)
target_link_libraries(exec boost_python ${PYTHON_LIBRARIES})
add_test(exec ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/exec ${CMAKE_CURRENT_SOURCE_DIR}/exec.py)

macro(doctest TESTNAME)
  add_test(${TESTNAME}
    ${VALGRIND_EXECUTABLE}
    ${VALGRIND_FLAGS}
    ${PYTHON_EXECUTABLE} 
    "${CMAKE_CURRENT_SOURCE_DIR}/bpl_testdriver.py"
    "${PYTHONPATH}"
    ${TESTNAME})
endmacro()

macro(bpl_test TESTNAME SCRIPT)
  foreach(MODULE ${ARGN})
    python_extension(${MODULE} ${MODULE}.cpp)
  endforeach()
  doctest(${TESTNAME})
endmacro()

bpl_test(crossmod_exception 
  crossmod_exception.py crossmod_exception_a crossmod_exception_b)
# 
# bpl_test(numpy numpy_test.py printer.py numeric_tests.py numarray_tests.py numpy.cpp)
bpl_test(newtest newtest.py m1 m2)
#bpl_test(iterator iterator.py iterator input_iterator)

python_extension(iterator_ext iterator.cpp)
python_extension(input_iterator input_iterator.cpp)
doctest(iterator)

python_extension(stl_iterator_ext stl_iterator.cpp)
doctest(stl_iterator)

bpl_test(crossmod_opaque
  crossmod_opaque.py crossmod_opaque_a crossmod_opaque_b)

add_executable(select_from_python_test
  select_from_python_test.cpp ../src/converter/type_id.cpp)
target_link_libraries(select_from_python_test ${PYTHON_LIBRARIES})
add_test(select_from_python_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/select_from_python_test)

foreach(TESTNAME

    ambig1
    ambig2
    ambig3
    ambig4
    ambig5
    ambig6
    overload_resolution
    multi_arg_constructor
    multiple_values_for_keyword_arg
    injected
    properties
    return_arg
    staticmethod
    shared_ptr
    andreas_beyer
    polymorphism
    wrapper_held_type
    polymorphism2_auto_ptr
    polymorphism2_shared_ptr
    polymorphism2_no_ptr
    auto_ptr
    minimal
    args
    raw_ctor
    enum
    exception_translator
    cltree
    const_argument
    keywords
    builtin_converters
    extract
    opaque
    voidptr
    pickle1
    pickle2
    pickle3
    pickle4
    sandbox
    nested
    # converter_collisions1    TODO: "scoped converters" ???
    # converter_collisions2
    function_objects
    function_objects2
    function_objects3
    function_objects4
    phoenix
    phoenix2
    zillion_arguments_workaround
    docstring
    pytype_function
    vector_indexing_suite
    pointer_vector
    pointer_adoption
    operators
    operators_wrapper
    callbacks
    defaults
    object
    list
    long
    dict
    tuple
    str
    slice
    virtual_functions
    back_reference
    implicit
    data_members
    ben_scott1
    bienstman1
    bienstman2
    bienstman3
    bienstman4
    bienstman5
    haoyu_bai0
    multiple_inheritance
    virtual_inheritance
    konstantin_aslanidi
    return_pointee_value
    bind
    ravi
    udconversions
    # udconversions_ostream # fixme: no docstring or main()
    # numpy2 # fixme: no docstring or main()
    setattr
    # weak_ptr_to_pyheld_shared_ptr  # fixme:  jan fuchs problem
    )
  python_extension(${TESTNAME}_ext ${TESTNAME}.cpp)
  file(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" PYTHONPATH)
  if(WIN32 AND NOT UNIX)
    string(REPLACE "\\" "\\\\" PYTHONPATH "${PYTHONPATH}")
  endif(WIN32 AND NOT UNIX)
  add_test(${TESTNAME}
    ${VALGRIND_EXECUTABLE}
    ${VALGRIND_FLAGS}
    ${PYTHON_EXECUTABLE} 
    "${CMAKE_CURRENT_SOURCE_DIR}/bpl_testdriver.py"
    "${PYTHONPATH}"
    ${TESTNAME})

endforeach()

python_extension(map_indexing_suite_ext 
  map_indexing_suite.cpp int_map_indexing_suite.cpp a_map_indexing_suite.cpp)

doctest(map_indexing_suite)
  
# --- unit tests of library components ---

foreach(RUNTEST
    pointer_type_id_test
    bases
    pointee
    upcast
    destroy_test
    if_else
    result
    stl_iterator_distance
    )
  add_executable(${RUNTEST} ${RUNTEST}.cpp)
  target_link_libraries(${RUNTEST} boost_python ${PYTHON_LIBRARIES})
endforeach()

foreach(compiletest
    indirect_traits_test
    string_literal
    borrowed
    object_manager
    copy_ctor_mutates_rhs
    select_holder
    select_arg_to_python_test
    are_base_and_derived
    )
  add_test(${compiletest}
    ${CMAKE_CTEST_COMMAND}
    --build-and-test
    ${CMAKE_CURRENT_SOURCE_DIR}/CompileTest
    ${CMAKE_CURRENT_SOURCE_DIR}/CompileTest
    --build-generator ${CMAKE_GENERATOR}
    --build-makeprogram ${CMAKE_MAKE_PROGRAM}
    --build-project CompileTest
    --build-options 
    "-DSOURCE:STRING=${CMAKE_CURRENT_SOURCE_DIR}/${compiletest}.cpp"
    "-DINCLUDES:STRING=${INCLUDE_PATH}"
    )
endforeach()

foreach(failtest
    raw_pyobject_fail1
    raw_pyobject_fail2
    as_to_python_function
    object_fail1
    max_arity_too_high
    not_base_and_derived
    not_base_and_derived2
    not_base_and_derived3
    function_objects_const_check
    return_pointee_value_fail
    )
  add_test(${failtest}
    ${CMAKE_CTEST_COMMAND}
    --build-and-test
    ${CMAKE_CURRENT_SOURCE_DIR}/CompileTest
    ${CMAKE_CURRENT_SOURCE_DIR}/CompileTest
    --build-generator ${CMAKE_GENERATOR}
    --build-makeprogram ${CMAKE_MAKE_PROGRAM}
    --build-project CompileTest
    --build-options 
    "-DSOURCE:STRING=${CMAKE_CURRENT_SOURCE_DIR}/${failtest}.cpp"
    "-DINCLUDES:STRING=${INCLUDE_PATH}"
    )
  set_tests_properties(${failtest} PROPERTIES WILL_FAIL ON)      
endforeach()
