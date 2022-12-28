//
// Created by irfnfnkemed on 2022/12/27.
//

#ifndef BOOKSTROREOFWGJ_LOG_H
#define BOOKSTROREOFWGJ_LOG_H

#include <fstream>
#include <string>
#include <iostream>
#include "login.h"

//存储交易总额的前缀和的类
class finance {
public:
    long in_100;
    long out_100;

    finance() {}

    finance(long in_100_, long out_100_);

    finance &operator-=(const finance &rhs);

    friend std::ostream &operator<<(std::ostream &os, finance rhs);
};


class log {
private:
    long count;
    const std::string fileFinanceName = "finance";
    std::fstream fileFinance;
    login *loginPoint;//指向相关联的登录栈的指针

public:

//-----------------------------------------------

    //构造函数
    log(login *loginPoint_);

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
