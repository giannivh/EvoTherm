//
// mode.cpp is part of EvoTherm.
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
// Time: 16:37
//

#include "mode.h"

Mode::Mode(const std::string &systemMode, const bool &canBePermanent, const bool &canBeTemporary)
        : systemMode(systemMode), canBePermanent(canBePermanent), canBeTemporary(canBeTemporary) {

}

const bool Mode::isValidMode(const std::string &mode) {

    return mode == "auto" || mode == "eco" || mode == "away" || mode == "dayoff" || mode == "off" || mode == "custom";
}

const std::string Mode::fromReadable(const std::string &mode) {

    std::string converted = "Auto";

    if (mode == "eco") {

        converted = "AutoWithEco";
    }
    else if (mode == "away") {

        converted = "Away";
    }
    else if (mode == "dayoff") {

        converted = "DayOff";
    }
    else if (mode == "off") {

        converted = "HeatingOff";
    }
    else if (mode == "custom") {

        converted = "Custom";
    }

    return converted;
}

const std::string Mode::toReadable(const std::string &mode) {

    std::string converted = "auto";

    if (mode == "AutoWithEco") {

        converted = "eco";
    }
    else if (mode == "Away") {

        converted = "away";
    }
    else if (mode == "DayOff") {

        converted = "dayoff";
    }
    else if (mode == "HeatingOff") {

        converted = "off";
    }
    else if (mode == "Custom") {

        converted = "custom";
    }

    return converted;
}
