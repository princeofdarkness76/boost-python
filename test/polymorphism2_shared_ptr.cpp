// Copyright Troy D. Straszheim 2009. 
// Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

namespace boost {
  template <typename T> struct shared_ptr;
}

#define HOLDER(X) , boost::shared_ptr< X >
#define HELD_PTR_ARG(X) boost::shared_ptr< X >
#define MODNAME() (polymorphism2_shared_ptr_ext)

#include "polymorphism2.cpp"
