#ifndef __BOOST_PYTHON_CONVERTERS_H
#define __BOOST_PYTHON_CONVERTERS_H

#include <boost/python.hpp>

#include "exception_wrap.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(boost_python_converters)
{
    wrap::exception<std::runtime_error>("RuntimeError", init<std::string>())
        .def("__str__", &std::runtime_error::what);
}

#endif