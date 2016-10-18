//
// terminal.cpp is part of EvoTherm.
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

#include <iostream>
#include "terminal.h"
#include "../app.h"
#include "../markup.h"
#include "../util/dateutil.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

void Terminal::printHelp() const {

    printHelp(0);
}

void Terminal::printHelp(const int &exitCode) const {

    std::cout << APP_NAME << " v" << APP_VERSION << std::endl;
    std::cout << "   by " << APP_AUTHOR << std::endl;
    std::cout << std::endl;

    std::cout << "Usage: " << APP_EXEC << " [[-h] | [-c] | [-l] | [[-z ZONE] [[[-t TEMP] [-u UNTIL]] | [-c]]] | [[-m MODE] [-u UNTIL]]]" << std::endl;
    std::cout << std::endl;

    printHelpOption("-h", "--help", "Print this help.");
    printHelpOption("-c", "--config", "Generate config file with your credentials.");
    printHelpOption("-l", "--list", "Optional. Print current status of all zones.");
    printHelpOption("-z", "--zone", "Select zone ZONE.");
    printHelpOption("-t", "--temp", "Use temperature setpoint TEMP.");
    printHelpOption("-u", "--until", "Optional. Set setpoint/mode until UNTIL, leave out to make permanent. (Format \"yyyy-MM-dd HH:mm:ss\")");
    printHelpOption("-c", "--cancel", "Cancel temperature setpoint override for the selected zone.");
    printHelpOption("-m", "--mode", "Set thermostat mode to MODE.");
    std::cout << std::endl;

    std::cout << "The possible MODEs are programmed through the thermostat and are one of the following:" << std::endl;
    printHelpMode("auto", "Follow the normal schedule.");
    printHelpMode("eco", "Follow the eco schedule.");
    printHelpMode("away", "Follow the away schedule.");
    printHelpMode("dayoff", "Follow the weekend schedule.");
    printHelpMode("off", "All heating off.");
    printHelpMode("custom", "Follow the custom schedule.");
    std::cout << std::endl;

    std::cout << "Example: " << std::endl;
    printHelpExample("--config");
    printHelpExample("--list");
    printHelpExample("--zone \"Bathroom\" --temp \"24\" --until \"2016-10-14 20:00:00\"");
    printHelpExample("--zone \"Bathroom\" --cancel");
    printHelpExample("--mode \"eco\" --until \"2016-10-14 20:00:00\"");
    std::cout << std::endl;

    exit(exitCode);
}


void Terminal::printHelpOption(const std::string &shortOption, const std::string &longOption,
                               const std::string &description) const {

    printf("%-5s %-10s %s\n", shortOption.c_str(), longOption.c_str(), description.c_str());
}

void Terminal::printHelpMode(const std::string &mode, const std::string &description) const {

    //std::cout << mode << "\t\t" << description << std::endl;
    printf("%-10s %s\n", mode.c_str(), description.c_str());
}

void Terminal::printHelpExample(const std::string &example) const {

    std::cout << "\t$ " << APP_EXEC << " " << example << std::endl;
}

