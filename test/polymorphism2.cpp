// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// This file is #included by polymorphism2_<holdertype>.cpp
// for holdertype in no_ptr, auto_ptr, shared_ptr
//

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/manage_new_object.hpp>
#include <boost/python/reference_existing_object.hpp>
#include <boost/python/pure_virtual.hpp>
#include <boost/python/wrapper.hpp>
#include <boost/python/def.hpp>
#include <boost/python/call.hpp>
#include <boost/utility.hpp>

#include <memory>

#include <boost/preprocessor/expand.hpp>

using namespace boost::python;

struct P
{
    virtual ~P(){}
    virtual char const* f() = 0;
    char const* g() { return "P::g()"; }
};

struct PCallback : P, wrapper<P>
{
    char const* f()
    {
        return this->get_override("f")();
    }
};

struct Q : virtual P
{
    char const* f() { return "Q::f()"; } 
};

struct A
{
    virtual ~A(){}
    virtual char const* f() { return "A::f()"; }
};

struct ACallback :  A, wrapper<A>
{
    char const* f()
    {
        if (override f = this->get_override("f"))
            return f();

        return A::f();
    }

    char const* default_f() { return this->A::f(); }
};

struct B : A
{
    virtual char const* f() { return "B::f()"; } 
};

struct C : A
{
    virtual char const* f() { return "C::f()"; }
};

struct D : A
{
    virtual char const* f() { return "D::f()"; }
    char const* g() { return "D::g()"; }
};

struct DCallback :  D,  wrapper<D>
{
    char const* f()
    {
        if (override f = this->get_override("f"))
            return f();
        //else
            return D::f();
    }
};

    
A& getBCppObj ()
{
    static B b;
    return b;
}

char const* call_f(A& a) { return a.f(); }

A* factory(unsigned choice)
{
    switch (choice % 3)
    {
    case 0: return new A;
        break;
    case 1: return new B;
        break;
    default: return new C;
        break;
    }
}

C& getCCppObj ()
{
    static C c;
    return c;
}

A* pass_a(A* x) { return x; }

BOOST_PP_EXPAND(BOOST_PYTHON_MODULE MODNAME())
{
  class_<ACallback HOLDER(ACallback),boost::noncopyable>("A")
        .def("f", &A::f, &ACallback::default_f)
        ;
    
    def("getBCppObj", getBCppObj, return_value_policy<reference_existing_object>());

    class_<C HOLDER(C),bases<A>,boost::noncopyable>("C")
        .def("f", &C::f)
        ;
    
    class_<DCallback HOLDER(DCallback),bases<A>,boost::noncopyable>("D")
        .def("f", &D::f)
        .def("g", &D::g)
        ;

    def("pass_a", &pass_a,  return_internal_reference<>());
    
    def("getCCppObj", getCCppObj, return_value_policy<reference_existing_object>());

    def("factory", factory, return_value_policy<manage_new_object>());

    def("call_f", call_f);

    class_<PCallback,boost::noncopyable>("P")
        .def("f", pure_virtual(&P::f))
        ;

    class_<Q HOLDER(Q), bases<P> >("Q")
        .def("g", &P::g) // make sure virtual inheritance doesn't interfere
        ;
}

//#include "module_tail.cpp"
