//
// HttpClient.cpp is part of EvoTherm.
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

#include <iostream>
#include "httpclient.h"
#include "../exitcode.h"

std::string HttpClient::request(const std::string &url) {

    std::vector<std::string> headerList;
    return request(url, headerList);
}

std::string HttpClient::request(const std::string &url, const std::vector<std::string> &headerList) {

    std::map<std::string, std::string> dataMap;
    return post(url, headerList, dataMap);
}

std::string HttpClient::post(const std::string &url, const std::vector<std::string> &headerList,
                             const std::map<std::string, std::string> &dataMap) {

    // init cURL
    CURL *curl = initCURL();

    // set post data, if any
    std::string data = "";
    for(auto const &entry : dataMap) {

        if (data.length() > 0) {

            data.append("&");
        }

        data.append(entry.first + "=" + urlEncode(curl, entry.second));
    }

    // do request
    return perform(curl, url, headerList, data);
}

std::string HttpClient::put(const std::string &url, const std::vector<std::string> &headerList, const std::string data) {

    // init cURL
    CURL *curl = initCURL();
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

    // do request
    return perform(curl, url, headerList, data);
}

std::string HttpClient::perform(CURL *curl, const std::string &url, const std::vector<std::string> &headerList,
                                const std::string data) {

    // set url
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // set header
    struct curl_slist *headers = NULL;
    for (int i = 0; i < headerList.size(); i++) {

        headers = curl_slist_append(headers, headerList[i].c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // set post data
    if (data.length() > 0) {

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
    }

    // callback
    std::string readBuffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    // do request
    CURLcode result = curl_easy_perform(curl);

    // check for errors
    if (result != CURLE_OK) {

        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
        exit(EXIT_CURL_RESULT_ERROR);
    }

    // cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // global cleanup
    curl_global_cleanup();

    return readBuffer;
}

size_t HttpClient::writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {

    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

CURL *HttpClient::initCURL() {

    CURL *curl;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (!curl) {

        std::cerr << "Error starting cURL" << std::endl;
        exit(EXIT_CURL_ERROR);
    }

    return curl;
}

std::string HttpClient::urlEncode(CURL *curl, const std::string &s) {

    char *url_encoded = curl_easy_escape(curl, s.c_str(), 0);
    std::string url_encoded_str(url_encoded);
    curl_free(url_encoded);

    return url_encoded_str;
}