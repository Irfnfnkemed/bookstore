//
// Created by irfnfnkemed on 2022/12/25.
//

#ifndef BOOKSTROREOFWGJ_TRANSLATE_H
#define BOOKSTROREOFWGJ_TRANSLATE_H

#include <string>
#include <iostream>
#include <cstring>

//////////////////////////////////////////////////////////////////
//translate.h主要用于实现对命令行的切片，以及各种数据类型间的转换
//////////////////////////////////////////////////////////////////

class tokenScanner {
private:
    std::string commandLine;//表示处理的字符串
    int now = -1;//表示现在处理的字符串的位置
    int len = -1;//表示处理的字符串的长度

public:

    //读入一行命令语句
    void scan();

    //以空格为分隔符，获得下一个片段
    //若无下一个片段，返回""
    std::string nextToken();

};

//去除字符串两端的引号
//若字符串两端无引号，报错
//若字符串只有两引号，报错
//若字符串除了两端外还有引号，报错
void popQuotations(std::string &token);

//找到第一个等号，并以此为界将字符串分成两半
//若无等号出现，报错
void breakEqual(std::string &token, std::string &tokenNew);

//将string转换为char[21]
//若string为空，报错
//若string长度超过20，报错
void toChar_20(std::string token, char *toChar);

//将string转换为char[31]
//若string为空，报错
//若string长度超过30，报错
void toChar_30(std::string token, char *toChar);

//将string转换为char[61]
//若string为空，报错
//若string长度超过60，报错
void toChar_60(std::string token, char *toChar);

//将string转换为对应的浮点数的100倍（非负）
//若string为空，报错
//若string长度超过13，报错
//若格式错误，报错
//若有前导零，报错
//输入精度为两位
long toLong_100(std::string token);

//将string转换为对应的int（非负）
//若string为空，报错
//若string长度超过10，报错
//若格式错误，报错
//若有前导零，报错
//若对应数值大于int存储的最大值，报错
int toInt(std::string token);

#endif //BOOKSTROREOFWGJ_TRANSLATE_H
