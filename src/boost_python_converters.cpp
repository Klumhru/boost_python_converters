#include "boost_python_converters.h"

BOOST_PYTHON_MODULE(boost_python_converters)
{
    wrap::exception<std::runtime_error>("RuntimeError", init<std::string>())
        .def("__str__", &std::runtime_error::what);
}