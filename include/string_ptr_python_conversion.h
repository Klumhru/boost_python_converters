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
#ifndef __WSTRING_PTR_PYTHON_CONVERSION_H
#define __WSTRING_PTR_PYTHON_CONVERSION_H

#include <iostream>
#include "orkuveita_ws_typedefs.h"

#include <boost/python.hpp>
#include "register_python_conversion.h"

struct StringToPy
{
  static PyObject* convert(String* pt)
  {
    if(!pt)
    {
      Py_RETURN_NONE;
    }
    PyObject* ret = PyUnicode_FromWideChar(pt->c_str(), pt->size());
    return ret;
  }
};


struct StringFromPy
{
  static void* convertible(PyObject* obj_ptr)
  {
    std::wcout << "Checking for string" << std::endl;
    if(PyUnicode_Check(obj_ptr))
    {
      std::wcout << "We're a unicode!" << std::endl;
      return obj_ptr;
    }
    if(PyString_Check(obj_ptr))
    {
      std::wcout << "We're a string!" << std::endl;
      return obj_ptr;
    }
    std::wcout << "We're not a string :(" << std::endl;
    return 0;
  }

  static void construct(PyObject* obj_ptr,
                        boost::python::converter::rvalue_from_python_stage1_data* data)
  {
    String* pt = new String();
    if(PyUnicode_Check(obj_ptr))
    {
      pt->resize(PyUnicode_GET_SIZE(obj_ptr));
      PyUnicode_AsWideChar((PyUnicodeObject*) obj_ptr, &pt->front(), pt->size());
    }
    else if (PyString_Check(obj_ptr))
    {
      std::string buf;
      buf.assign(PyString_AsString(obj_ptr));
      pt->assign(buf.begin(), buf.end());
    }

    data->convertible = (void*)pt;
  }
};

typedef register_python_conversion<
  String*,
  StringToPy, StringFromPy> string_python_conversion;

#endif
