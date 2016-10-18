//
// Created by Gianni Van Hoecke on 18/10/2016.
//

#include "evothermexception.h"

EvoThermException::EvoThermException(const std::string &message, const int &exitCode)
        : message(message), exitCode(exitCode) {

}

const char *EvoThermException::what() const throw() {

    return this->message.c_str();
}

const int &EvoThermException::getExitCode() const {

    return this->exitCode;
}
