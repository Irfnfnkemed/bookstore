//
// Created by irfnfnkemed on 2022/12/23.
//

#ifndef BOOKSTROREOFWGJ_ERROR_H
#define BOOKSTROREOFWGJ_ERROR_H

#include <exception>
#include <string>

class errorException : public std::exception {
private:
    std::string message;
public:
    explicit errorException(std::string message);

    std::string getMessage() const;
};

void error(std::string message);

#endif //BOOKSTROREOFWGJ_ERROR_H
