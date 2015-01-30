// Copyright (c) 2015 Högni Gylfason

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
#ifndef __EXCEPTION_WRAP_H
#define __EXCEPTION_WRAP_H

#include <boost/python.hpp>

using namespace boost::python;

#include <stdexcept>
#include <iostream>
#include <string>
#include <boost/function.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

namespace {
   template<typename T>
   std::string wrap_output(T const& exn)
   {
      return exn.what();
   }
}

namespace wrap
{
   template< typename CPP_ExceptionType
           , typename X1 = ::boost::python::detail::not_specified
           , typename X2 = ::boost::python::detail::not_specified
           , typename X3 = ::boost::python::detail::not_specified
           >
   class exception
      : public ::boost::python::class_<CPP_ExceptionType, X1, X2, X3>
   {
   public:
      typedef ::boost::python::class_<CPP_ExceptionType, X1, X2, X3> base_type;
      typedef exception<CPP_ExceptionType, X1, X2, X3>               self;

      // Construct with the class name, with or without docstring, and default
      // __init__() function
      exception(char const* name, char const* doc = 0)
         : base_type(name, doc), m_exception_name(name)
      {
         init();
      }

      // Construct with class name, no docstring, and an uncallable
      // __init__ function
      exception(char const* name, no_init_t const& no_init_tag)
         : base_type(name, no_init_tag), m_exception_name(name)
      {
         init();
      }

      // Construct with class name, docstring, and an uncallable
      // __init__ function
      exception(char const* name, char const* doc, no_init_t const& no_init_tag)
         : base_type(name, doc, no_init_tag), m_exception_name(name)
      {
         init();
      }

      // Construct with class name and init<> function
      template <class DerivedT>
      inline exception(char const* name, init_base<DerivedT> const& i)
         : base_type(name, i), m_exception_name(name)
      {
         init();
      }

      // Construct with class name, docstring and init<> function
      template <class DerivedT>
      inline exception( char const* name
                      , char const* doc
                      , init_base<DerivedT> const& i)
         : base_type(name, doc, i), m_exception_name(name)
      {
         init();
      }

   private:
      std::string get_module_qualified_name() const
      {
         return boost::str(boost::format("%s.%s") % "pit" % m_exception_name);
      }

      void init() const
      {
         using namespace boost;
         function<void (typename base_type::wrapped_type const&)>
            conversion_func = bind( &exception::to_python_exception
                                  , *this, get_module_qualified_name(), _1);
         ::boost::python::register_exception_translator<typename
             base_type::wrapped_type>(conversion_func);
      }

      static void to_python_exception( ::boost::python::object const& exn_type
                                  , std::string const& exception_name
                                  , typename base_type::wrapped_type const& exn
                                  )
      {
         static const ::boost::python::to_python_value<typename
           base_type::wrapped_type> convert_argument;
         PyErr_SetObject(exn_type.ptr(), convert_argument(exn));

         throw_error_already_set();
      }

      std::string const m_exception_name;
   };
}

#endif