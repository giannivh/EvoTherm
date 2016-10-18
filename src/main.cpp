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

    try {

        // assert config validity
        Config config = Config::assertValidity();

        // we need data if we get here...
        EvohomeClient evohomeClient = EvohomeClient(config);


        // parse options
        if (argc == 1 || cmdParser.cmdOptionGiven("-l", "--list")) {

            // list will be printed at the end.
            // nothing to do here...
        } else if (cmdParser.cmdOptionGiven("-m", "--mode")) {

            // get and check mode
            const std::string &mode = cmdParser.getCMDOptionValue("-m", "--mode");
            const std::string &until = cmdParser.getCMDOptionValue("-u", "--until");

            if (mode == CMDParser::NO_VALUE || !Mode::isValidMode(mode)) {

                throw EvoThermException("Invalid mode.", EXIT_INVALID_MODE);
            }

            // parse until
            std::string untilParsed = DateUtil::toUTC(until);

            // set thermostat mode
            evohomeClient.setMode(mode, untilParsed);
        } else if (cmdParser.cmdOptionGiven("-z", "--zone")) {

            // get zone
            const std::string &zone = cmdParser.getCMDOptionValue("-z", "--zone");

            // check zone
            if (zone == CMDParser::NO_VALUE || !evohomeClient.hasZone(zone)) {

                throw EvoThermException("Invalid zone.", EXIT_INVALID_ZONE);
            }

            // operation on zone
            if (cmdParser.cmdOptionGiven("-c", "--cancel")) {

                // cancel override
                evohomeClient.cancelOverride(evohomeClient.getZoneByName(zone));
            } else {

                // set setpoint temperature
                const std::string &temp = cmdParser.getCMDOptionValue("-t", "--temp");
                const std::string &until = cmdParser.getCMDOptionValue("-u", "--until");

                if (temp == CMDParser::NO_VALUE) {

                    throw EvoThermException("Invalid zone parameters. --temp TEMP is required.", EXIT_INVALID_ZONE_PARAM);
                }

                // validate temperature
                char *endptr = 0;
                strtod(temp.c_str(), &endptr);
                if (*endptr != '\0' || endptr == temp) {

                    throw EvoThermException("Invalid temperature " + temp + ".", EXIT_INVALID_TEMP);
                }

                // parse until
                std::string untilParsed = DateUtil::toUTC(until);

                // set setpoint temperature
                evohomeClient.setTargetTemperature(evohomeClient.getZoneByName(zone), temp, untilParsed);
            }
        } else {

            // unknown option given
            terminal.printError("Unknown option given.");

            // print help and exit
            terminal.printHelp(EXIT_UNKNOWN_CMD_OPTION);
        }

        // show the current status of all zones
        std::cout << std::endl;
        terminal.printZones(evohomeClient.getInstallationInfo());
    }
    catch (EvoThermException &evoThermException) {

        terminal.printFatalError(evoThermException);
    }

    return 0;
}