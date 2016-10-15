//
// dateutil.cpp is part of evotherm.
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
// Date: 15/10/16
// Time: 16:42
//

#include "dateutil.h"

std::string DateUtil::toUTC(const std::string &local) {

    std::string utc = "";

    if (local != "") {

        struct tm cal = {0, 0, 0, 0, 0, 0, 0, 0, -1, 0, NULL};
        strptime(local.c_str(), "%Y-%m-%d %H:%M:%S", &cal);
        time_t time = mktime(&cal);
        char buffer[21];
        strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", gmtime(&time));

        utc = buffer;
    }

    return utc;
}

std::string DateUtil::toLocal(const std::string &utc) {

    std::string local = "";

    if (utc != "") {

        struct tm cal = {0, 0, 0, 0, 0, 0, 0, 0, -1, 0, NULL};
        strptime(utc.c_str(), "%Y-%m-%dT%H:%M:%SZ", &cal);
        time_t time = timegm(&cal);
        char buffer[21];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time));

        local = buffer;
    }

    return local;
}
