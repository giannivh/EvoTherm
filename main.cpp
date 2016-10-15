//
// main.cpp is part of EvoTherm.
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
// Date: 12/10/16
// Time: 15:51
//

#include <iostream>
#include "model/config.h"
#include "model/evohome/evohomeclient.h"
#include "model/io/cmdparser.h"
#include "model/io/terminal.h"
#include "model/exitcode.h"
#include "model/util/dateutil.h"

int main(int argc, char* argv[]) {

    CMDParser cmdParser = CMDParser(argc, (const char **) argv);
    Terminal terminal = Terminal();

    // show help if needed
    if (cmdParser.cmdOptionGiven("-h", "--help")) {

        terminal.printHelp();
    }

    // assert config validity
    Config config = Config::assertValidity();

    // we need data if we get here...
    std::cout << "Fetching data, please wait..." << std::flush;
    EvohomeClient evohomeClient = EvohomeClient(config);
    std::cout << "\r" << std::flush;

    // parse options
    if (argc == 1 || cmdParser.cmdOptionGiven("-l", "--list")) {

        // show the current status of all zones
        terminal.printZones(evohomeClient.getAllZones());
    }
    else if (cmdParser.cmdOptionGiven("-m", "--mode")) {

        // set thermostat mode
        const std::string &mode = cmdParser.getCMDOptionValue("-m", "--mode");

        if (mode == CMDParser::NO_VALUE || !Mode::isValidMode(mode)) {

            std::cout << std::endl;
            std::cerr << "Invalid mode." << std::endl;
            exit(EXIT_INVALID_MODE);
        }

        std::cout << "TODO: Setting mode to " << mode << std::endl;
        // todo set mode
    }
    else if (cmdParser.cmdOptionGiven("-z", "--zone")) {

        // get zone
        const std::string &zone = cmdParser.getCMDOptionValue("-z", "--zone");

        // check zone
        if (zone == CMDParser::NO_VALUE || !evohomeClient.hasZone(zone)) {

            std::cout << std::endl;
            std::cerr << "Invalid zone." << std::endl;
            exit(EXIT_INVALID_ZONE);
        }

        // operation on zone
        if (cmdParser.cmdOptionGiven("-c", "--cancel")) {

            // cancel override
            evohomeClient.cancelOverride(evohomeClient.getZoneByName(zone));

            // show the current status of all zones
            std::cout << std::endl;
            terminal.printZones(evohomeClient.getAllZones());
        }
        else {

            // set setpoint temperature
            const std::string &temp = cmdParser.getCMDOptionValue("-t", "--temp");
            const std::string &until = cmdParser.getCMDOptionValue("-u", "--until");

            if (temp == CMDParser::NO_VALUE) {

                std::cout << std::endl;
                std::cerr << "Invalid zone parameters. --temp TEMP is required." << std::endl;
                exit(EXIT_INVALID_ZONE_PARAM);
            }

            // validate temperature
            char* endptr = 0;
            strtod(temp.c_str(), &endptr);
            if (*endptr != '\0' || endptr == temp) {

                std::cout << std::endl;
                std::cerr << "Invalid temperature " << temp << "." << std::endl;
                exit(EXIT_INVALID_TEMP);
            }

            // parse until
            std::string untilParsed = DateUtil::toUTC(until);

            // set setpoint temperature
            evohomeClient.setTargetTemperature(evohomeClient.getZoneByName(zone), temp, untilParsed);

            // show the current status of all zones
            std::cout << std::endl;
            terminal.printZones(evohomeClient.getAllZones());
        }
    }
    else {

        // unknown option given
        std::cout << std::endl;
        std::cerr << "Unknown option given." << std::endl;

        // print help and exit
        terminal.printHelp(EXIT_UNKNOWN_CMD_OPTION);
    }

    return 0;
}