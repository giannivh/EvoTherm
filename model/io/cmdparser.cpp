//
// cmdparser.cpp is part of EvoTherm.
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
// Time: 17:55
//

#include <algorithm>
#include "cmdparser.h"

const std::string CMDParser::NO_VALUE = "";

CMDParser::CMDParser(const int &argc, const char *argv[]) {

    for (int i = 1; i < argc; ++i) {

        this->tokens.push_back(std::string(argv[i]));
    }
}

const std::string &CMDParser::getCMDOptionValue(const std::string &shortOption, const std::string &longOption) const {

    std::vector<std::string>::const_iterator itr;

    // find using short option
    itr = std::find(this->tokens.begin(), this->tokens.end(), shortOption);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()) {

        return *itr;
    }

    // find using long option
    itr = std::find(this->tokens.begin(), this->tokens.end(), longOption);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()) {

        return *itr;
    }

    // not found
    return CMDParser::NO_VALUE;
}

bool CMDParser::cmdOptionGiven(const std::string &shortOption, const std::string &longOption) const {

    return std::find(this->tokens.begin(), this->tokens.end(), shortOption) != this->tokens.end() ||
            std::find(this->tokens.begin(), this->tokens.end(), longOption) != this->tokens.end();
}