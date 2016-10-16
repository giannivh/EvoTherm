//
// exitcode.h is part of EvoTherm.
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
// Date: 14/10/16
// Time: 19:33
//

#ifndef EVOTHERM_EXITCODE_H
#define EVOTHERM_EXITCODE_H

#define EXIT_NO_CONFIG          1
#define EXIT_INVALID_CONFIG     2
#define EXIT_CURL_ERROR         3
#define EXIT_CURL_RESULT_ERROR  4
#define EXIT_LOGIN_FAILED       5
#define EXIT_USER_DATA_ERROR    6
#define EXIT_INSTALL_DATA_ERROR 7
#define EXIT_TEMP_DATA_ERROR    8
#define EXIT_UNKNOWN_CMD_OPTION 9
#define EXIT_INVALID_MODE       10
#define EXIT_INVALID_ZONE       11
#define EXIT_INVALID_ZONE_PARAM 12
#define EXIT_INVALID_TEMP       13
#define EXIT_TEMP_SET_FAILED    14
#define EXIT_MODE_SET_FAILED    15

#endif //EVOTHERM_EXITCODE_H
