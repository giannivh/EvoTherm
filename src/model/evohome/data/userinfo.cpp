//
// userinfo.cpp is part of EvoTherm.
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
// Time: 15:36
//

#include <string>
#include <iostream>
#include "userinfo.h"
#include "../../../ext/jsoncpp/json/json.h"
#include "../../exitcode.h"

void UserInfo::parse(const std::string &jsonData) {

    Json::Reader reader;
    Json::Value obj;

    if (!reader.parse(jsonData, obj) || obj["userId"].isNull()) {

        std::cerr << "Could not get user data." << std::endl;
        exit(EXIT_USER_DATA_ERROR);
    }

    this->userId = obj["userId"].asString();
    this->username = obj["username"].asString();
    this->firstname = obj["firstname"].asString();
    this->lastname = obj["lastname"].asString();
    this->streetAddress = obj["streetAddress"].asString();
    this->city = obj["city"].asString();
    this->postcode = obj["postcode"].asString();
    this->country = obj["country"].asString();
    this->language = obj["language"].asString();
}
