//
// Created by irfnfnkemed on 2022/12/27.
//

#include "log.h"
#include "error.h"
#include <iomanip>


finance::finance(long in_100_, long out_100_) {
    in_100 = in_100_;
    out_100 = out_100_;
}

finance &finance::operator-=(const finance &rhs) {
    in_100 -= rhs.in_100;
    out_100 -= rhs.out_100;
    return *this;
}

std::ostream &operator<<(std::ostream &os, finance rhs) {
    os << std::fixed << std::setprecision(2)
       << "+ " << double(rhs.in_100) / 100
       << " - " << double(rhs.out_100) / 100 << '\n';//输出收入支出
    return os;
}

log::log(login *loginPoint_) {
    loginPoint = loginPoint_;
    //打开文件
    fileFinance.open(fileFinanceName, std::ios::in);
    if (fileFinance.is_open()) {
        fileFinance.close();
        fileFinance.open(fileFinanceName, std::ios::in | std::ios::out);
    } else {
        fileFinance.open(fileFinanceName, std::ios::out);
        fileFinance.close();
        fileFinance.open(fileFinanceName, std::ios::in | std::ios::out);
    }
    fileFinance.seekg(0, std::ios::end);
    count = fileFinance.tellg() / sizeof(finance);
}

log::~log() {
    fileFinance.close();
}

void log::show() {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 7) { error("Invalid\n"); }//权限不足，操作失败
    //读入所有交易额之和，直接输出即可
    if (count == 0) { std::cout << finance(0, 0); }
    else {
        finance gmv;
        fileFinance.seekg(-sizeof(finance), std::ios::end);
        fileFinance.read(reinterpret_cast<char *>(&gmv), sizeof(finance));
        std::cout << gmv;
    }
}

void log::show(int count_) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 7) { error("Invalid\n"); }//权限不足，操作失败
    if (count_ > count) { error("Invalid\n"); }//count_大于历史交易总笔数，操作失败
    finance gmv, gmvBefore;
    fileFinance.seekg(-sizeof(finance), std::ios::end);
    fileFinance.read(reinterpret_cast<char *>(&gmv), sizeof(finance));
    if (count_ == count) { std::cout << gmv; }//相当于输出所有交易额之和，直接输出即可
    else if (count_ == 0) { std::cout << '\n'; }//输出空行
    else {
        fileFinance.seekg(-sizeof(finance) * (count_ + 1), std::ios::end);
        fileFinance.read(reinterpret_cast<char *>(&gmvBefore), sizeof(finance));
        std::cout << (gmv -= gmvBefore);
    }
}

void log::add(bool flag, long add) {
    finance gmv(0, 0);
    if (count > 0) {
        fileFinance.seekg(-sizeof(finance), std::ios::end);
        fileFinance.read(reinterpret_cast<char *>(&gmv), sizeof(finance));
    }
    if (flag) { gmv.in_100 += add; }
    else { gmv.out_100 += add; }
    fileFinance.write(reinterpret_cast<char *>(&gmv), sizeof(finance));
    ++count;
}