# boost_python_converters
Boost python converters for common types


This library contains some headers that define converters for basic std types and pointers plus python-datetime to boost-datetime converters.

The library is header only if you want to use it in your own project but contains some source examples and python tests that can be run in a virtualenv.

The library relies quite heavily on C++11 idioms and requires -std=c++11 parameter to gcc/g++

The library requires unicode and uses std::wstring internally to represent strings.