void Terminal::printZones(const InstallationInfo &installationInfo) const {

    // print mode
    std::cout << "Your " << installationInfo.modelType << " is in " <<
              BOLD << installationInfo.currentMode << RESET << " mode";
    if (installationInfo.currentModeUntil != "") {

        std::cout << " until " << BOLD << installationInfo.currentModeUntil << RESET;
    }
    std::cout << "." << std::endl << std::endl;

    // print table header
    printf("+%s+%s+%s+%s+\n", std::string(20,'-').c_str(), std::string(15,'-').c_str(), std::string(15,'-').c_str(),
           std::string(27,'-').c_str());
    printf("| %-18s | %-13s | %-13s | %-25s |\n", "Zone", "Temperature", "Set Point", "Mode");
    printf("+%s+%s+%s+%s+\n", std::string(20,'-').c_str(), std::string(15,'-').c_str(), std::string(15,'-').c_str(),
           std::string(27,'-').c_str());

    // print data
    const std::vector<Zone> &zones = installationInfo.zones;
    for (int i = 0; i < zones.size(); i++) {

        // set color
        std::string color = BLUE_WARM;
        if (zones[i].targetTemperature >= TEMP_HIGH) {

            color = ORANGE_WARM;
        }
        else if (zones[i].targetTemperature >= TEMP_MEDIUM) {

            color = ORANGE_LIGHT;
        }
        else if (zones[i].targetTemperature >= TEMP_LOW) {

            color = GREEN_LIGHT;
        }

        // set format
        std::string format = " ";
        if (zones[i].temperature < zones[i].targetTemperature) {

            format = CHAR_RISING;
        }
        else if (zones[i].temperature > zones[i].targetTemperature) {

            color = color + BOLD;
        }

        // convert doubles
        char tempD[6];
        sprintf(tempD, "%.2f", zones[i].temperature);
        char spD[6];
        sprintf(spD, "%.2f", zones[i].targetTemperature);

        // convert mode
        std::string schedule = "";
        if (zones[i].setpointMode == "FollowSchedule") {

            schedule = "Following schedule";
        }
        else if (zones[i].setpointMode == "TemporaryOverride") {

            schedule = "Until " + DateUtil::toLocal(zones[i].until);
        }
        else if (zones[i].setpointMode == "PermanentOverride") {

            schedule = "Permanent";
        }
        else {

            schedule = zones[i].setpointMode;
        }

        // print row
        printf("| %s%-18s%s | %s%s%9s °C%s | %s%10s °C%s | %s%-25s%s |\n",
               color.c_str(), zones[i].name.c_str(), RESET,
               color.c_str(), format.c_str(), tempD, RESET,
               color.c_str(), spD, RESET,
               color.c_str(), schedule.c_str(), RESET);
    }

    // print table footer
    printf("+%s+%s+%s+%s+\n", std::string(20,'-').c_str(), std::string(15,'-').c_str(), std::string(15,'-').c_str(),
           std::string(27,'-').c_str());
}

void Terminal::printFatalError(const EvoThermException &evoThermException) const {

    std::cerr << evoThermException.what() << std::endl;

    exit(evoThermException.getExitCode());
}

void Terminal::printError(const std::string &message) const {

    std::cerr << message << std::endl;
}

void Terminal::printSuccess(const std::string &message) const {

    std::cout << GREEN_LIGHT << message << RESET << std::endl << std::endl;
}

void Terminal::printMessage(const std::string &message) const {

    std::cout << message << std::endl;
}

const std::string Terminal::getInput(const std::string &message) const {

    std::cout << message; // no new line

    std::string input = "";

    getline(std::cin, input);

    return input;
}

const std::string Terminal::getInputMasked(const std::string &message) const {

    this->setSTDINEcho(false);
    std::string input = this->getInput(message);
    this->setSTDINEcho(true);

    return input;
}

const bool Terminal::askYesNo(const std::string &message, const bool &defaultOption) const {

    std::string input = "";
    std::string append = std::string("[") + (defaultOption? "Yn": "yN") + "] ";

    while (true) {

        input = getInput(message + append);

        if (input == "y" || input == "yes" || (defaultOption && input == "")) {

            return true;
        }
        else if (input == "n" || input == "no" || (!defaultOption && input == "")) {

            return false;
        }
        else {

            this->printError("Please write y or n.");
        }
    }
}


void Terminal::setSTDINEcho(const bool &echoEnabled) const {

#ifdef _WIN32

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (echoEnabled) {

        mode |= ENABLE_ECHO_INPUT;
    } else {

        mode &= ~ENABLE_ECHO_INPUT;
    }

    SetConsoleMode(hStdin, mode);

#else

    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);

    if (echoEnabled) {

        tty.c_lflag |= ECHO;
    } else {

        tty.c_lflag &= ~ECHO;
    }

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);

#endif
}

const void Terminal::exitClean() const {

    exit(0);
}
