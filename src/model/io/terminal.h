//
// terminal.h is part of EvoTherm.
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
// Time: 18:05
//

#ifndef EVOTHERM_TERMINAL_H
#define EVOTHERM_TERMINAL_H

#include <string>
#include <vector>
#include "../evohome/data/zone.h"
#include "../evohome/data/installationinfo.h"
#include "../exception/evothermexception.h"

class Terminal {

public:

    void printHelp() const;
    void printHelp(const int &exitCode) const;
    void printZones(const InstallationInfo &installationInfo) const;
    void printError(const std::string &message) const;
    void printFatalError(const EvoThermException &evoThermException) const;

private:

    void printHelpOption(const std::string &shortOption, const std::string &longOption, const std::string &description) const;
    void printHelpMode(const std::string &mode, const std::string &description) const;
    void printHelpExample(const std::string &example) const;

};


#endif //EVOTHERM_TERMINAL_H
