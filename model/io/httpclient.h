//
// HttpClient.h is part of EvoTherm.
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
// Date: 13/10/16
// Time: 14:05
//

#ifndef EVOTHERM_HTTPCLIENT_H
#define EVOTHERM_HTTPCLIENT_H

#include <string>
#include <map>
#include <vector>
#include <curl/curl.h>

class HttpClient {

public:

    static std::string request(const std::string &url);

    static std::string request(const std::string &url, const std::vector<std::string> &headerList);

    static std::string request(const std::string &url, const std::vector<std::string> &headerList,
                               const std::map<std::string, std::string> &dataMap);

    static std::string urlEncode(CURL *curl, const std::string &s);

private:

    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
};


#endif //EVOTHERM_HTTPCLIENT_H
