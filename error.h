//
// Created by irfnfnkemed on 2022/12/23.
//

#ifndef BOOKSTROREOFWGJ_ERROR_H
#define BOOKSTROREOFWGJ_ERROR_H

#include <exception>
#include <string>

/////////////////////////////////////
//error.h主要用于实现异常的抛出
/////////////////////////////////////

class errorException : public std::exception {
private:
    std::string message;
public:
    //构造函数
    explicit errorException(std::string message);

    //获得错误信息
    std::string getMessage() const;
};

//抛出错误
void error(std::string message);

#endif //BOOKSTROREOFWGJ_ERROR_H
