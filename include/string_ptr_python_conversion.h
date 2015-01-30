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
