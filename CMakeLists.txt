#
# Copyright Troy D. Straszheim
#
# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt
#
cmake_minimum_required(VERSION 2.8)

enable_testing()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

find_package(PythonLibs)
find_package(PythonInterp)

if (CMAKE_BUILD_TYPE MATCHES "Deb")
  add_definitions(-DBOOST_PYTHON_DEBUG)
endif()

# set this as necessary
set(BOOST_ROOT "/usr/local/boost-1.42.0")

find_package(Boost)
message("Boost headers at ${Boost_INCLUDE_DIRS}")

set(INCLUDE_PATH   
  ${PYTHON_INCLUDE_PATH} 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS})

include_directories(${INCLUDE_PATH})

#
#  Macro for building python extensions
#
macro(python_extension MODULE_NAME)

  add_library(${MODULE_NAME} SHARED
    ${ARGN}
    )
  target_link_libraries(${MODULE_NAME} boost_python)

  set_target_properties(${MODULE_NAME}
    PROPERTIES
    OUTPUT_NAME "${MODULE_NAME}"
    PREFIX ""
    COMPILE_FLAGS "-DBPL_EXTENSION_MODULE=${MODULE_NAME}"
    )

endmacro()


add_subdirectory(src)
add_subdirectory(test)

