//
// temperatureutil.cpp is part of evotherm.
// 
// Copyright (c) 2016 Gianni Van Hoecke <gianni.vh@gmail.com>
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// User: Gianni Van Hoecke <gianni.vh@gmail.com>
// Date: 19/10/16
// Time: 12:41
//

#include "temperatureutil.h"
#include "../exception/evothermexception.h"
#include "../exitcode.h"

const void TemperatureUtil::assertValidTemperature(const std::string &temp) {

    if (temp == "") {

        throw EvoThermException("Invalid zone parameters. --temp TEMP is required.", EXIT_INVALID_ZONE_PARAM);
    }

    char *endptr = 0;
    strtod(temp.c_str(), &endptr);
    if (*endptr != '\0' || endptr == temp) {

        throw EvoThermException("Invalid temperature " + temp + ".", EXIT_INVALID_TEMP);
    }
}
