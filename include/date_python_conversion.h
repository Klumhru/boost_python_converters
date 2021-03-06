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
#ifndef __BIND_DATETIME
#define __BIND_DATETIME

#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/python.hpp>
#include "register_python_conversion.h"
#include "datetime.h" // Python datetime

using namespace boost::gregorian;
using namespace boost::posix_time;

const std::locale ws_iso_format = std::locale(std::locale::classic(),
    new time_input_facet("%Y-%m-%dT%H:%M:%s"));

void wstringToTime(const std::wstring& ws, Time& pt)
{
    std::string s;
    s.assign(ws.begin(), ws.end());
    std::istringstream is(s);
    is.imbue(ws_iso_format);
    is >> pt;
}


struct DateTimeToPy
{
  static PyObject* convert(TimeParam pt)
  {
    PyDateTime_IMPORT;
    boost::gregorian::date date = pt.date();
    boost::posix_time::time_duration td = pt.time_of_day();

    return PyDateTime_FromDateAndTime(date.year(), date.month(), date.day(),
                                      td.hours(), td.minutes(), td.seconds(),
                                      td.total_microseconds());
  }
};


struct DateTimeFromPy
{
  static void* convertible(PyObject* obj_ptr)
  {
    PyDateTime_IMPORT;
    if(PyDateTime_Check(obj_ptr)) return obj_ptr;
    return 0;
  }
  static void construct(PyObject* obj_ptr,
                        boost::python::converter::rvalue_from_python_stage1_data* data)
  {
    PyDateTime_IMPORT;
    int y = PyDateTime_GET_YEAR(obj_ptr);
    int M = PyDateTime_GET_MONTH(obj_ptr);
    int d = PyDateTime_GET_DAY(obj_ptr);
    int h = PyDateTime_DATE_GET_HOUR(obj_ptr);
    int m = PyDateTime_DATE_GET_MINUTE(obj_ptr);
    int s = PyDateTime_DATE_GET_SECOND(obj_ptr);
    int ms = PyDateTime_DATE_GET_MICROSECOND(obj_ptr);

    boost::gregorian::date date(y, M, d);
    boost::posix_time::time_duration td(h, m, s);

    Time* pt = new Time(date, td+boost::posix_time::microseconds(ms));
    data->convertible = (void*)pt;
  }
};

typedef register_python_conversion<Time, DateTimeToPy, DateTimeFromPy> conversion_datetime;

#endif
