//
// Created by irfnfnkemed on 2022/12/25.
//

#include "translate.h"
#include "error.h"

void tokenScanner::scan() {
    commandLine.clear();
    getline(std::cin, commandLine);
    if (std::cin.eof()) { exit(0); }
    now = 0;
    len = commandLine.length();
}

std::string tokenScanner::nextToken() {
    if (now == -1) { return ""; }//空
    else if (now == len) {//读到末尾
        commandLine.clear();
        now = len = -1;
        return "";
    } else {
        std::string tmp;
        while (commandLine[now] == ' ') { ++now; }
        while (commandLine[now] != ' ' && now < len) {
            tmp = tmp + commandLine[now];
            ++now;
        }
        return tmp;
    }
}

void popQuotations(std::string &token) {
    if (token.length() <= 2) { error("invalid\n"); }
    if (token[0] != '\"' || token[token.length() - 1] != '\"') { error("invalid\n"); }
    token.pop_back();
    token.erase(0, 1);
}

void breakEqual(std::string &token, std::string &tokenNew) {
    int i = token.find_first_of('=');
    if (i <= 0 || i >= token.length()) { error("invalid\n"); }
    tokenNew = token.substr(i + 1, token.length() - i - 1);
    token.erase(i, token.length() - i);
}

void toChar_20(std::string token, char *toChar) {
    if (token.length() > 20) { error("invalid\n"); }
    if (token.length() == 0) { toChar[0] = '\0'; }
    else {
        for (int i = 0; i < token.length(); ++i) {
            toChar[i] = token[i];
        }
        toChar[token.length()] = '\0';
    }
}


void toChar_30(std::string token, char *toChar) {
    if (token.length() > 30) { error("invalid\n"); }
    if (token.length() == 0) { toChar[0] = '\0'; }
    else {
        for (int i = 0; i < token.length(); ++i) {
            toChar[i] = token[i];
        }
        toChar[token.length()] = '\0';
    }
}

void toChar_60(std::string token, char *toChar) {
    if (token.length() > 60) { error("invalid\n"); }
    if (token.length() == 0) { toChar[0] = '\0'; }
    else {
        for (int i = 0; i < token.length(); ++i) {
            toChar[i] = token[i];
        }
        toChar[token.length()] = '\0';
    }
}

long toLong_100(std::string token) {
    if (token.length() > 13) { error("invalid\n"); }
    if (token.length() == 0) { return 0; }
    long tmp = 0;
    int flag = 2;//防止记录'.'位置
    for (int i = 0; i < token.length(); ++i) {
        if (token[i] >= '0' && token[i] <= '9') {
            tmp = tmp * 10 + (token[i] - '0');
        } else if (token[i] == '.' && i >= token.length() - 3 &&
                   i > 0 && i < token.length() - 1 && flag == 2) {//防止出现多个'.'，防止'.'出现在最后、两位小数之前
            flag = i - token.length() + 3;
            continue;
        } else { error("invalid\n"); }//输入不合法
    }
    for (int i = 1; i <= flag; ++i) { tmp *= 10; }
    return tmp;
}

int toInt(std::string token) {
    if (token.length() > 10) { error("invalid\n"); }
    if (token.length() == 0) { return 0; }
    long tmp = 0;
    for (int i = 0; i < token.length(); ++i) {
        if (token[i] >= '0' && token[i] <= '9') {
            tmp = tmp * 10 + (token[i] - '0');
        } else { error("invalid\n"); }//输入不合法
    }
    if (tmp > 2147483647) { error("invalid\n"); }//大数不合法
    return int(tmp);
}