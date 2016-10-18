//
// config.cpp is part of EvoTherm.
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

#include "config.h"
#include "exitcode.h"
#include "exception/evothermexception.h"
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <fstream>

Config Config::assertValidity() {

    // get home dir
    const char* homedir;
    if ((homedir = getenv("HOME")) == NULL) {

        homedir = getpwuid(getuid())->pw_dir;
    }

    // config location
    std::string configLocation = std::string(homedir) + "/.evotherm/config";

    // check if file exists
    struct stat buffer;
    if (stat(configLocation.c_str(), &buffer) != 0) {

        std::string message = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        message += "@     WARNING: SETTINGS FILE NOT FOUND     @\n";
        message += "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        message += "Please create one at '" + configLocation + "' with following data:\n";
        message += "ET_USERNAME=your_username_here\n";
        message += "ET_PASSWORD=your_password_here";

        throw EvoThermException(message, EXIT_NO_CONFIG);
    }

    // check file permissions
    int statchmod = buffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    char chmod[4];
    snprintf(chmod, 4, "%o", statchmod);
    if (std::string(chmod) != "600") {

        std::string message = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        message += "@     WARNING: UNPROTECTED SETTINGS FILE     @\n";
        message += "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        message += "Permissions " + std::string(chmod) + " for '" + configLocation + "' are too open.\n";
        message += "The file should not be accessible by other users.\n";
        message += "Please change the file's permissions to 600.";

        throw EvoThermException(message, EXIT_INVALID_CONFIG);
    }

    // parse file
    std::ifstream infile(configLocation.c_str());
    std::string line;
    std::string username;
    std::string password;
    while (std::getline(infile, line)) {

        unsigned long pos = line.find('=');
        if (pos == std::string::npos) continue; // no '=', invalid line
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1, line.length() - pos + 1);

        if (key == "ET_USERNAME") {

            username = value;
        }
        else if (key == "ET_PASSWORD") {

            password = value;
        }
    }

    return Config(username, password);
}

Config::Config(std::string username, std::string password)
        : username(username), password(password) {

}

std::string Config::getUsername() {

    return this->username;
}

std::string Config::getPassword() {

    return this->password;
}
