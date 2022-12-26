//
// Created by irfnfnkemed on 2022/12/25.
//

#ifndef BOOKSTROREOFWGJ_TRANSLATE_H
#define BOOKSTROREOFWGJ_TRANSLATE_H

#include <string>
#include <iostream>
#include <cstring>
#include <sstream>

class tokenScanner {
private:
    std::string commandLine;//表示处理的字符串
    int now = -1;//表示现在处理的字符串的位置
    int len = -1;//表示处理的字符串的长度

public:
    void scan();

    //在将等号作为分隔符的情况下，空格分隔失效，必须有等号出现，否则视作非法
    std::string nextToken();

    void popQuotations(std::string &token);

    void breakEqual(std::string &token, std::string &tokenNew);

};

void toChar_20(std::string token, char *toChar);

void toChar_30(std::string token, char *toChar);

void toChar_60(std::string token, char *toChar);

long toLong_100(std::string token);

int toInt(std::string token);

#endif //BOOKSTROREOFWGJ_TRANSLATE_H
