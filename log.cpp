//
// Created by irfnfnkemed on 2022/12/27.
//

#include "log.h"
#include "error.h"
#include <iomanip>
#include <cstring>


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

log::log(login *loginPoint_, logShow *logShowPoint_) {
    loginPoint = loginPoint_;
    logShowPoint = logShowPoint_;
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
    //添加日志
    logShowPoint->storeLog("Account[" + std::string(loginPoint->front()->data.index) +
                           "] show all finance.\n");
}

void log::show(int count_) {
    if (loginPoint->empty()) { error("Invalid\n"); }//无账户登录，操作失败
    if (loginPoint->front()->data.privilege < 7) { error("Invalid\n"); }//权限不足，操作失败
    if (count_ > count) { error("Invalid\n"); }//count_大于历史交易总笔数，操作失败
    if (count_ == 0) { std::cout << '\n'; }//输出空行
    else {
        finance gmv, gmvBefore;
        fileFinance.seekg(-sizeof(finance), std::ios::end);
        fileFinance.read(reinterpret_cast<char *>(&gmv), sizeof(finance));
        if (count_ == count) { std::cout << gmv; }//相当于输出所有交易额之和，直接输出即可
        else {
            fileFinance.seekg(-sizeof(finance) * (count_ + 1), std::ios::end);
            fileFinance.read(reinterpret_cast<char *>(&gmvBefore), sizeof(finance));
            std::cout << (gmv -= gmvBefore);
        }
    }
    //添加日志
    logShowPoint->storeLog("Account[" + std::string(loginPoint->front()->data.index) +
                           "] show finance with count[");
    logShowPoint->storeLog(std::to_string(count_) + "].\n");
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

logShow::logShow() {
    //打开文件
    fileLog.open(fileLogName, std::ios::in);
    if (fileLog.is_open()) {
        fileLog.close();
        fileLog.open(fileLogName, std::ios::in | std::ios::out);
    } else {
        fileLog.open(fileLogName, std::ios::out);
        fileLog.close();
        fileLog.open(fileLogName, std::ios::in | std::ios::out);
    }
    fileLog.seekg(0, std::ios::end);
    count = fileLog.tellg() / charMax;
    //若程序中途退出（未析构），则在文件中补充信息
    if (count > 0) {
        fileLog.seekg(-charMax, std::ios::end);
        fileLog.read(tmp, charMax);
        if (strcmp(tmp, "------------------------------\nExit the bookstore system.\n\n") != 0) {
            std::string tmpString = "\n------------------------------\nThe system was unexpectedly exited.\n\n";
            for (int i = 0; i < tmpString.length(); ++i) { tmp[i] = tmpString[i]; }
            tmp[tmpString.length()] = '\0';
            fileLog.write(tmp, charMax);
            ++count;
        }
    }
}

logShow::~logShow() {
    fileLog.close();
}

void logShow::storeLog(std::string info) {
    for (int i = 0; i < info.length(); ++i) { tmp[i] = info[i]; }
    tmp[info.length()] = '\0';
    fileLog.seekg(0, std::ios::end);
    fileLog.write(tmp, charMax);
    ++count;
}

void logShow::storeLog(const char *info) {
    for (int i = 0; i < charMax; ++i) {
        tmp[i] = info[i];
        if (info[i] == '\0') { break; }
    }
    fileLog.seekg(0, std::ios::end);
    fileLog.write(tmp, charMax);
    ++count;
}

void logShow::show() {
    fileLog.seekg(0, std::ios::beg);
    for (int i = 1; i <= count; ++i) {
        fileLog.read(tmp, charMax);
        std::cout << tmp;
    }
}

std::string toString(long long_100) {
    std::string tmp = std::to_string(long_100);
    if (tmp.length() == 1) { tmp.insert(0, 2, '0'); }
    else if (tmp.length() == 2) { tmp.insert(0, 1, '0'); }
    tmp.insert(tmp.length() - 2, 1, '.');
    return tmp;
}
