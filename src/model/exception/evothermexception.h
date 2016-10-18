//
// Created by Gianni Van Hoecke on 18/10/2016.
//

#ifndef EVOTHERM_EVOCLIENTEXCEPTION_H
#define EVOTHERM_EVOCLIENTEXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

class EvoThermException : public std::exception {

public:

    EvoThermException(const std::string &message, const int &exitCode);

    virtual const char* what() const throw();

    const int &getExitCode() const;

private:

    const std::string message;
    const int exitCode;

};


#endif //EVOTHERM_EVOCLIENTEXCEPTION_H
