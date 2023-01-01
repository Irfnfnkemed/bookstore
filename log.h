//
// Created by irfnfnkemed on 2022/12/27.
//

#ifndef BOOKSTROREOFWGJ_LOG_H
#define BOOKSTROREOFWGJ_LOG_H

#include <fstream>
#include <string>
#include <iostream>
#include "login.h"

////////////////////////////////////////
//log.h主要用于实现日志、交易记录的存储和相关操作
////////////////////////////////////////


//用于记录并生成日志

const size_t charMax = 75;

class logShow {
private:

    const std::string fileLogName = "log";
    std::fstream fileLog;
    int count;//记录日志文件中信息条数
    char tmp[charMax];//用于临时储存，充当中介

public:

    //构造函数
    logShow();

    //析构函数
    ~logShow();

//---------------------------------------------------------------------

    //storeLog函数

    //将string类信息存储到log文件中
    void storeLog(std::string info);

    //将char类信息存储到log文件中
    void storeLog(const char *info);

//---------------------------------------------------------------------

    //show函数
    //打印日志
    void show();
};

//toString函数
//将long_100除以100后存入string
std::string toString(long long_100);

//存储交易总额的前缀和的类
class finance {
public:

    long in_100;//收入*100
    long out_100;//支出*100

    //构造函数
    finance() {}

    finance(long in_100_, long out_100_);

    //重载
    finance &operator-=(const finance &rhs);

    friend std::ostream &operator<<(std::ostream &os, finance rhs);
};

//用于记录交易
class log {
private:
    long count;
    const std::string fileFinanceName = "finance";
    std::fstream fileFinance;
    login *loginPoint;//指向相关联的登录栈的指针
    logShow *logShowPoint;//日志
public:

    //构造函数
    log(login *loginPoint_, logShow *logShowPoint_);

    //析构函数
    ~log();

//-----------------------------------------------

    //show函数

    //输出所有交易之总额
    //最低权限要求 {7}
    void show();

    //输出后count_笔交易之总额
    //最低权限要求 {7}
    void show(int count_);

//-----------------------------------------------

    //add函数

    //储存一笔收入/支出记录，添加到finance文件中
    //flag为true表收入，false表支出
    void add(bool flag, long add);

};


#endif //BOOKSTROREOFWGJ_LOG_H
