//
// Created by irfnfnkemed on 2022/12/23.
//

#include "error.h"

errorException::errorException(std::string message) {
    this->message = message;
}

std::string errorException::getMessage() const {
    return message;
}


void error(std::string message) {
    throw errorException(message);
}