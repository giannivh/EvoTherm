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
#include "app.h"
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <fstream>

Config Config::assertValidity() {

    // config location
    std::string configLocation = getConfigLocation();

    // check if file exists
    struct stat buffer;
    if (stat(configLocation.c_str(), &buffer) != 0) {

        std::string message = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        message += "@     WARNING: SETTINGS FILE NOT FOUND     @\n";
        message += "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        message += "Please create one at \"" + configLocation + "\" with following data:\n";
        message += "ET_USERNAME=your_username_here\n";
        message += "ET_PASSWORD=your_password_here\n\n";
        message += "Alternatively, you can generate one by issuing \"" + std::string(APP_EXEC) +" --config\".";

        throw EvoThermException(message, EXIT_NO_CONFIG);
    }

    // check file permissions
#ifndef _WIN32
    int statchmod = buffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    char chmod[4];
    snprintf(chmod, 4, "%o", statchmod);
    if (std::string(chmod) != "600") {

        std::string message = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        message += "@     WARNING: UNPROTECTED SETTINGS FILE     @\n";
        message += "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        message += "Permissions " + std::string(chmod) + " for \"" + configLocation + "\" are too open.\n";
        message += "The file should not be accessible by other users.\n";
        message += "Please change the file's permissions to 600.";

        throw EvoThermException(message, EXIT_INVALID_CONFIG);
    }
#endif

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

Config Config::create(const Terminal &terminal) {

    // config location
    std::string configDirectory = getConfigDirectory();
    std::string configLocation = getConfigLocation();

    // check if file exists
    struct stat buffer;
    if (stat(configLocation.c_str(), &buffer) == 0 && !terminal.askYesNo("Do you want to overwrite your current config? ", true)) {

        throw EvoThermException("Config creation aborted by user.", EXIT_CONFIG_ABORTED);
    }

    // ask email address and password
    terminal.printMessage("Before we get started, we need your Honeywell My Total Connect Comfort credentials.");
    std::string username = terminal.getInput("What is your email address? ");
    std::string password = terminal.getInputMasked("What is your password? ");

    // mkdir .evotherm
#ifdef _WIN32
    _mkdir(configDirectory.c_str());
#else
    mkdir(configDirectory.c_str(), 0755);
#endif

    // (over)write file
    std::ofstream outfile(configLocation);
    outfile << "ET_USERNAME=" << username << std::endl;
    outfile << "ET_PASSWORD=" << password << std::endl;
    outfile.close();

    // chmod 600
#ifndef _WIN32
    chmod(configLocation.c_str(), (__mode_t) strtol("0600", 0, 8));
#endif

    terminal.printMessage("\nConfig file created at \"" + configLocation + "\"!");

    return Config(username, password);
}

const std::string Config::getConfigDirectory() {

    // get home dir
    const char* homedir;
    if ((homedir = getenv("HOME")) == NULL) {

        homedir = getpwuid(getuid())->pw_dir;
    }

    // config directory
    return std::string(homedir) + "/.evotherm";
}

const std::string Config::getConfigLocation() {

    // config location
    return getConfigDirectory() + "/config";
}

Config::Config(std::string username, std::string password)
        : username(username), password(password) {

}
